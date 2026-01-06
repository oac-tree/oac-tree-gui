/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_DISPATCHER_H_
#define OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_DISPATCHER_H_

#include <oac_tree_gui/jobsystem/domain_event_dispatcher_context.h>
#include <oac_tree_gui/jobsystem/domain_events.h>

#include <QObject>

namespace oac_tree_gui
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
                                 DomainEventDispatcherContext context,
                                 QObject* parent_object = nullptr);

  /**
   * @brief Processes new event by calling get_event callback.
   */
  void OnNewEvent();

  void operator()(const std::monostate& event) const;
  void operator()(const InstructionStateUpdatedEvent& event) const;
  void operator()(const VariableUpdatedEvent& event) const;
  void operator()(const JobStateChangedEvent& event) const;
  void operator()(const LogEvent& event) const;
  void operator()(const ActiveInstructionChangedEvent& event) const;
  void operator()(const BreakpointHitEvent& event) const;

private:
  get_event_callback_t m_get_event;
  DomainEventDispatcherContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_DISPATCHER_H_
