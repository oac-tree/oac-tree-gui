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

#include "sequencergui/jobsystem/functionrunner.h"

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

struct FunctionRunner::FunctionRunnerImpl
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
      m_flow_controller.WaitIfNecessary();
    }
    std::cout << "aaaa 1.2 " << std::endl;
    SetRunnerStatus(m_halt_request.load() ? RunnerStatus::kStopped : RunnerStatus::kCompleted);
  }

  void Stop()
  {
    SetRunnerStatus(RunnerStatus::kStopping);
    Shutdown();
  }

  void Shutdown()
  {
    m_halt_request.store(true);
    m_flow_controller.Interrupt();  // to prevent possible waiting on step request
    if (m_runner_thread.joinable())
    {
      m_runner_thread.join();
    }
  }
};

FunctionRunner::FunctionRunner(std::function<bool()> worker,
                               std::function<void(RunnerStatus)> status_changed_callback)
    : p_impl(
        std::make_unique<FunctionRunnerImpl>(std::move(worker), std::move(status_changed_callback)))
{
}

FunctionRunner::~FunctionRunner()
{
  p_impl->Shutdown();
}

bool FunctionRunner::Start()
{
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

bool FunctionRunner::Stop()
{
  p_impl->Stop();
  return true;
}

bool FunctionRunner::Pause()
{
  p_impl->m_flow_controller.SetWaitingMode(WaitingMode::kWaitForRelease);
  return true;
}

bool FunctionRunner::Step()
{
  p_impl->m_flow_controller.StepRequest();
  return true;
}

bool FunctionRunner::IsInTransition() const
{
  return false;
}

RunnerStatus FunctionRunner::GetRunnerStatus() const
{
  return p_impl->m_runner_status;
}

bool FunctionRunner::IsBusy() const
{
  return p_impl->IsBusy();
}

bool WaitForCompletion(const FunctionRunner &runner, double timeout_sec)
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
