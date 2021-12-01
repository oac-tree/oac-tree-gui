/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include "mvvm/model/itemutils.h"

namespace sequi
{
// ----------------------------------------------------------------------------
// UnknownInstructionItem
// ----------------------------------------------------------------------------
static inline const std::string kChildInstructions = "kChildInstructions";

UnknownInstructionItem::UnknownInstructionItem() : InstructionItem(Type)
{
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kChildInstructions), /*as_default*/ true);
}

std::string UnknownInstructionItem::GetDomainType() const
{
  return m_domain_name;
}

//! Initialise instruction from domain item.
//! This is temporarily implementation which is used for all instructions, yet unknown for the  GUI.

void UnknownInstructionItem::InitFromDomainImpl(const instruction_t *instruction)
{
  m_domain_name = instruction->GetType();

  SetDisplayName(instruction->GetType() + " (unknown)");

  // creating string properties for every domain attribute found
  for (auto [name, value] : DomainUtils::GetAttributes(instruction))
  {
    if (name
        != DomainConstants::kNameAttribute)  // already processed by InstructionItem::InitFromDomain
    {
      m_domain_attributes.push_back(name);
      AddProperty(name, value);
    }
  }
}

void UnknownInstructionItem::SetupDomainImpl(instruction_t *instruction) const
{
  for (auto name : m_domain_attributes)
  {
    instruction->AddAttribute(name, Property<std::string>(name));
  }
}

// ----------------------------------------------------------------------------
// InstructionContainerItem
// ----------------------------------------------------------------------------

InstructionContainerItem::InstructionContainerItem() : CompoundItem(Type)
{
  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kChildInstructions), /*as_default*/ true);
}

std::vector<InstructionItem *> InstructionContainerItem::GetInstructions() const
{
  return GetItems<InstructionItem>(kChildInstructions);
}

// ----------------------------------------------------------------------------
// ProcedureItem
// ----------------------------------------------------------------------------

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";

ProcedureItem::ProcedureItem() : CompoundItem(Type)
{
  // FIXME return simplified way of constructing properties, after kPropertyEditor Appearance flag
  // is there
  //  AddProperty<InstructionContainerItem>(kInstructions)->SetDisplayName("Instructions");
  //  AddProperty<WorkspaceItem>(kWorkspace)->SetDisplayName("Workspace");

  // we create it via universal tag to let recursive iteration via childrenstrategy in
  // TopItemsTreeView
  RegisterTag(
      ModelView::TagInfo::CreateUniversalTag(kInstructions, {InstructionContainerItem::Type}));
  auto instructions = InsertItem<InstructionContainerItem>({kInstructions, 0});
  instructions->SetDisplayName("Instructions");

  RegisterTag(ModelView::TagInfo::CreateUniversalTag(kWorkspace, {WorkspaceItem::Type}));
  auto workspace = InsertItem<WorkspaceItem>({kWorkspace, 0});
  workspace->SetDisplayName("Workspace");
}

InstructionContainerItem *ProcedureItem::GetInstructionContainer() const
{
  return GetItem<InstructionContainerItem>(kInstructions);
}

WorkspaceItem *ProcedureItem::GetWorkspace() const
{
  return GetItem<WorkspaceItem>(kWorkspace);
}

}  // namespace sequi
