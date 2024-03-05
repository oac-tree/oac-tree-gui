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

#ifndef SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENTS_H_
#define SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENTS_H_

//! @file
//! Contains a collection of classes representing various events happening on the domain side during
//! sequencer execution.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/log_event.h>

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/job_states.h>

#include <string>
#include <variant>
#include <vector>

namespace sequencergui
{

/**
 * @brief The InstructionStatusChangedEvent struct represents an event when instruction status has
 * changed.
 */
struct InstructionStatusChangedEvent
{
  const instruction_t* instruction{nullptr};
  sup::sequencer::ExecutionStatus status;

  bool operator==(const InstructionStatusChangedEvent& other) const;
  bool operator!=(const InstructionStatusChangedEvent& other) const;
};

/**
 * @brief The JobStateChanged struct represents an event when instruction status has
 * changed.
 */
struct JobStateChanged
{
  sup::sequencer::JobState status;

  bool operator==(const JobStateChanged& other) const;
  bool operator!=(const JobStateChanged& other) const;
};

/**
 * @brief The NextLeavesChanged struct represents an event when list of next instruction leaves is
 * changed.
 */
struct NextLeavesChanged
{
  std::vector<const instruction_t*> leaves;

  bool operator==(const NextLeavesChanged& other) const;
  bool operator!=(const NextLeavesChanged& other) const;
};

using domain_event_t = std::variant<std::monostate, InstructionStatusChangedEvent, JobStateChanged,
                                    LogEvent, NextLeavesChanged>;

bool IsValid(const domain_event_t& value);

std::string ToString(const domain_event_t& value);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENTS_H_
