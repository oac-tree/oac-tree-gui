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

#include "domain_events.h"

#include <sup/gui/model/anyvalue_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/oac-tree/instruction.h>

#include <sstream>

namespace
{

struct DomainEventToStringVisitor
{
  std::string operator()(const std::monostate &event) const
  {
    (void)event;
    return std::string("monostate");
  }

  std::string operator()(const ::oac_tree_gui::InstructionStateUpdatedEvent &event) const
  {
    std::ostringstream ostr;
    ostr << "InstructionStateUpdated"
         << " " << (event.index) << " "
         << ::sup::oac_tree::StatusToString(event.state.m_execution_status);
    return ostr.str();
  }

  std::string operator()(const ::oac_tree_gui::VariableUpdatedEvent &event) const
  {
    std::ostringstream ostr;
    ostr << std::string("VariableUpdatedEvent") << " " << ::sup::dto::PrintAnyValue(event.value);
    ostr << " connected:" << event.connected;
    return ostr.str();
  }

  std::string operator()(const ::oac_tree_gui::JobStateChangedEvent &event) const
  {
    return std::string("JobStateChanged") + " " + ::sup::oac_tree::ToString(event.state);
  }

  std::string operator()(const ::oac_tree_gui::LogEvent &event) const
  {
    std::ostringstream ostr;
    ostr << std::string("LogEvent") << " " << ::oac_tree_gui::ToString(event.severity);
    ostr << " " << event.message;
    return ostr.str();
  }

  std::string operator()(const ::oac_tree_gui::NextLeavesChangedEvent &event) const
  {
    std::ostringstream ostr;
    ostr << std::string("NextLeavesChanged") << " size: " << event.leaves.size() << " indx:";
    for (auto instr_index : event.leaves)
    {
      ostr << instr_index << " ";
    }
    return ostr.str();
  }
};

}  // namespace

namespace oac_tree_gui
{

// JobStatusChanged

bool JobStateChangedEvent::operator==(const JobStateChangedEvent &other) const
{
  return state == other.state;
}

bool JobStateChangedEvent::operator!=(const JobStateChangedEvent &other) const
{
  return !(*this == other);
}

// NextLeavesChanged

bool NextLeavesChangedEvent::operator==(const NextLeavesChangedEvent &other) const
{
  return leaves == other.leaves;
}

bool NextLeavesChangedEvent::operator!=(const NextLeavesChangedEvent &other) const
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

// InstructionStateUpdatedEvent

bool InstructionStateUpdatedEvent::operator==(const InstructionStateUpdatedEvent &other) const
{
  return index == other.index && state.m_breakpoint_set == other.state.m_breakpoint_set
         && state.m_execution_status == other.state.m_execution_status;
}

bool InstructionStateUpdatedEvent::operator!=(const InstructionStateUpdatedEvent &other) const
{
  return !(*this == other);
}

// InstructionStateUpdatedEvent

bool VariableUpdatedEvent::operator==(const VariableUpdatedEvent &other) const
{
  return index == other.index && value == other.value && connected == other.connected;
}

bool VariableUpdatedEvent::operator!=(const VariableUpdatedEvent &other) const
{
  return !(*this == other);
}

}  // namespace oac_tree_gui
