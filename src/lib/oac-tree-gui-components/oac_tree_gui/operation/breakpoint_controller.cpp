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

#include "breakpoint_controller.h"

#include "breakpoint_helper.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>

namespace oac_tree_gui
{

BreakpointController::BreakpointController() = default;

void BreakpointController::SaveBreakpoints(const ProcedureItem &procedure_item)
{
  m_breakpoints = CollectBreakpointInfo(*procedure_item.GetInstructionContainer());
}

void BreakpointController::RestoreBreakpoints(ProcedureItem &procedure_item)
{
  SetBreakpointsFromInfo(m_breakpoints, *procedure_item.GetInstructionContainer());
}

void BreakpointController::SetAsActiveBreakpoint(InstructionItem *instruction)
{
  ResetCurrentActiveBreakpoint();

  m_active_breakpoint_instruction = instruction;
  SetBreakpointStatus(*instruction, BreakpointStatus::kSetAndHit);
}

void BreakpointController::ResetCurrentActiveBreakpoint()
{
  if (m_active_breakpoint_instruction)
  {
    // Current simplified logic is that if the breakpoint was Hit, it means it was Set before.
    // Let's return it to Set
    SetBreakpointStatus(*m_active_breakpoint_instruction, BreakpointStatus::kSet);
    m_active_breakpoint_instruction = nullptr;
  }
}

}  // namespace oac_tree_gui
