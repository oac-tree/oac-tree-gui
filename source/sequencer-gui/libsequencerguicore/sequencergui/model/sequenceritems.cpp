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

#include "sequencergui/model/sequenceritems.h"

#include "Instruction.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/item_constants.h"

#include "mvvm/model/itemutils.h"

namespace sequencergui
{
// ----------------------------------------------------------------------------
// InstructionContainerItem
// ----------------------------------------------------------------------------

InstructionContainerItem::InstructionContainerItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
              /*as_default*/ true);
}

std::vector<InstructionItem *> InstructionContainerItem::GetInstructions() const
{
  return GetItems<InstructionItem>(itemconstants::kChildInstructions);
}

// ----------------------------------------------------------------------------
// WorkspaceItem
// ----------------------------------------------------------------------------
static inline const std::string kVariableItems = "kVariableItems";

WorkspaceItem::WorkspaceItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kVariableItems), /*as_default*/ true);
}

std::vector<VariableItem *> WorkspaceItem::GetVariables() const
{
  return GetItems<VariableItem>(kVariableItems);
}

// ----------------------------------------------------------------------------
// ProcedureItem
// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------
// JobItem
// ----------------------------------------------------------------------------

JobItem::JobItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kStatus, std::string())->SetDisplayName("Status")->SetEditable(false);
}

std::string JobItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void JobItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

}  // namespace sequencergui
