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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_event_dispatcher.h"

#include <oac-tree-gui/core/exceptions.h>

namespace sequencergui
{

DomainEventDispatcher::DomainEventDispatcher(get_event_callback_t get_event_callback,
                                             DomainEventDispatcherContext context,
                                             QObject *parent_object)
    : QObject(parent_object)
    , m_get_event(std::move(get_event_callback))
    , m_context(std::move(context))
{
  if (!m_get_event)
  {
    throw RuntimeException("Callback to get event is not initialised");
  }
}

void DomainEventDispatcher::OnNewEvent()
{
  auto event = m_get_event();
  std::visit(*this, event);
}

void DomainEventDispatcher::operator()(const std::monostate &event) const
{
  (void)event;
}

void DomainEventDispatcher::operator()(const InstructionStateUpdatedEvent &event) const
{
  if (m_context.process_instruction_state_updated)
  {
    m_context.process_instruction_state_updated(event);
  }
}

void DomainEventDispatcher::operator()(const VariableUpdatedEvent &event) const
{
  if (m_context.process_variable_updated)
  {
    m_context.process_variable_updated(event);
  }
}

void DomainEventDispatcher::operator()(const JobStateChangedEvent &event) const
{
  if (m_context.process_job_state_changed)
  {
    m_context.process_job_state_changed(event);
  }
}

void DomainEventDispatcher::operator()(const LogEvent &event) const
{
  if (m_context.process_log_event)
  {
    m_context.process_log_event(event);
  }
}

void DomainEventDispatcher::operator()(const NextLeavesChangedEvent &event) const
{
  if (m_context.next_leaves_changed_event)
  {
    m_context.next_leaves_changed_event(event);
  }
}

}  // namespace sequencergui
