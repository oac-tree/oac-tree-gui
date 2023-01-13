/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/domain_runner_adapter.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/function_runner.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace sequencergui
{

DomainRunnerAdapter::DomainRunnerAdapter(procedure_t *procedure, userinterface_t *interface,
                                         std::function<void(RunnerStatus)> status_changed_callback)
    : m_procedure(procedure), m_userinterface(interface)
{
  m_domain_runner = std::make_unique<runner_t>(m_userinterface);
  m_domain_runner->SetProcedure(m_procedure);
  auto worker = [this] { return ExecuteSingle(); };
  m_function_runner = std::make_unique<FunctionRunner>(worker, std::move(status_changed_callback));
}

RunnerStatus DomainRunnerAdapter::GetStatus() const
{
  return m_function_runner->GetStatus();
}

void DomainRunnerAdapter::SetStatus(RunnerStatus status)
{
  m_function_runner->SetStatus(status);
}

bool DomainRunnerAdapter::WaitForCompletion(std::chrono::milliseconds timeout_msec)
{
  return ::sequencergui::WaitForCompletion(*m_function_runner, timeout_msec);
}

void DomainRunnerAdapter::SetTickTimeout(int msec)
{
  m_tick_timeout_ms.store(msec);
}

bool DomainRunnerAdapter::IsBusy() const
{
  return m_function_runner->IsBusy();
}

void DomainRunnerAdapter::StartRequest()
{
  if (GetStatus() != RunnerStatus::kIdle)
  {
    throw RuntimeException("Domain runner is not intended to start the job twice");
  }

  m_function_runner->StartRequest();
}

void DomainRunnerAdapter::PauseModeOnRequest()
{
  m_function_runner->PauseModeOnRequest();
}

void DomainRunnerAdapter::PauseModeOffRequest()
{
  m_function_runner->PauseModeOffRequest();
}

void DomainRunnerAdapter::StepRequest()
{
  m_function_runner->StepRequest();
}

void DomainRunnerAdapter::StopRequest()
{
  m_domain_runner->Halt();
  m_function_runner->StopRequest();
}

void DomainRunnerAdapter::OnStatusChange(RunnerStatus status)
{
  m_function_runner->OnStatusChange(status);
}

bool DomainRunnerAdapter::ExecuteSingle()
{
  m_domain_runner->ExecuteSingle();
  auto is_running = !m_domain_runner->IsFinished();
  if (is_running && m_tick_timeout_ms.load() > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_ms.load()));
  }

  return is_running;
}

DomainRunnerAdapter::~DomainRunnerAdapter()
{
  // Line below is commented since we don't now if the underlying procedure is still alive.
  // So attempt to delete the runner during procedure execution will lead to UB
  // m_domain_runner->Halt();
  // FIXME find solution, see comments DomainRunnerAdapterTest::PrematureDeletion
}

}  // namespace sequencergui
