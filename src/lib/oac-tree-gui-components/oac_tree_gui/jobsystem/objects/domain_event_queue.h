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

#ifndef OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_QUEUE_H_
#define OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_QUEUE_H_

#include <oac_tree_gui/jobsystem/domain_events.h>

#include <mvvm/utils/threadsafe_queue.h>

#include <QObject>

namespace oac_tree_gui
{

/**
 * @brief The DomainEventQueue class represents a thread-safe queue to store events coming from the
 * domain sequencer.
 *
 * It is expected that sequencer's JobInfoIO will post events in this queue.
 */
class DomainEventQueue : public QObject
{
  Q_OBJECT

public:
  explicit DomainEventQueue(QObject* parent_object = nullptr);

  /**
   * @brief Pops an event from the queue.
   *
   * Expected to be executed from a GUI thread. If queue is empty will return empty (monostate)
   * event.
   */
  domain_event_t PopEvent();

  /**
   * @brief Pushes event in a queue.
   *
   * Expected to be executed from sequencer thread.
   */
  void PushEvent(const domain_event_t& event);

  /**
   * @brief Returns number of events in a queue.
   */
  std::size_t GetEventCount() const;

signals:
  /**
   * @brief The signal to notify about new events in a queue. Must be connected with GUI thread via
   * queued connections.
   */
  void NewEvent();

private:
  mvvm::threadsafe_queue<domain_event_t> m_domain_events;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_OBJECTS_DOMAIN_EVENT_QUEUE_H_
