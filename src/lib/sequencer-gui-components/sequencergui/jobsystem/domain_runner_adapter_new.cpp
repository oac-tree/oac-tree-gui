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

#include "abstract_job_helper.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace sequencergui
{

DomainRunnerAdapterNew::DomainRunnerAdapterNew(
    procedure_t *procedure, userinterface_t *interface,
    std::function<void(RunnerStatus)> status_changed_callback)
    : m_procedure(procedure)
    , m_userinterface(interface)
    , m_status_changed_callback(status_changed_callback)
{
  m_domain_runner = std::make_unique<runner_t>(*m_userinterface);
  m_domain_runner->SetProcedure(m_procedure);

  auto tick_callback = [this](const procedure_t&)
  {
    auto is_running = !m_domain_runner->IsFinished();
    if (is_running && m_tick_timeout_ms.load() > 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_ms.load()));
    }
  };
  m_domain_runner->SetTickCallback(tick_callback);
}

DomainRunnerAdapterNew::~DomainRunnerAdapterNew() = default;

void DomainRunnerAdapterNew::SetTickTimeout(int msec)
{
  m_tick_timeout_ms.store(msec);
}

bool DomainRunnerAdapterNew::IsBusy() const
{
  if (!m_future_result.valid())
  {
    return false;
  }

  return m_future_result.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
}

bool DomainRunnerAdapterNew::IsInPauseMode() const
{
  return m_pause_mode_request;
}

void DomainRunnerAdapterNew::StartRequest()
{
  if (m_future_result.valid())
  {
    std::cout
        << "DomainRunnerAdapterNew::StartRequest() -> Already running job, waiting for previous "
           "operation to complete"
        << std::endl;
    m_future_result.wait();
  }

  // deliberately before thread start
  SetStatus(RunnerStatus::kRunning);

  auto worker = [this]()
  {
    try
    {
      std::cout << "AAA 1.1" << std::endl;
      m_domain_runner->ExecuteProcedure();
      std::cout << "AAA 1.2" << std::endl;

      if (GetStatus() == RunnerStatus::kRunning)
      {
        SetStatus(RunnerStatus::kCompleted);
      }
      else if (GetStatus() == RunnerStatus::kStopping)
      {
        SetStatus(RunnerStatus::kStopped);
      }

      std::cout << "AAA 1.3" << std::endl;
    }
    catch (const std::exception &ex)
    {
      SetStatus(RunnerStatus::kFailed);
    }
  };
  std::cout << "AAA 2.1" << std::endl;
  m_future_result = std::async(std::launch::async, worker);
  std::cout << "AAA 2.2" << std::endl;
}

void DomainRunnerAdapterNew::PauseModeOnRequest()
{
  m_pause_mode_request = true;
  m_domain_runner->Pause();
  SetStatus(RunnerStatus::kPaused);
}

void DomainRunnerAdapterNew::PauseModeOffRequest()
{
  m_pause_mode_request = false;
  StartRequest();
}

void DomainRunnerAdapterNew::StepRequest()
{
  if (m_future_result.valid())
  {
    std::runtime_error("DomainRunnerAdapterNew::StartRequest() -> Already running job");
  }

  // deliberately before thread start
  SetStatus(RunnerStatus::kRunning);

  auto worker = [this]()
  {
    try
    {
      m_domain_runner->ExecuteSingle();

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

    }
    catch (const std::exception &ex)
    {
      SetStatus(RunnerStatus::kFailed);
    }
  };
  m_future_result = std::async(std::launch::async, worker);
}

void DomainRunnerAdapterNew::StopRequest()
{
  SetStatus(RunnerStatus::kStopping);
  m_domain_runner->Halt();
}

void DomainRunnerAdapterNew::OnStatusChange(RunnerStatus status)
{
  m_status_changed_callback(status);
}

bool DomainRunnerAdapterNew::Step()
{
  bool is_valid_request{false};
  if (CanReleaseJob(GetStatus()))
  {
    StepRequest();
    is_valid_request = true;
  }
  else if (CanStartJob(GetStatus()))
  {
    StepRequest();
    //    PauseModeOnRequest();
    //    StepRequest();
    //    StartRequest();
    is_valid_request = true;
  }
  return is_valid_request;

}

}  // namespace sequencergui
