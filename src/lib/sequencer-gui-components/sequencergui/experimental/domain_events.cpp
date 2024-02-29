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

#include <sup/sequencer/instruction.h>

#include <sstream>

namespace
{

struct DomainEventToStringVisitor
{
  std::string operator()(const std::monostate &event) const { return std::string("monostate"); }

  std::string operator()(const sequencergui::experimental::InstructionStatusChanged &event) const
  {
    std::ostringstream ostr;
    const std::string name = (event.instruction ? event.instruction->GetName() : std::string());
    ostr << "InstructionStatusChanged"
         << " " << (event.instruction) << " " << name << " " << event.status;
    return ostr.str();
  }

  std::string operator()(const sequencergui::experimental::JobStatusChanged &event) const
  {
    return std::string("JobStatusChanged") + " " + event.status;
  }
};

}  // namespace

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

std::string ToString(const domain_event_t &value)
{
  return std::visit(DomainEventToStringVisitor{}, value);
}

}  // namespace sequencergui::experimental
