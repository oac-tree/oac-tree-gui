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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_

#include <sequencergui/jobsystem/domain_events.h>

#include <functional>

namespace sequencergui
{

/**
 * @brief Provides collection of callbacks to distribute events from the queue.
 */
struct DomainEventDispatcherContext
{
  //! a callback to process a change in instruction status
  std::function<void(const InstructionStatusChangedEvent&)> process_instruction_status_changed;

  //! a callback to process a change in variable
  std::function<void(const WorkspaceEvent&)> process_workspace_event;

  //! a callback to process job status change
  std::function<void(const JobStateChangedEvent&)> process_job_state_changed;

  //! a callback to process log event
  std::function<void(const LogEvent&)> process_log_event;

  //! a callback to process next-leaves-changed event
  std::function<void(const NextLeavesChangedEvent&)> next_leaves_changed_event;

  //! a callback to process a change in automation instruction state
  std::function<void(const InstructionStateUpdatedEvent&)> process_instruction_state_updated;

  //! a callback to process a change in a variable
  std::function<void(const VariableUpdatedEvent&)> process_variable_updated;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
