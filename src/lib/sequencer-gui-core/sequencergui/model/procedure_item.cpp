/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_item.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/workspace_item.h>

namespace sequencergui
{

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";
static inline const std::string kPreamble = "kPreamble";
static inline const std::string kFileName = "kFileName";

ProcedureItem::ProcedureItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kName, std::string())->SetDisplayName("Name");
  AddProperty(itemconstants::kDescription, std::string())->SetDisplayName("Description");
  AddProperty(kFileName, std::string())->SetDisplayName("File name");
  AddBranch<InstructionContainerItem>(kInstructions)->SetDisplayName("Instructions");
  AddBranch<WorkspaceItem>(kWorkspace)->SetDisplayName("Workspace");
  AddBranch<WorkspaceItem>(kPreamble)->SetDisplayName("Preamble");
}

std::unique_ptr<mvvm::SessionItem> ProcedureItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ProcedureItem>(*this, make_unique_id);
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

std::string ProcedureItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void ProcedureItem::SetFileName(const std::string &value)
{
  SetProperty(kFileName, value);
}

InstructionContainerItem *ProcedureItem::GetInstructionContainer() const
{
  return GetItem<InstructionContainerItem>(kInstructions);
}

WorkspaceItem *ProcedureItem::GetWorkspace() const
{
  return GetItem<WorkspaceItem>(kWorkspace);
}

ProcedurePreambleItem *ProcedureItem::GetPreambleItem() const
{
  return GetItem<ProcedurePreambleItem>(kPreamble);
}

}  // namespace sequencergui
