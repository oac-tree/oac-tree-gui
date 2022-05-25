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
  std::mutex m_mutex;
  std::thread m_runner_thread;
  RunnerStatus m_runner_status{RunnerStatus::kIdle};
  std::function<bool()> m_worker;
  std::function<void(RunnerStatus)> m_status_changed_callback;
  std::atomic<bool> m_halt_request{false};
  FlowController m_flow_controller;

  explicit FunctionRunnerImpl(std::function<bool()> worker,
                              std::function<void(RunnerStatus)> status_changed_callback)
      : m_worker(std::move(worker)), m_status_changed_callback(std::move(status_changed_callback))
  {
  }

  void SetRunnerStatus(RunnerStatus value)
  {
    std::lock_guard lock(m_mutex);

    if (value == m_runner_status)
    {
      return;
    }

    m_runner_status = value;

    std::cout << "SetRunnerStatus " << static_cast<int>(value) << std::endl;

    if (m_status_changed_callback)
    {
      m_status_changed_callback(m_runner_status);
    }
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
        SetRunnerStatus(RunnerStatus::kPaused);
      }

      m_flow_controller.WaitIfNecessary();

      if (m_flow_controller.IsPaused())
      {
        // after the release, we have to switch back to running state
        SetRunnerStatus(RunnerStatus::kRunning);
      }

      std::cout << "aaaa 1.1b " << std::endl;
    }
    std::cout << "aaaa 1.2 " << std::endl;

    SetRunnerStatus(m_halt_request.load() ? RunnerStatus::kStopped : RunnerStatus::kCompleted);
  }

  void Stop()
  {
    SetRunnerStatus(RunnerStatus::kStopping);
    Shutdown();
    SetRunnerStatus(RunnerStatus::kStopped);
  }

  //! Shutdown working thread.
  void Shutdown()
  {
    m_halt_request.store(true); // request to quit from event loop
    m_flow_controller.Interrupt();  // interrupt waiting if in step mode
    if (m_runner_thread.joinable())
    {
      m_runner_thread.join();
    }
  }
};

FunctionRunnerV2::FunctionRunnerV2(std::function<bool()> worker,
                               std::function<void(RunnerStatus)> status_changed_callback)
    : p_impl(
        std::make_unique<FunctionRunnerImpl>(std::move(worker), std::move(status_changed_callback)))
{
}

FunctionRunnerV2::~FunctionRunnerV2()
{
  p_impl->Shutdown();
}

bool FunctionRunnerV2::Start()
{
  if (GetRunnerStatus() == RunnerStatus::kPaused)
  {

  }

  if (p_impl->IsBusy())
  {
    return false;
  }

  if (!p_impl->m_worker)
  {
    throw InvalidOperationException("Worker is not defined");
  }

  if (p_impl->m_runner_thread.joinable())
  {
    p_impl->m_runner_thread.join();
  }

  p_impl->SetRunnerStatus(RunnerStatus::kRunning);
  p_impl->m_runner_thread = std::thread([this]() { p_impl->Launch(); });

  return true;
}

bool FunctionRunnerV2::Stop()
{
  p_impl->Stop();
  return true;
}

bool FunctionRunnerV2::Pause()
{
  p_impl->m_flow_controller.SetWaitingMode(WaitingMode::kWaitForRelease);
  return true;
}

bool FunctionRunnerV2::Step()
{
  p_impl->m_flow_controller.StepRequest();
  return true;
}

RunnerStatus FunctionRunnerV2::GetRunnerStatus() const
{
  return p_impl->m_runner_status;
}

bool FunctionRunnerV2::IsBusy() const
{
  return p_impl->IsBusy();
}

void FunctionRunnerV2::SetWaitingMode(WaitingMode waiting_mode)
{
  p_impl->m_flow_controller.SetWaitingMode(waiting_mode);
}

bool WaitForCompletion(const FunctionRunnerV2 &runner, double timeout_sec)
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
