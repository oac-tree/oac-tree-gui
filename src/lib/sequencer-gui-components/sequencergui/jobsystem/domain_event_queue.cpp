/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "domain_event_queue.h"

#include <sequencergui/core/exceptions.h>

namespace sequencergui
{

DomainEventQueue::DomainEventQueue(QObject *parent_object) : QObject(parent_object) {}

domain_event_t DomainEventQueue::PopEvent()
{
  domain_event_t result;
  if (!m_domain_events.try_pop(result))
  {
    // We should warn somehow the user without exaggerating too much. Wouldn't be already enough to
    // print to stderr?
    throw RuntimeException("Broken logic, event stack is empty");
  }
  return result;
}

void DomainEventQueue::PushEvent(const domain_event_t &event)
{
  m_domain_events.push(event);
  emit NewEvent();
}

size_t DomainEventQueue::GetEventCount() const
{
  return m_domain_events.size();
}

}  // namespace sequencergui
