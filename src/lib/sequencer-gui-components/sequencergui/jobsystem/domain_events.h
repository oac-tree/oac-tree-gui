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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENTS_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENTS_H_

//! @file
//! Contains a collection of classes representing various events happening on the domain side during
//! sequencer execution.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/log_event.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_state.h>
#include <sup/sequencer/job_states.h>

#include <string>
#include <variant>
#include <vector>

namespace sequencergui
{

/**
 * @brief The InstructionStateUpdatedEvent class represents automation server event when
 * instruction state has changed.
 */
struct InstructionStateUpdatedEvent
{
  size_t index{0};
  sup::sequencer::InstructionState state;

  bool operator==(const InstructionStateUpdatedEvent& other) const;
  bool operator!=(const InstructionStateUpdatedEvent& other) const;
};

/**
 * @brief The VariableUpdatedEvent class represents automation server event when variable value has
 * changed.
 */
struct VariableUpdatedEvent
{
  size_t index{0};
  sup::dto::AnyValue value;
  bool connected{false};

  bool operator==(const VariableUpdatedEvent& other) const;
  bool operator!=(const VariableUpdatedEvent& other) const;
};

/**
 * @brief The JobStateChangedEvent struct represents an event when instruction status has
 * changed.
 */
struct JobStateChangedEvent
{
  sup::sequencer::JobState status;

  bool operator==(const JobStateChangedEvent& other) const;
  bool operator!=(const JobStateChangedEvent& other) const;
};

struct NextLeavesChangedEvent
{
  std::vector<sup::dto::uint32> leaves;

  bool operator==(const NextLeavesChangedEvent& other) const;
  bool operator!=(const NextLeavesChangedEvent& other) const;
};

using domain_event_t =
    std::variant<std::monostate, InstructionStateUpdatedEvent, VariableUpdatedEvent,
                 JobStateChangedEvent, LogEvent, NextLeavesChangedEvent>;

bool IsValid(const domain_event_t& value);

std::string ToString(const domain_event_t& value);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_EVENTS_H_
