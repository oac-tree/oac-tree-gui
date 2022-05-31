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

#include "Runner.h"
#include "sequencergui/jobsystem/functionrunner.h"

#include <chrono>
#include <thread>

using msec = std::chrono::milliseconds;

namespace sequencergui
{

DomainRunnerAdapter::DomainRunnerAdapter(std::unique_ptr<runner_t> domain_runner,
                                         std::function<void(RunnerStatus)> status_changed_callback)
    : m_domain_runner(std::move(domain_runner))
{
  auto worker = [this]()
  {
    auto result = ExecuteSingle();
    if (result && m_tick_timeout_ms.load() > 0)
    {
      std::this_thread::sleep_for(msec(m_tick_timeout_ms.load()));
    }
    return result;
  };
  m_function_runner = std::make_unique<FunctionRunner>(worker, std::move(status_changed_callback));
}

bool DomainRunnerAdapter::Start()
{
  return m_function_runner->Start();
}

bool DomainRunnerAdapter::Stop()
{
  m_domain_runner->Halt();
  return m_function_runner->Stop();
}

bool DomainRunnerAdapter::Pause()
{
  return m_function_runner->Pause();
}

bool DomainRunnerAdapter::Step()
{
  return m_function_runner->Step();
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
  return !m_domain_runner->IsFinished();
}

DomainRunnerAdapter::~DomainRunnerAdapter()
{
  // Line below is commented since we don't now if the underlying procedure is still alive.
  // So attempt to delete the runner during procedure execution will lead to UB
  // m_domain_runner->Halt();
  // FIXME find solution, see comments DomainRunnerAdapterTest::PrematureDeletion
}

}  // namespace sequencergui
