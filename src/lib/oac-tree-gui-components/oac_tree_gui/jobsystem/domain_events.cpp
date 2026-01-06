/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "domain_event_helper.h"

namespace oac_tree_gui
{

bool operator==(const JobStateChangedEvent& lhs, const JobStateChangedEvent& rhs)
{
  return lhs.state == rhs.state;
}

bool operator!=(const JobStateChangedEvent& lhs, const JobStateChangedEvent& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const ActiveInstructionChangedEvent& lhs, const ActiveInstructionChangedEvent& rhs)
{
  return lhs.instr_idx == rhs.instr_idx;
}

bool operator!=(const ActiveInstructionChangedEvent& lhs, const ActiveInstructionChangedEvent& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const InstructionStateUpdatedEvent& lhs, const InstructionStateUpdatedEvent& rhs)
{
  return (lhs.index == rhs.index) && (lhs.state.m_breakpoint_set == rhs.state.m_breakpoint_set)
         && (lhs.state.m_execution_status == rhs.state.m_execution_status);
}

bool operator!=(const InstructionStateUpdatedEvent& lhs, const InstructionStateUpdatedEvent& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const VariableUpdatedEvent& lhs, const VariableUpdatedEvent& rhs)
{
  return (lhs.index == rhs.index) && (lhs.value == rhs.value) && (lhs.connected == rhs.connected);
}

bool operator!=(const VariableUpdatedEvent& lhs, const VariableUpdatedEvent& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const BreakpointHitEvent& lhs, const BreakpointHitEvent& rhs)
{
  return lhs.index == rhs.index;
}

bool operator!=(const BreakpointHitEvent& lhs, const BreakpointHitEvent& rhs)
{
  return !(lhs == rhs);
}

bool IsValid(const domain_event_t& value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

std::string ToString(const domain_event_t& value)
{
  return std::visit(DomainEventToStringVisitor{}, value);
}

}  // namespace oac_tree_gui
