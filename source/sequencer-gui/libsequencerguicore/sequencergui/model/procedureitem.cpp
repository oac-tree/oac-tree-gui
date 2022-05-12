/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/procedureitem.h"

#include "sequencergui/model/instructioncontaineritem.h"
#include "sequencergui/model/item_constants.h"
#include "sequencergui/model/workspaceitem.h"

namespace sequencergui
{

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";

ProcedureItem::ProcedureItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kStatus, std::string())->SetDisplayName("Status")->SetEditable(false);
  AddBranch<InstructionContainerItem>(kInstructions)->SetDisplayName("Instructions");
  AddBranch<WorkspaceItem>(kWorkspace)->SetDisplayName("Workspace");
}

std::string ProcedureItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void ProcedureItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

InstructionContainerItem *ProcedureItem::GetInstructionContainer() const
{
  return GetItem<InstructionContainerItem>(kInstructions);
}

WorkspaceItem *ProcedureItem::GetWorkspace() const
{
  return GetItem<WorkspaceItem>(kWorkspace);
}
}  // namespace sequencergui
