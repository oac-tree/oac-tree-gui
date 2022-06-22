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

#include "sequencergui/model/instruction_item.h"

#include "Instruction.h"
#include <sequencergui/model/domain_constants.h>
#include <sequencergui/model/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include "sequencergui/model/standard_instruction_items.h"

#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/utils/string_utils.h>

namespace sequencergui
{
// ----------------------------------------------------------------------------
// InstructionItem
// ----------------------------------------------------------------------------

InstructionItem::InstructionItem(const std::string &item_type) : CompoundItem(item_type)
{
  AddProperty(itemconstants::kName, std::string())->SetDisplayName("Name");
  AddProperty(itemconstants::kIsRoot, false)->SetDisplayName("IsRoot");
  AddProperty(itemconstants::kStatus, std::string())
      ->SetDisplayName("Status")
      ->SetVisible(false)
      ->SetEditable(false);
  AddProperty(itemconstants::kXpos, 0.0)->SetDisplayName("X")->SetVisible(false);
  AddProperty(itemconstants::kYpos, 0.0)->SetDisplayName("Y")->SetVisible(false);
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

    if (instruction->HasAttribute(domainconstants::kNameAttribute))
    {
      SetProperty(itemconstants::kName, instruction->GetAttribute(domainconstants::kNameAttribute));
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
    result->AddAttribute(domainconstants::kNameAttribute,
                         Property<std::string>(itemconstants::kName));
  }
  result->AddAttribute(domainconstants::kIsRootAttribute,
                       mvvm::utils::FromBool(Property<bool>(itemconstants::kIsRoot)));
  SetupDomainImpl(result.get());
  return result;
}

//! Returns all children instruction, if exist.

std::vector<InstructionItem *> InstructionItem::GetInstructions() const
{
  return GetTaggedItems()->HasTag(itemconstants::kChildInstructions)
             ? GetItems<InstructionItem>(itemconstants::kChildInstructions)
             : std::vector<InstructionItem *>();
}

bool InstructionItem::IsRoot() const
{
  return Property<bool>(itemconstants::kIsRoot);
}

void InstructionItem::SetIsRootFlag(bool value)
{
  SetProperty(itemconstants::kIsRoot, value);
}

std::string InstructionItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void InstructionItem::SetName(const std::string &name)
{
  SetProperty(itemconstants::kName, name);
}

std::string InstructionItem::GetStatus() const
{
  return Property<std::string>(itemconstants::kStatus);
}

void InstructionItem::SetStatus(const std::string &status)
{
  SetProperty(itemconstants::kStatus, status);
}

double InstructionItem::GetX() const
{
  return Property<double>(itemconstants::kXpos);
}

void InstructionItem::SetX(double value)
{
  SetProperty(itemconstants::kXpos, value);
}

double InstructionItem::GetY() const
{
  return Property<double>(itemconstants::kYpos);
}

void InstructionItem::SetY(double value)
{
  SetProperty(itemconstants::kYpos, value);
}

mvvm::SessionItem *InstructionItem::GetStatusItem() const
{
  return GetItem(itemconstants::kStatus);
}

mvvm::SessionItem *InstructionItem::GetNameItem() const
{
  return GetItem(itemconstants::kName);
}

}  // namespace sequencergui
