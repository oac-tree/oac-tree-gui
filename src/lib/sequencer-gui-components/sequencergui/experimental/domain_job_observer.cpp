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
    std::lock_guard<std::mutex> lk{m_mutex};
    m_state = state;
    m_post_event_callback(JobStateChanged{state});
  }
  m_cv.notify_one();
}

void DomainJobObserver::OnBreakpointChange(const sup::sequencer::Instruction *instruction,
                                           bool breakpoint_set) noexcept
{
  (void)instruction;
  (void)breakpoint_set;
}

sup::sequencer::JobState DomainJobObserver::GetCurrentState() const
{
  std::lock_guard<std::mutex> lk{m_mutex};
  return m_state;
}

sup::sequencer::JobState DomainJobObserver::WaitForFinished() const
{
  auto pred = [this]() { return sup::sequencer::IsFinishedJobState(m_state); };
  std::unique_lock<std::mutex> lk{m_mutex};
  m_cv.wait(lk, pred);
  return m_state;
}

}  // namespace sequencergui
