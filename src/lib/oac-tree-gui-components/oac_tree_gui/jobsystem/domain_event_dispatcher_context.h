/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#ifndef OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
#define OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_

#include <oac_tree_gui/jobsystem/domain_events.h>

#include <functional>

namespace oac_tree_gui
{

/**
 * @brief Provides collection of callbacks to distribute events from the queue.
 */
struct DomainEventDispatcherContext
{
  //! a callback to process a change in automation instruction state
  std::function<void(const InstructionStateUpdatedEvent&)> process_instruction_state_updated;

  //! a callback to process a change in a variable
  std::function<void(const VariableUpdatedEvent&)> process_variable_updated;

  //! a callback to process job status change
  std::function<void(const JobStateChangedEvent&)> process_job_state_changed;

  //! a callback to process log event
  std::function<void(const LogEvent&)> process_log_event;

  //! a callback to process change in active instruction leaves
  std::function<void(const ActiveInstructionChangedEvent&)> active_instruction_changed_event;

  //! a callback to process breakpoint hit event
  std::function<void(const BreakpointHitEvent&)> breakpoint_hit_updated;
};

}  // namespace sequencergui

#endif  // OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENT_DISPATCHER_CONTEXT_H_
