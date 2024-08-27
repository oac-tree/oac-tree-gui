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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_H_

#include <sequencergui/jobsystem/domain_event_dispatcher_context.h>
#include <sequencergui/jobsystem/domain_events.h>

#include <QObject>

namespace sequencergui
{

/**
 * @brief The DomainEventDispatcher class processes domain events and calls different callbacks
 * depending on event type.
 */
class DomainEventDispatcher : public QObject
{
  Q_OBJECT

public:
  using get_event_callback_t = std::function<domain_event_t()>;
  explicit DomainEventDispatcher(get_event_callback_t get_event_callback,
                                 DomainEventDispatcherContext context, QObject* parent = nullptr);

  /**
   * @brief Processes new event by calling get_event callback.
   */
  void OnNewEvent();

  void operator()(const std::monostate& event) const;
  void operator()(const InstructionStatusChangedEvent& event) const;
  void operator()(const WorkspaceEvent& event) const;
  void operator()(const JobStateChangedEvent& event) const;
  void operator()(const LogEvent& event) const;
  void operator()(const NextLeavesChangedEvent& event) const;
  void operator()(const InstructionStateUpdatedEvent& event) const;
  void operator()(const VariableUpdatedEvent& event) const;

private:
  get_event_callback_t m_get_event;
  DomainEventDispatcherContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_H_
