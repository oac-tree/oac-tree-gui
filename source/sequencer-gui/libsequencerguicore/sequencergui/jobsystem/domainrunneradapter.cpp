/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/domainrunneradapter.h"

#include "Procedure.h"
#include "Runner.h"
#include "sequencergui/jobsystem/functionrunner.h"

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

bool DomainRunnerAdapter::Start()
{
  std::cout << "DomainRunnerAdapter::Start()" << std::endl;
  ResetIfNecessary();

  return m_function_runner->Start();
}

bool DomainRunnerAdapter::Stop()
{
  m_domain_runner->Halt();
  auto result = m_function_runner->Stop();
  if (result)
  {
    m_was_stopped = true;
  }

  return result;
}

bool DomainRunnerAdapter::Pause()
{
  return m_function_runner->Pause();
}

bool DomainRunnerAdapter::Step()
{
  ResetIfNecessary();
  bool result = m_function_runner->Step();
  return result;
}

RunnerStatus DomainRunnerAdapter::GetStatus() const
{
  return m_function_runner->GetStatus();
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

void DomainRunnerAdapter::ResetIfNecessary()
{
  // Reset if procedure was explicitely stopped, or was terminated normally
  if (m_was_stopped || m_procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::SUCCESS)
  {
    m_procedure->Reset();
  }
};

DomainRunnerAdapter::~DomainRunnerAdapter()
{
  // Line below is commented since we don't now if the underlying procedure is still alive.
  // So attempt to delete the runner during procedure execution will lead to UB
  // m_domain_runner->Halt();
  // FIXME find solution, see comments DomainRunnerAdapterTest::PrematureDeletion
}

}  // namespace sequencergui
