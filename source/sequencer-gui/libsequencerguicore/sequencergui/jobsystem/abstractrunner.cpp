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

#include "sequencergui/jobsystem/abstractrunner.h"

#include "sequencergui/core/exceptions.h"

#include <mutex>
#include <thread>

namespace sequencergui
{

struct AbstractRunner::AbstractRunnerImpl
{
  std::mutex m_mutex;
  std::thread m_runner_thread;
  RunnerStatus m_runner_status{RunnerStatus::kIdle};
  std::function<bool()> m_worker;

  AbstractRunnerImpl(std::function<bool()> worker) : m_worker(std::move(worker)) {}

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
};

AbstractRunner::AbstractRunner(std::function<bool()> worker)
    : p_impl(std::make_unique<AbstractRunnerImpl>(std::move(worker)))
{
}

AbstractRunner::~AbstractRunner() = default;

bool AbstractRunner::Start()
{
  if (!p_impl->m_worker)
  {
    throw InvalidOperationException("Worker is not defined");
  }

  if (p_impl->m_runner_thread.joinable())
  {
    p_impl->m_runner_thread.join();
  }

  p_impl->SetRunnerStatus(RunnerStatus::kRunning);

  return true;
}

bool AbstractRunner::Stop()
{
  return true;
}

bool AbstractRunner::Pause()
{
  return true;
}

bool AbstractRunner::Step()
{
  return true;
}

bool AbstractRunner::IsInTransition() const
{
  return false;
}

RunnerStatus AbstractRunner::GetRunnerStatus() const
{
  return p_impl->m_runner_status;
}

}  // namespace sequencergui
