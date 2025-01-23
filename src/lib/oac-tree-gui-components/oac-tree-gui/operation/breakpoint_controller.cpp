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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "breakpoint_controller.h"

#include "breakpoint_helper.h"

#include <oac-tree-gui/model/procedure_item.h>

namespace sequencergui
{

BreakpointController::BreakpointController() = default;

void BreakpointController::SaveBreakpoints(const ProcedureItem &procedure_item)
{
  m_breakpoints = std::move(CollectBreakpointInfo(*procedure_item.GetInstructionContainer()));
}

void BreakpointController::RestoreBreakpoints(ProcedureItem &procedure_item)
{
  SetBreakpointsFromInfo(m_breakpoints, *procedure_item.GetInstructionContainer());
}

}  // namespace sequencergui
