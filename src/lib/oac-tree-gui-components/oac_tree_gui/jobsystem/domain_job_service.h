/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_
#define OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_

#include <oac_tree_gui/jobsystem/domain_events.h>

#include <memory>
#include <chrono>

namespace oac_tree_gui
{

class DomainEventQueue;
class DomainEventDispatcher;
struct DomainEventDispatcherContext;
class DomainJobObserver;
struct UserContext;

/**
 * @brief The DomainJobService class provides thread-safe wiring between domain's JobInfoIO and GUI
 * context.
 */
class DomainJobService
{
public:
  DomainJobService(DomainEventDispatcherContext dispatcher_context, UserContext user_context);
  virtual ~DomainJobService();

  DomainJobService(const DomainJobService&) = delete;
  DomainJobService& operator=(const DomainJobService&) = delete;
  DomainJobService(DomainJobService&&) = delete;
  DomainJobService& operator=(DomainJobService&&) = delete;

  sup::oac_tree::IJobInfoIO* GetJobInfoIO();

  /**
   * @brief Returns last reported job state.
   */
  sup::oac_tree::JobState GetJobState() const;

  /**
   * @brief Waits for underlying job opbserver reports finished state.
   */
  sup::oac_tree::JobState WaitForFinished() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::oac_tree::JobState state, std::chrono::milliseconds duration) const;

  /**
   * @brief Sets sleep time on every tick.
   */
  void SetTickTimeout(std::chrono::milliseconds timeout);

  /**
   * @brief Returns number of events in a queue.
   */
  std::size_t GetEventCount() const;

  /**
   * @brief Sets filter to suppress active instruction notifications.
   */
  void SetInstructionActiveFilter(const active_filter_t& filter);

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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_SERVICE_H_
