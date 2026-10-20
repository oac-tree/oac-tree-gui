/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "instruction_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/iterate_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

namespace oac_tree_gui
{

namespace
{

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{
      domainconstants::kForceSuccessInstructionType, domainconstants::kIncludeInstructionType,
      domainconstants::kListenInstructionType, domainconstants::kRepeatInstructionType,
      domainconstants::kInverterInstructionType};
  return result;
}

}  // namespace

InstructionItem* InsertInstruction(const std::string& domain_type, mvvm::SessionItem* parent,
                                   const mvvm::TagIndex& tag_index)
{
  return dynamic_cast<InstructionItem*>(
      parent->InsertItem(CreateInstructionItem(domain_type), tag_index));
}

bool IsCollapsible(const InstructionItem& item)
{
  const auto* attributes = item.GetAttributes();
  return (attributes != nullptr)
         && mvvm::utils::HasTag(*attributes, domainconstants::kShowCollapsedAttribute);
}

bool IsCollapsed(const InstructionItem& item)
{
  if (!IsCollapsible(item))
  {
    return false;
  }
  const auto* property = item.GetAttributes()->FindItem(domainconstants::kShowCollapsedAttribute);
  if (property->HasData())
  {
    return property->Data<bool>();
  }

  // not set: include instructions are collapsed
  static const std::vector<std::string> kCollapsedByDefault = {
      domainconstants::kIncludeInstructionType, domainconstants::kIncludeProcedureInstructionType};
  return mvvm::utils::Contains(kCollapsedByDefault, item.GetDomainType());
}

void SetCollapsed(bool value, InstructionItem& item)
{
  if (IsCollapsible(item))
  {
    (void)item.GetAttributes()->SetProperty(domainconstants::kShowCollapsedAttribute, value);
  }
}

void ToggleCollapsed(InstructionItem& item)
{
  if (IsCollapsible(item))
  {
    SetCollapsed(!IsCollapsed(item), item);
  }
}

std::vector<const InstructionItem*> GetCollapsedItems(const InstructionContainerItem& container)
{
  std::vector<const InstructionItem*> result;

  auto func = [&result](const InstructionItem* item)
  {
    if (IsCollapsed(*item))
    {
      result.push_back(item);
    }
  };

  IterateInstructionContainer<const InstructionItem*>(container.GetInstructions(), func);

  return result;
}

const sup::gui::AnyValueItem* GetAnyValueItem(const InstructionItem& item)
{
  // FindItem is total, it returns nullptr when the tag is not registered
  const auto* attributes = item.GetAttributes();
  return (attributes != nullptr)
             ? attributes->FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag)
             : nullptr;
}

sup::gui::AnyValueItem* GetAnyValueItem(InstructionItem& item)
{
  auto* attributes = item.GetAttributes();
  return (attributes != nullptr)
             ? attributes->FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag)
             : nullptr;
}

bool IsCompoundInstruction(const InstructionItem* instruction)
{
  return instruction->GetTaggedItems()->HasTag(itemconstants::kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem* instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return mvvm::utils::Contains(domain_names, instruction->GetDomainType());
}

const mvvm::SessionItem* GetIsRootItem(const InstructionItem& item)
{
  // FindItem is total, it returns nullptr when the tag is not registered
  const auto* attributes = item.GetAttributes();
  return (attributes != nullptr) ? attributes->FindItem(domainconstants::kIsRootAttribute)
                                 : nullptr;
}

mvvm::SessionItem* GetIsRootItem(InstructionItem& item)
{
  auto* attributes = item.GetAttributes();
  return (attributes != nullptr) ? attributes->FindItem(domainconstants::kIsRootAttribute)
                                 : nullptr;
}

const InstructionItem* GetInstructionFromProperty(const mvvm::SessionItem& property)
{
  return mvvm::utils::FindItemUp<InstructionItem>(property.GetParent());
}

InstructionItem* GetInstructionFromProperty(mvvm::SessionItem& property)
{
  return mvvm::utils::FindItemUp<InstructionItem>(property.GetParent());
}

bool IsPotentialRootInstruction(const InstructionItem& instruction)
{
  if (auto parent = instruction.GetParent(); parent != nullptr)
  {
    if (parent->GetType() == mvvm::GetTypeName<InstructionContainerItem>())
    {
      return true;
    }
  }
  return false;
}

}  // namespace oac_tree_gui
