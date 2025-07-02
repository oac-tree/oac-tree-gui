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

#include "domain_event_helper.h"

namespace oac_tree_gui
{

bool JobStateChangedEvent::operator==(const JobStateChangedEvent &other) const
{
  return state == other.state;
}

bool JobStateChangedEvent::operator!=(const JobStateChangedEvent &other) const
{
  return !(*this == other);
}

bool ActiveInstructionChangedEvent::operator==(const ActiveInstructionChangedEvent &other) const
{
  return instr_idx == other.instr_idx;
}

bool ActiveInstructionChangedEvent::operator!=(const ActiveInstructionChangedEvent &other) const
{
  return !(*this == other);
}

bool InstructionStateUpdatedEvent::operator==(const InstructionStateUpdatedEvent &other) const
{
  return index == other.index && state.m_breakpoint_set == other.state.m_breakpoint_set
         && state.m_execution_status == other.state.m_execution_status;
}

bool InstructionStateUpdatedEvent::operator!=(const InstructionStateUpdatedEvent &other) const
{
  return !(*this == other);
}

bool VariableUpdatedEvent::operator==(const VariableUpdatedEvent &other) const
{
  return index == other.index && value == other.value && connected == other.connected;
}

bool VariableUpdatedEvent::operator!=(const VariableUpdatedEvent &other) const
{
  return !(*this == other);
}


bool BreakpointHitEvent::operator==(const BreakpointHitEvent &other) const
{
  return index == other.index;
}

bool BreakpointHitEvent::operator!=(const BreakpointHitEvent &other) const
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

}  // namespace oac_tree_gui
