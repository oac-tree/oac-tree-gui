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

#include "domain_runner_adapter_new.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/function_runner.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace sequencergui
{

DomainRunnerAdapterNew::DomainRunnerAdapterNew(procedure_t *procedure, userinterface_t *interface,
                                         std::function<void(RunnerStatus)> status_changed_callback)
    : m_procedure(procedure), m_userinterface(interface)
{
  m_domain_runner = std::make_unique<runner_t>(*m_userinterface);
  m_domain_runner->SetProcedure(m_procedure);
  auto worker = [this] { return ExecuteSingle(); };
  m_function_runner = std::make_unique<FunctionRunner>(worker, std::move(status_changed_callback));
}

DomainRunnerAdapterNew::~DomainRunnerAdapterNew() = default;


RunnerStatus DomainRunnerAdapterNew::GetStatus() const
{
  return m_function_runner->GetStatus();
}

void DomainRunnerAdapterNew::SetStatus(RunnerStatus status)
{
  m_function_runner->SetStatus(status);
}

void DomainRunnerAdapterNew::SetTickTimeout(int msec)
{
  m_tick_timeout_ms.store(msec);
}

bool DomainRunnerAdapterNew::IsBusy() const
{
  return m_function_runner->IsBusy();
}

void DomainRunnerAdapterNew::StartRequest()
{
  if (GetStatus() != RunnerStatus::kIdle)
  {
    throw RuntimeException("Domain runner is not intended to start the job twice");
  }

  m_function_runner->StartRequest();
}

void DomainRunnerAdapterNew::PauseModeOnRequest()
{
  m_function_runner->PauseModeOnRequest();
}

void DomainRunnerAdapterNew::PauseModeOffRequest()
{
  m_function_runner->PauseModeOffRequest();
}

void DomainRunnerAdapterNew::StepRequest()
{
  m_function_runner->StepRequest();
}

void DomainRunnerAdapterNew::StopRequest()
{
  m_domain_runner->Halt();
  m_function_runner->StopRequest();
}

void DomainRunnerAdapterNew::OnStatusChange(RunnerStatus status)
{
  m_function_runner->OnStatusChange(status);
}

bool DomainRunnerAdapterNew::ExecuteSingle()
{
  m_domain_runner->ExecuteSingle();
  auto is_running = !m_domain_runner->IsFinished();
  if (is_running && m_tick_timeout_ms.load() > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_ms.load()));
  }

  return is_running;
}


}  // namespace sequencergui
