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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_

#include <sequencergui/jobsystem/domain_events.h>

#include <sup/sequencer/job_state_monitor.h>

#include <condition_variable>
#include <functional>
#include <mutex>

namespace sequencergui
{

/**
 * @brief The DomainJobObserver class listens for state changes in the domain JobController and
 * reports them to the event queue.
 */
class DomainJobObserver : public sup::sequencer::JobStateMonitor
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  /**
   * @brief The main c-tor.
   *
   * @param post_event_callback A callback to report events to the GUI.
   */
  explicit DomainJobObserver(post_event_callback_t post_event_callback);

  void OnStateChange(sup::sequencer::JobState state) noexcept override;

  void OnBreakpointChange(const sup::sequencer::Instruction* instruction,
                          bool breakpoint_set) noexcept override;

  void OnProcedureTick(const sup::sequencer::Procedure& proc) noexcept override;

  sup::sequencer::JobState GetCurrentState() const;

  sup::sequencer::JobState WaitForFinished() const;

  void SetTickTimeout(int msec);

private:
  post_event_callback_t m_post_event_callback;
  sup::sequencer::JobState m_state;
  mutable std::mutex m_mutex;
  mutable std::condition_variable m_cv;
  int m_tick_timeout_msec{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
