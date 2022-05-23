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

#include <atomic>
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
  std::atomic<bool> m_halt_request;
  FlowController m_flow_controller;

  explicit FunctionRunnerImpl(std::function<bool()> worker) : m_worker(std::move(worker)) {}

  void SetRunnerStatus(RunnerStatus value)
  {
    std::lock_guard lock(m_mutex);
    m_runner_status = value;
    // FIXME emit is necessary here
  }

  bool IsBusy() const
  {
    return m_runner_status == RunnerStatus::kRunning || m_runner_status == RunnerStatus::kCanceling
           || m_runner_status == RunnerStatus::kPaused;
  }

  void Launch()
  {
    m_halt_request.store(false);

    while (!m_halt_request.load())
    {
      m_halt_request.store(m_worker());
      m_flow_controller.WaitIfNecessary();
    }
  }

  void Stop()
  {
    SetRunnerStatus(RunnerStatus::kCanceling);
    m_halt_request.store(true);
    m_flow_controller.Interrupt();  // to prevent possible waiting on step request
    if (m_runner_thread.joinable())
    {
      m_runner_thread.join();
    }
    SetRunnerStatus(RunnerStatus::kStopped);
  }
};

FunctionRunner::FunctionRunner(std::function<bool()> worker)
    : p_impl(std::make_unique<FunctionRunnerImpl>(std::move(worker)))
{
}

FunctionRunner::~FunctionRunner() = default;

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

}  // namespace sequencergui
