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

#include "universal_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/iterate_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

namespace oac_tree_gui
{

InstructionItem* InsertInstruction(const std::string& domain_type, mvvm::SessionItem* parent,
                                   const mvvm::TagIndex& tag_index)
{
  return dynamic_cast<InstructionItem*>(
      parent->InsertItem(CreateInstructionItem(domain_type), tag_index));
}

void SetInput(const std::string& value, InstructionItem& item)
{
  (void)item.SetProperty(domainconstants::kInputVariableNameAttribute, value);
}

void SetOutput(const std::string& value, InstructionItem& item)
{
  (void)item.SetProperty(domainconstants::kOutputVariableNameAttribute, value);
}

void SetDescription(const std::string& value, InstructionItem& item)
{
  (void)item.SetProperty(domainconstants::kDescriptionAttribute, value);
}

void SetText(const std::string& value, InstructionItem& item)
{
  (void)item.SetProperty(domainconstants::kTextAttribute, value);
}

void SetVariableName(const std::string& value, InstructionItem& item)
{
  (void)item.SetProperty(domainconstants::kGenericVariableNameAttribute, value);
}

bool IsCollapsible(const InstructionItem& item)
{
  return mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute);
}

// FIXME merge with RegisterShowCollapsedProperty
void UpdateShowCollapsedPropertyAppearance(InstructionItem& item)
{
  static const std::vector<std::string> collapsed_by_default{
      domainconstants::kIncludeInstructionType, domainconstants::kIncludeProcedureInstructionType};

  if (mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute))
  {
    auto property = item.GetItem<sup::gui::AnyValueItem>(domainconstants::kShowCollapsedAttribute);
    (void)property->SetDisplayName("Show collapsed");
    (void)property->SetToolTip("Show child branch collapsed duing procedure execution");
    const auto is_collapsed_value =
        mvvm::utils::Contains(collapsed_by_default, item.GetDomainType());
    (void)property->SetData(is_collapsed_value);
    SetAttributeExposedFlag(is_collapsed_value, *property);
  }
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

sup::gui::AnyValueItem* GetAnyValueItem(const InstructionItem& item)
{
  return mvvm::utils::HasTag(item, itemconstants::kAnyValueTag)
             ? item.GetItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag)
             : nullptr;
}

void UpdateIsRootPropertyAppearance(InstructionItem& item)
{
  if (mvvm::utils::HasTag(item, domainconstants::kIsRootAttribute))
  {
    auto property = item.GetItem<sup::gui::AnyValueItem>(domainconstants::kIsRootAttribute);
    SetAttributeExposedFlag(true, *property);
  }
}

}  // namespace oac_tree_gui
