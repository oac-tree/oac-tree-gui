/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_job_observer.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/job_states.h>
#include <sup/sequencer/procedure.h>

#include <cmath>
#include <thread>

namespace
{

/**
 * @brief Checks if this is the last tick of the given procedure.
 */
bool IsLastTick(const sup::sequencer::Procedure &proc)
{
  using sup::sequencer::ExecutionStatus;
  const auto status = proc.GetStatus();
  return status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE;
}

}  // namespace

namespace sequencergui
{

DomainJobObserver::DomainJobObserver(post_event_callback_t post_event_callback)
    : m_post_event_callback(std::move(post_event_callback))
{
  if (!m_post_event_callback)
  {
    throw RuntimeException("Callback is not initialised");
  }
}

void DomainJobObserver::OnStateChange(sup::sequencer::JobState state) noexcept
{
  {
    const std::lock_guard<std::mutex> lock{m_mutex};
    m_state = state;
    m_post_event_callback(JobStateChangedEvent{state});
  }
  m_cv.notify_one();
}

void DomainJobObserver::OnBreakpointChange(const sup::sequencer::Instruction *instruction,
                                           bool breakpoint_set) noexcept
{
  (void)instruction;
  (void)breakpoint_set;
}

void DomainJobObserver::OnProcedureTick(const sup::sequencer::Procedure &proc) noexcept
{
  (void)proc;
  std::unique_lock<std::mutex> lock{m_mutex};

  m_post_event_callback(NextLeavesChangedEvent{sup::sequencer::GetNextLeaves(proc)});

  if (m_tick_timeout_msec > 0 && !IsLastTick(proc))
  {
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_msec));
  }
}

sup::sequencer::JobState DomainJobObserver::GetCurrentState() const
{
  const std::lock_guard<std::mutex> lock{m_mutex};
  return m_state;
}

bool DomainJobObserver::WaitForState(sup::sequencer::JobState state, double msec) const
{
  const double nanosec_per_msec{1e6};
  auto duration = std::chrono::nanoseconds(std::lround(msec * nanosec_per_msec));
  std::unique_lock<std::mutex> lock{m_mutex};
  return m_cv.wait_for(lock, duration, [this, state]() { return m_state == state; });
}

sup::sequencer::JobState DomainJobObserver::WaitForFinished() const
{
  auto pred = [this]() { return sup::sequencer::IsFinishedJobState(m_state); };
  std::unique_lock<std::mutex> lock{m_mutex};
  m_cv.wait(lock, pred);
  return m_state;
}

void DomainJobObserver::SetTickTimeout(int msec)
{
  const std::unique_lock<std::mutex> lock{m_mutex};
  m_tick_timeout_msec = msec;
}

}  // namespace sequencergui
