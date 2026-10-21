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

#include "instruction_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_descriptor_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>

#include <sup/oac-tree/instruction.h>

#include <stack>

namespace oac_tree_gui
{

namespace
{

//! Returns true if the domain attribute is represented by an attribute property.
bool IsAttributeProperty(const std::string& name)
{
  static const std::vector<std::string> kNonPropertyAttributes = {
      domainconstants::kNameAttribute,  // display name
      domainconstants::kTypeAttribute,  // AnyValueItem
      domainconstants::kValueAttribute  // AnyValueItem
  };
  return !mvvm::utils::Contains(kNonPropertyAttributes, name);
}

//! Returns the attribute with the given name, or nullptr.
const AttributeDescriptor* FindAttributeDescriptor(const InstructionDescriptor& descriptor,
                                                   const std::string& name)
{
  auto iter = std::find_if(descriptor.attributes.begin(), descriptor.attributes.end(),
                           [&name](const auto& attribute) { return attribute.name == name; });
  return iter == descriptor.attributes.end() ? nullptr : &*iter;
}

//! Returns the combo property with instruction behaviors.
mvvm::ComboProperty CreateBehaviorProperty()
{
  mvvm::ComboProperty result({itemconstants::kNativeBehavior, itemconstants::kHiddenBehavior,
                              itemconstants::kSucceedBehavior, itemconstants::kFailBehavior});
  result.SetToolTips({"Instruction behaves according to the implementation",
                      "Instruction will be invisible and will not be executed",
                      "Instruction will always succeed", "Instruction will always fail"});
  return result;
}

//! Adds GUI properties: behavior, status, coordinates and breakpoint.
void AddGuiProperties(InstructionItem& item)
{
  (void)item.AddProperty(itemconstants::kBehaviorTag, CreateBehaviorProperty())
      .SetToolTip("Modifier that allows to change the behavior");
  (void)item.AddProperty(itemconstants::kStatus, std::string())
      .SetDisplayName("Status")
      .SetVisible(false)
      .SetEditable(false);
  (void)item.AddProperty(itemconstants::kXpos, 0.0).SetDisplayName("X").SetVisible(false);
  (void)item.AddProperty(itemconstants::kYpos, 0.0).SetDisplayName("Y").SetVisible(false);
  (void)item.AddProperty(itemconstants::kBreakpoint, 0)
      .SetDisplayName("breakpoint")
      .SetEditable(false)
      .SetVisible(false);
}

//! Registers the tag for the JSON type and value pair, with an empty AnyValueItem.
void AddAnyValueTag(mvvm::CompoundItem& item)
{
  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueEmptyItem>(mvvm::TagIndex::Append());
  (void)anyvalue_item->SetDisplayName(itemconstants::kAnyValueDefaultDisplayName);
  SetAttributeExposedFlag(false, *anyvalue_item);
}

//! Registers the tag for child instructions, according to the category.
void RegisterChildrenTag(instruction_t::Category category, InstructionItem& item)
{
  if (category == sup::oac_tree::Instruction::kCompound)
  {
    item.RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
                     /*as_default*/ true);
  }
  else if (category == sup::oac_tree::Instruction::kDecorator)
  {
    item.RegisterTag(mvvm::TagInfo(itemconstants::kChildInstructions, 0, 1, {}),
                     /*as_default*/ true);
  }
  else
  {
    // leaf instructions do not have children
  }
}

//! Adds a string attribute for an attribute unknown to the descriptor.
void AddStringAttribute(const std::string& name, const std::string& value, mvvm::CompoundItem& item)
{
  auto& property = item.AddProperty<AttributeItem>(name);
  property.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
  (void)property.SetDisplayName(name);
  (void)property.SetData(value);
}

//! Sets up the item for the given descriptor: domain type, attributes, tags, GUI properties.
void PopulateInstructionItem(const InstructionDescriptor& descriptor, InstructionItem& item)
{
  item.SetDomainType(descriptor.domain_type);
  (void)item.SetDisplayName(descriptor.domain_type);

  auto& attributes = item.AddProperty<mvvm::CompoundItem>(itemconstants::kAttributes);
  (void)attributes.SetDisplayName("Attributes");
  for (const auto& attribute : descriptor.attributes)
  {
    if (!IsAttributeProperty(attribute.name))
    {
      continue;
    }
    auto& property = attributes.AddProperty<AttributeItem>(attribute.name);
    property.SetAttributeType(attribute.default_value, attribute.nullable, attribute.allow_text);
    if (attribute.is_set)
    {
      (void)property.SetData(attribute.default_value);
    }
    (void)property.SetDisplayName(attribute.name);
    if (!attribute.tooltip.empty())
    {
      (void)property.SetToolTip(attribute.tooltip);
    }
    if (!attribute.visible)
    {
      (void)property.SetVisible(false);
    }
  }

  RegisterChildrenTag(descriptor.category, item);
  AddGuiProperties(item);

  if (auto property = attributes.FindItem(domainconstants::kShowCollapsedAttribute); property)
  {
    (void)property->SetDisplayName("Show collapsed");
    (void)property->SetToolTip("Show child branch collapsed during procedure execution");
  }

  if ((FindAttributeDescriptor(descriptor, domainconstants::kTypeAttribute) != nullptr)
      && (FindAttributeDescriptor(descriptor, domainconstants::kValueAttribute) != nullptr))
  {
    AddAnyValueTag(attributes);
  }
}

//! Sets attribute values of the node to the item.
void SetAttributeValues(const InstructionNode& node, InstructionItem& item)
{
  auto& attributes = *item.GetAttributes();
  const bool has_anyvalue_tag = mvvm::utils::HasTag(attributes, itemconstants::kAnyValueTag);
  std::string type_json;
  std::string value_json;

  for (const auto& [name, value] : node.attributes)
  {
    if (IsAttributeProperty(name) && FindAttributeDescriptor(node.descriptor, name))
    {
      SetAttributeFromString(value, *attributes.FindItem<AttributeItem>(name));
    }
    else if (name == domainconstants::kNameAttribute)
    {
      if (!value.empty())
      {
        (void)item.SetDisplayName(value);
      }
    }
    else if (has_anyvalue_tag && name == domainconstants::kTypeAttribute)
    {
      type_json = value;
    }
    else if (has_anyvalue_tag && name == domainconstants::kValueAttribute)
    {
      value_json = value;
    }
    else
    {
      AddStringAttribute(name, value, attributes);
    }
  }

  if (!type_json.empty() && !value_json.empty())
  {
    SetAnyValueFromJsonString(type_json, value_json, item);
  }
}

//! Creates an item for the given node, without children.
std::unique_ptr<InstructionItem> CreateItem(const InstructionNode& node)
{
  auto result = std::make_unique<InstructionItem>();
  PopulateInstructionItem(node.descriptor, *result);
  SetAttributeValues(node, *result);
  return result;
}

}  // namespace

InstructionTree CreateInstructionItemTree(const InstructionNode& node)
{
  auto root = CreateItem(node);
  std::vector<std::pair<std::uint32_t, InstructionItem*>> items;
  std::stack<std::pair<const InstructionNode*, InstructionItem*>> stack;
  stack.emplace(&node, root.get());

  while (!stack.empty())
  {
    auto [current, item] = stack.top();
    stack.pop();
    items.emplace_back(current->index, item);

    for (const auto& child : current->children)
    {
      auto child_item = CreateItem(child);
      auto* child_item_ptr = child_item.get();
      (void)item->InsertItem(std::move(child_item), mvvm::TagIndex::Append());
      stack.emplace(&child, child_item_ptr);
    }
  }

  std::vector<InstructionItem*> indexes(items.size(), nullptr);
  for (const auto& [index, item] : items)
  {
    if (index >= indexes.size())
    {
      throw RuntimeException("Instruction index [" + std::to_string(index) + "] is out of range");
    }
    indexes[index] = item;
  }
  return {std::move(root), std::move(indexes)};
}

std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type)
{
  return CreateInstructionItemTree(CreateInstructionNode(domain_type)).root;
}

std::unique_ptr<instruction_t> CreateDomainInstruction(const InstructionItem& item)
{
  auto result = CreateDomainInstruction(item.GetDomainType());

  // all attributes, visible or not
  const auto properties = item.GetAttributes()->GetAllItems();
  for (const auto* attribute : mvvm::utils::CastItems<const AttributeItem>(properties))
  {
    SetDomainAttribute(*attribute, attribute->GetTagIndex().GetTag(), *result);
  }

  if (item.GetDisplayName() != item.GetDomainType())
  {
    result->SetName(item.GetDisplayName());
  }

  if (auto anyvalue_item = GetAnyValueItem(item);
      anyvalue_item != nullptr && GetAttributeExposedFlag(*anyvalue_item))
  {
    SetJsonAttributesFromItem(*anyvalue_item, *result);
  }
  return result;
}

}  // namespace oac_tree_gui
