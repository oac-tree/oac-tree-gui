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

#include <string>
#include <variant>

namespace sequencergui::experimental
{

/**
 * @brief The InstructionStatusChanged struct represents an event when instruction status has
 * changed.
 */
struct InstructionStatusChanged
{
  const instruction_t* instruction{nullptr};
  std::string status;

  bool operator==(const InstructionStatusChanged& other) const;
  bool operator!=(const InstructionStatusChanged& other) const;
};

/**
 * @brief The JobStatusChanged struct represents an event when instruction status has
 * changed.
 */
struct JobStatusChanged
{
  std::string status;

  bool operator==(const JobStatusChanged& other) const;
  bool operator!=(const JobStatusChanged& other) const;
};

using domain_event_t = std::variant<std::monostate, InstructionStatusChanged, JobStatusChanged>;

bool IsValid(const domain_event_t& value);

}  // namespace sequencergui::experimental

#endif  // SEQUENCERGUI_EXPERIMENTAL_DOMAIN_EVENTS_H_
