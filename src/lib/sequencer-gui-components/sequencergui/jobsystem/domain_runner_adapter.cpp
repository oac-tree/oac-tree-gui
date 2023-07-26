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

#include "domain_runner_adapter.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <chrono>
#include <thread>
#include <iostream>

namespace sequencergui
{

DomainRunnerAdapter::DomainRunnerAdapter(const DomainRunnerContext &context) : m_context(context)
{
  m_domain_runner = std::make_unique<runner_t>(*m_context.user_interface);
  m_domain_runner->SetProcedure(m_context.procedure);

  auto tick_callback = [this](const procedure_t &procedure)
  {
    auto is_running = !m_domain_runner->IsFinished();
    if (is_running && m_tick_timeout_ms.load() > 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_ms.load()));
    }
    if (m_context.m_tick_cb)
    {
      m_context.m_tick_cb(procedure);
    }
  };
  m_domain_runner->SetTickCallback(tick_callback);
}

DomainRunnerAdapter::~DomainRunnerAdapter() = default;

void DomainRunnerAdapter::SetTickTimeout(int msec)
{
  m_tick_timeout_ms.store(msec);
}

bool DomainRunnerAdapter::IsBusy() const
{
  if (!m_future_result.valid())
  {
    return false;
  }

  return m_future_result.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
}

void DomainRunnerAdapter::StartRequest()
{
  RunProcedure(/*in_step_mode*/ false);
}

void DomainRunnerAdapter::PauseModeOnRequest()
{
  m_domain_runner->Pause();
  SetStatus(RunnerStatus::kPaused);
}

void DomainRunnerAdapter::PauseModeOffRequest()
{
  StartRequest();
}

void DomainRunnerAdapter::StepRequest()
{
  RunProcedure(/*in_step_mode*/ true);
}

void DomainRunnerAdapter::StopRequest()
{
  SetStatus(RunnerStatus::kStopping);
  m_domain_runner->Halt();
  SetStatus(RunnerStatus::kStopped);
}


runner_t *DomainRunnerAdapter::GetDomainRunner()
{
  return m_domain_runner.get();
}

void DomainRunnerAdapter::OnStatusChange(RunnerStatus status)
{
  m_context.runner_status_changed_cb(status);
}

void DomainRunnerAdapter::RunProcedure(bool in_step_mode)
{
  if (m_future_result.valid() && GetStatus() != RunnerStatus::kPaused)
  {
    throw RuntimeException("DomainRunnerAdapterNew::StartRequest() -> Already running job");
  }

  SetStatus(RunnerStatus::kRunning);

  auto worker = [this, in_step_mode]()
  {
    try
    {
      if (in_step_mode)
      {
        auto old_timeout = m_tick_timeout_ms.load();
        SetTickTimeout(0);
        m_domain_runner->ExecuteSingle();
        SetTickTimeout(old_timeout);
      }
      else
      {
        m_domain_runner->ExecuteProcedure();
      }

      UpdateStatusOnRunnerCompletion();
    }
    catch (const std::exception &ex)
    {
      SetStatus(RunnerStatus::kFailed);
    }
  };
  m_future_result = std::async(std::launch::async, worker);
}

void DomainRunnerAdapter::UpdateStatusOnRunnerCompletion()
{
  // can't find more elegant way

  if (GetStatus() == RunnerStatus::kRunning)
  {
    if (m_domain_runner->IsFinished())
    {
      SetStatus(RunnerStatus::kCompleted);
    }
    else
    {
      SetStatus(RunnerStatus::kPaused);
    }
  }
  else if (GetStatus() == RunnerStatus::kStopping)
  {
    SetStatus(RunnerStatus::kStopped);
  }
}

}  // namespace sequencergui
