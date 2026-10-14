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
  return mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute);
}

bool IsCollapsed(const InstructionItem& item)
{
  return mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute)
             ? item.Property<bool>(domainconstants::kShowCollapsedAttribute)
             : false;
}

void SetCollapsed(bool value, InstructionItem& item)
{
  if (IsCollapsible(item))
  {
    (void)item.SetProperty(domainconstants::kShowCollapsedAttribute, value);
  }
}

void ToggleCollapsed(InstructionItem& item)
{
  if (IsCollapsible(item))
  {
    const bool current_value = item.Property<bool>(domainconstants::kShowCollapsedAttribute);
    (void)item.SetProperty(domainconstants::kShowCollapsedAttribute, !current_value);
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
  return item.FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
}

sup::gui::AnyValueItem* GetAnyValueItem(InstructionItem& item)
{
  return item.FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
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

const mvvm::SessionItem* GetIsRootItem(const mvvm::SessionItem& parent)
{
  // FindItem is total, it returns nullptr when the tag is not registered
  return parent.FindItem(domainconstants::kIsRootAttribute);
}

mvvm::SessionItem* GetIsRootItem(mvvm::SessionItem& parent)
{
  return parent.FindItem(domainconstants::kIsRootAttribute);
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
