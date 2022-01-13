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

#include "sequencergui/model/instructionitem.h"

#include "Instruction.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/item_constants.h"

#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/utils/stringutils.h"

namespace sequencergui
{
// ----------------------------------------------------------------------------
// InstructionItem
// ----------------------------------------------------------------------------

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(ItemConstants::kName, std::string())->SetDisplayName("Name");
  AddProperty(ItemConstants::kIsRoot, false)->SetDisplayName("IsRoot");
  AddProperty(ItemConstants::kStatus, std::string())->SetDisplayName("Status");
  AddProperty(ItemConstants::kXpos, 0.0)->SetDisplayName("X");
  AddProperty(ItemConstants::kYpos, 0.0)->SetDisplayName("Y");
}

void InstructionItem::InitFromDomain(const instruction_t *instruction)
{
  // Initialise from common attributes (which exist in every Instruction).
  if (GetType() != UnknownInstructionItem::Type)  // UnknownInstructionItem has own implementation
  {
    if (instruction->GetType() != GetDomainType())
    {
      throw std::runtime_error("Error in InstructionItem: domain instruction doesn't match.");
    }

    if (instruction->HasAttribute(DomainConstants::kNameAttribute))
    {
      SetProperty(ItemConstants::kName, instruction->GetAttribute(DomainConstants::kNameAttribute));
    }
  }

  SetIsRootFlag(DomainUtils::IsRootInstruction(instruction));

  InitFromDomainImpl(instruction);
}

std::unique_ptr<instruction_t> InstructionItem::CreateDomainInstruction() const
{
  auto result = DomainUtils::CreateDomainInstruction(GetDomainType());

  // Set common attributes (that exist in every instruction)
  if (GetType() != UnknownInstructionItem::Type)  // UnknownInstructionItem has own implementation
  {
    result->AddAttribute(DomainConstants::kNameAttribute, Property<std::string>(ItemConstants::kName));
  }
  result->AddAttribute(DomainConstants::kIsRootAttribute,
                       ModelView::Utils::FromBool(Property<bool>(ItemConstants::kIsRoot)));
  SetupDomainImpl(result.get());
  return result;
}

//! Returns all children instruction, if exist.

std::vector<InstructionItem *> InstructionItem::GetInstructions() const
{
  return GetTaggedItems()->HasTag(ItemConstants::kChildInstructions)
             ? GetItems<InstructionItem>(ItemConstants::kChildInstructions)
             : std::vector<InstructionItem *>();
}

bool InstructionItem::IsRoot() const
{
  return Property<bool>(ItemConstants::kIsRoot);
}

void InstructionItem::SetIsRootFlag(bool value)
{
  SetProperty(ItemConstants::kIsRoot, value);
}

std::string InstructionItem::GetName() const
{
  return Property<std::string>(ItemConstants::kName);
}

void InstructionItem::SetName(const std::string &name)
{
  SetProperty(ItemConstants::kName, name);
}

void InstructionItem::SetStatus(const std::string &status)
{
  SetProperty(ItemConstants::kStatus, status);
}

std::string InstructionItem::GetStatus() const
{
  return Property<std::string>(ItemConstants::kStatus);
}

double InstructionItem::GetX() const
{
  return Property<double>(ItemConstants::kXpos);
}

void InstructionItem::SetX(double value)
{
  SetProperty(ItemConstants::kXpos, value);
}

double InstructionItem::GetY() const
{
  return Property<double>(ItemConstants::kYpos);
}

void InstructionItem::SetY(double value)
{
  SetProperty(ItemConstants::kYpos, value);
}

ModelView::SessionItem *InstructionItem::GetStatusItem() const
{
  return GetItem(ItemConstants::kStatus);
}

ModelView::SessionItem *InstructionItem::GetNameItem() const
{
  return GetItem(ItemConstants::kName);
}

}  // namespace sequi
