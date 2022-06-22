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

#include "sequencergui/model/procedure_item.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/workspace_item.h>

namespace sequencergui
{

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";

ProcedureItem::ProcedureItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kName, std::string())->SetDisplayName("Name");
  AddProperty(itemconstants::kDescription, std::string())->SetDisplayName("Description");
  AddBranch<InstructionContainerItem>(kInstructions)->SetDisplayName("Instructions");
  AddBranch<WorkspaceItem>(kWorkspace)->SetDisplayName("Workspace");
}

std::string ProcedureItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void ProcedureItem::SetName(const std::string &value)
{
  SetProperty(itemconstants::kName, value);
}

std::string ProcedureItem::GetDescription() const
{
  return Property<std::string>(itemconstants::kDescription);
}

void ProcedureItem::SetDescription(const std::string &value)
{
  SetProperty(itemconstants::kDescription, value);
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
