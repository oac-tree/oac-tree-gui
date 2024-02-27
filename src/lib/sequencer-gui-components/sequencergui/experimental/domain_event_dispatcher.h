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

#ifndef SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_H_
#define SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_H_

#include <sequencergui/experimental/domain_event_dispatcher_context.h>
#include <sequencergui/experimental/domain_events.h>

#include <QObject>

namespace sequencergui::experimental
{

/**
 * @brief The DomainEventDispatcher class processes domain events and calls different callbacks
 * depending on event type.
 */
class DomainEventDispatcher : public QObject
{
  Q_OBJECT

public:
  explicit DomainEventDispatcher(DomainEventDispatcherContext context, QObject* parent = nullptr);

  /**
   * @brief Processes new event by calling get_event callback.
   */
  void OnNewEvent();

  void operator()(const std::monostate& event) const;
  void operator()(const InstructionStatusChanged& event) const;
  void operator()(const JobStatusChanged& event) const;

private:
  DomainEventDispatcherContext m_context;
};

}  // namespace sequencergui::experimental

#endif  // SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_H_
