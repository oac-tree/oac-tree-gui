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

#include "domain_event_queue.h"

namespace sequencergui
{

DomainEventQueue::DomainEventQueue(QObject *parent) : QObject(parent) {}

domain_event_t DomainEventQueue::PopEvent()
{
  domain_event_t result;
  m_domain_events.try_pop(result);
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
