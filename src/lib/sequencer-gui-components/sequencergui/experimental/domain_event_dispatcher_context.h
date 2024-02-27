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

#ifndef SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
#define SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_

#include <sequencergui/experimental/domain_events.h>

#include <functional>

namespace sequencergui::experimental
{

/**
 * @brief Provides collection of callbacks to distribute events from the queue.
 */
struct DomainEventDispatcherContext
{
  //! a callback to retrieve domain event from a queue
  std::function<domain_event_t()> get_event;

  //! a callback to process a change in instruction status
  std::function<void(const InstructionStatusChanged&)> process_instruction_status_changed;

  //! a callback to process job status change
  std::function<void(const JobStatusChanged&)> process_job_status_changed;
};

}  // namespace sequencergui::experimental

#endif  // SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
