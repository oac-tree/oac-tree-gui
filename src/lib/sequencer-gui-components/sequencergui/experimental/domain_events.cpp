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

#include "domain_events.h"

namespace sequencergui::experimental
{

// InstructionStatusChanged

bool InstructionStatusChanged::operator==(const InstructionStatusChanged &other) const
{
  return instruction == other.instruction && status == other.status;
}

bool InstructionStatusChanged::operator!=(const InstructionStatusChanged &other) const
{
  return !(*this == other);
}

// JobStatusChanged

bool JobStatusChanged::operator==(const JobStatusChanged &other) const
{
  return status == other.status;
}

bool JobStatusChanged::operator!=(const JobStatusChanged &other) const
{
  return !(*this == other);
}

bool IsValid(const domain_event_t &value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

}  // namespace sequencergui::experimental
