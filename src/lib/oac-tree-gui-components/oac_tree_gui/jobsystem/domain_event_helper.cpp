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

#include "domain_event_helper.h"

#include <sup/gui/model/anyvalue_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/oac-tree/instruction.h>

#include <sstream>

namespace oac_tree_gui
{

std::string DomainEventToStringVisitor::operator()(const std::monostate &event) const
{
  (void)event;
  return {"monostate"};
}

std::string DomainEventToStringVisitor::operator()(const InstructionStateUpdatedEvent &event) const
{
  std::ostringstream ostr;
  ostr << "InstructionStateUpdated"
       << " " << (event.index) << " "
       << ::sup::oac_tree::StatusToString(event.state.m_execution_status);
  return ostr.str();
}

std::string DomainEventToStringVisitor::operator()(const VariableUpdatedEvent &event) const
{
  std::ostringstream ostr;
  ostr << std::string("VariableUpdatedEvent") << " " << ::sup::dto::PrintAnyValue(event.value);
  ostr << " connected:" << event.connected;
  return ostr.str();
}

std::string DomainEventToStringVisitor::operator()(const LogEvent &event) const
{
  std::ostringstream ostr;
  ostr << std::string("LogEvent") << " " << ::oac_tree_gui::ToString(event.severity);
  ostr << " " << event.message;
  return ostr.str();
}

std::string DomainEventToStringVisitor::operator()(const JobStateChangedEvent &event) const
{
  return std::string("JobStateChanged") + " " + ::sup::oac_tree::ToString(event.state);
}

std::string DomainEventToStringVisitor::operator()(const ActiveInstructionChangedEvent &event) const
{
  std::ostringstream ostr;
  ostr << std::string("ActiveInstructionChanged") << " size: " << event.instr_idx.size()
       << " indx:";
  for (auto instr_index : event.instr_idx)
  {
    ostr << instr_index << " ";
  }
  return ostr.str();
}

}  // namespace oac_tree_gui
