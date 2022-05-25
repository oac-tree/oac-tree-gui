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

#include "sequencergui/jobsystem/functionrunnerv2.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/monitor/flowcontroller.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>

namespace sequencergui
{

struct FunctionRunnerV2::FunctionRunnerImpl
{
  AbstractJobV2* m_self{nullptr};
  std::mutex m_mutex;
  std::thread m_runner_thread;
  RunnerStatus m_runner_status{RunnerStatus::kIdle};
  std::function<bool()> m_worker;
  std::function<void(RunnerStatus)> m_status_changed_callback;
  std::atomic<bool> m_halt_request{false};
  FlowController m_flow_controller;

  explicit FunctionRunnerImpl(AbstractJobV2* self, std::function<bool()> worker,
                              std::function<void(RunnerStatus)> status_changed_callback)
      : m_self(self)
      , m_worker(std::move(worker))
      , m_status_changed_callback(std::move(status_changed_callback))
  {
  }

  bool IsBusy() const
  {
    return m_runner_status == RunnerStatus::kRunning || m_runner_status == RunnerStatus::kStopping
           || m_runner_status == RunnerStatus::kPaused;
  }

  void Launch()
  {
    std::cout << "aaaa 1.0 " << std::endl;
    m_halt_request.store(false);

    while (!m_halt_request.load())
    {
      std::cout << "aaaa 1.1 " << std::endl;
      if (!m_worker())
      {
        break;
      }
      std::cout << "aaaa 1.1a " << std::endl;

      if (m_flow_controller.IsPaused())
      {
        m_self->SetStatus(RunnerStatus::kPaused);
      }

      m_flow_controller.WaitIfNecessary();

      if (m_flow_controller.IsPaused())
      {
        // after the release, we have to switch back to running state
        m_self->SetStatus(RunnerStatus::kRunning);
      }

      std::cout << "aaaa 1.1b " << std::endl;
    }
    std::cout << "aaaa 1.2 " << std::endl;

    m_self->SetStatus(m_halt_request.load() ? RunnerStatus::kStopped : RunnerStatus::kCompleted);
  }

  void Stop()
  {
    m_self->SetStatus(RunnerStatus::kStopping);
    Shutdown();
    m_self->SetStatus(RunnerStatus::kStopped);
  }

  //! Shutdown working thread.
  void Shutdown()
  {
    m_halt_request.store(true);     // request to quit from event loop
    m_flow_controller.Interrupt();  // interrupt waiting if in step mode
    if (m_runner_thread.joinable())
    {
      m_runner_thread.join();
    }
  }
};

FunctionRunnerV2::FunctionRunnerV2(std::function<bool()> worker,
                                   std::function<void(RunnerStatus)> status_changed_callback)
    : p_impl(std::make_unique<FunctionRunnerImpl>(this, std::move(worker),
                                                  std::move(status_changed_callback)))
{
}

FunctionRunnerV2::~FunctionRunnerV2()
{
  p_impl->Shutdown();
}

bool FunctionRunnerV2::IsBusy() const
{
  return p_impl->IsBusy();
}

void FunctionRunnerV2::StartRequest()
{
  if (p_impl->m_runner_thread.joinable())
  {
    p_impl->m_runner_thread.join();
  }

  SetStatus(RunnerStatus::kRunning);
  p_impl->m_runner_thread = std::thread([this]() { p_impl->Launch(); });
}

void FunctionRunnerV2::PauseModeOnRequest()
{
  p_impl->m_flow_controller.SetWaitingMode(WaitingMode::kWaitForRelease);
}

void FunctionRunnerV2::PauseModeOffRequest()
{
  p_impl->m_flow_controller.SetWaitingMode(WaitingMode::kProceed);
}

void FunctionRunnerV2::StepRequest()
{
  p_impl->m_flow_controller.StepRequest();
}

void FunctionRunnerV2::StopRequest()
{
  p_impl->Stop();
}

void FunctionRunnerV2::OnStatusChange(RunnerStatus status)
{
  if (p_impl->m_status_changed_callback)
  {
    p_impl->m_status_changed_callback(status);
  }
}

bool WaitForCompletion(const FunctionRunnerV2& runner, double timeout_sec)
{
  const int timeout_precision_msec(10);
  auto timeout =
      std::chrono::system_clock::now() + std::chrono::nanoseconds(std::lround(timeout_sec * 1e9));
  while (std::chrono::system_clock::now() < timeout)
  {
    if (!runner.IsBusy())
    {
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_precision_msec));
  }
  return false;
}

}  // namespace sequencergui
