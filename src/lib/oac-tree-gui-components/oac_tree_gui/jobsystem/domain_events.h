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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENTS_H_
#define OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENTS_H_

//! @file
//! Contains a collection of classes representing various events happening on the domain side during
//! sequencer execution.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/jobsystem/log_event.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_state.h>
#include <sup/oac-tree/job_states.h>

#include <string>
#include <variant>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The InstructionStateUpdatedEvent class represents automation server event when
 * instruction state has changed.
 */
struct InstructionStateUpdatedEvent
{
  std::size_t index{0};
  sup::oac_tree::InstructionState state;

  bool operator==(const InstructionStateUpdatedEvent& other) const;
  bool operator!=(const InstructionStateUpdatedEvent& other) const;
};

/**
 * @brief The VariableUpdatedEvent class represents automation server event when variable value has
 * changed.
 */
struct VariableUpdatedEvent
{
  std::size_t index{0};
  sup::dto::AnyValue value;
  bool connected{false};

  bool operator==(const VariableUpdatedEvent& other) const;
  bool operator!=(const VariableUpdatedEvent& other) const;
};

/**
 * @brief The JobStateChangedEvent struct represents an event when domain job state has
 * changed.
 */
struct JobStateChangedEvent
{
  sup::oac_tree::JobState state;

  bool operator==(const JobStateChangedEvent& other) const;
  bool operator!=(const JobStateChangedEvent& other) const;
};

struct ActiveInstructionChangedEvent
{
  std::vector<sup::dto::uint32> leaves;

  bool operator==(const ActiveInstructionChangedEvent& other) const;
  bool operator!=(const ActiveInstructionChangedEvent& other) const;
};

using domain_event_t =
    std::variant<std::monostate, InstructionStateUpdatedEvent, VariableUpdatedEvent,
                 JobStateChangedEvent, LogEvent, ActiveInstructionChangedEvent>;

bool IsValid(const domain_event_t& value);

std::string ToString(const domain_event_t& value);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_DOMAIN_EVENTS_H_
