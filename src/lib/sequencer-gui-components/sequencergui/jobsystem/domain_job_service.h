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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_

#include <sequencergui/jobsystem/domain_events.h>

#include <memory>

namespace sequencergui
{

class DomainEventQueue;
class DomainEventDispatcher;
class DomainEventDispatcherContext;
class DomainJobObserver;
class UserContext;

/**
 * @brief The DomainJobService class provides thread-safe wiring between domain's JobInfoIO and GUI
 * context.
 */
class DomainJobService
{
public:
  DomainJobService(DomainEventDispatcherContext dispatcher_context, UserContext user_context);
  virtual ~DomainJobService();

  sup::sequencer::IJobInfoIO* GetJobInfoIO();

  /**
   * @brief Returns last reported job state.
   */
  sup::sequencer::JobState GetJobState() const;

  /**
   * @brief Waits for underlying job opbserver reports finished state.
   */
  sup::sequencer::JobState WaitForFinished() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::sequencer::JobState state, double msec) const;

  /**
   * @brief Sets sleep time on every tick.
   */
  void SetTickTimeout(int msec);

  /**
   * @brief Returns number of events in a queue.
   */
  size_t GetEventCount() const;

private:
  /**
   * @brief Creates a callback to publish domain events.
   */
  std::function<void(const domain_event_t& event)> CreatePostEventCallback() const;

  /**
   * @brief Creates a callback to get events from event queue.
   */
  std::function<domain_event_t()> CreateGetEventCallback() const;

  std::unique_ptr<DomainEventQueue> m_event_queue;
  std::unique_ptr<DomainEventDispatcher> m_event_dispatcher;
  std::unique_ptr<DomainJobObserver> m_job_observer;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_
