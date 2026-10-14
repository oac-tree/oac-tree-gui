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
#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/universal_instruction_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <stack>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The InstructionInfoStackNode structs stores stack information during traversing of
 * InstructionInfo object.
 */
struct InstructionInfoStackNode
{
  const sup::oac_tree::InstructionInfo& info;
  oac_tree_gui::InstructionItem& item;
};

/**
 * @brief Creates instruction tree from provided domain information.
 *
 * Depending on light_tree flag, the resulting tree can be either InstructionInfoItem based, or will
 * contain a proper InstructionItem hierarchy.
 *
 * @param info Instruction info from the domain
 * @param light_tree Output tree will be based of InstructionInfoItem if true
 * @return Instruction tree.
 */
InstructionTree CreateInstructionItemTreeIntern(const sup::oac_tree::InstructionInfo& info,
                                                bool light_tree)
{
  std::stack<InstructionInfoStackNode> stack;

  std::unique_ptr<InstructionItem> result =
      light_tree ? CreateInstructionInfoItem(info) : CreateInstructionItem(info);
  std::map<InstructionItem*, std::uint32_t> item_to_index;

  stack.push({info, *result});
  item_to_index[result.get()] = info.GetIndex();

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto child_info : node.info.Children())
    {
      auto child_item =
          light_tree ? CreateInstructionInfoItem(*child_info) : CreateInstructionItem(*child_info);
      auto child_item_ptr = child_item.get();
      item_to_index[child_item_ptr] = child_info->GetIndex();

      (void)node.item.InsertItem(std::move(child_item), mvvm::TagIndex::Append());

      stack.push({*child_info, *child_item_ptr});
    }
  }

  std::vector<InstructionItem*> index_list(item_to_index.size());
  (void)std::for_each(item_to_index.begin(), item_to_index.end(),
                      [&index_list](auto it) { index_list[it.second] = it.first; });

  return {std::move(result), std::move(index_list)};
}

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
void AddAnyValueTag(InstructionItem& item)
{
  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueEmptyItem>(mvvm::TagIndex::Append());
  (void)anyvalue_item->SetDisplayName(itemconstants::kAnyValueDefaultDisplayName);
  SetAttributeExposedFlag(false, *anyvalue_item);
}

//! Adds a string property for an attribute unknown to the descriptor.
void AddStringAttribute(const std::string& name, const std::string& value, InstructionItem& item)
{
  auto& property = item.AddProperty<sup::gui::AnyValueScalarItem>(name);
  property.SetAnyTypeName(sup::dto::kStringTypeName);
  (void)property.SetDisplayName(name);
  (void)property.SetData(value);
}

//! Sets up the item for the given descriptor: domain type, attributes, tags, GUI properties.
void PopulateInstructionItem(const InstructionDescriptor& descriptor, InstructionItem& item)
{
  // the override in UniversalInstructionItem runs the domain setup
  item.InstructionItem::SetDomainType(descriptor.domain_type);
  (void)item.SetDisplayName(descriptor.domain_type);

  for (const auto& attribute : descriptor.attributes)
  {
    if (!IsAttributeProperty(attribute.name))
    {
      continue;
    }
    auto& property = item.AddProperty<sup::gui::AnyValueScalarItem>(attribute.name);
    property.SetAnyTypeName(sup::gui::GetAnyValueFromScalar(attribute.default_value).GetTypeName());
    (void)property.SetData(attribute.default_value);
    (void)property.SetDisplayName(attribute.name);
    if (!attribute.tooltip.empty())
    {
      (void)property.SetToolTip(attribute.tooltip);
    }
    SetAttributeExposedFlag(attribute.is_set, property);
  }

  RegisterChildrenTag(descriptor.category, item);
  AddGuiProperties(item);

  if (auto property =
          item.FindItem<sup::gui::AnyValueItem>(domainconstants::kShowCollapsedAttribute);
      property)
  {
    (void)property->SetDisplayName("Show collapsed");
    (void)property->SetToolTip("Show child branch collapsed during procedure execution");
  }

  if (FindAttributeDescriptor(descriptor, domainconstants::kTypeAttribute)
      && FindAttributeDescriptor(descriptor, domainconstants::kValueAttribute))
  {
    AddAnyValueTag(item);
  }
}

//! Sets attribute values of the node to the item.
void SetAttributeValues(const InstructionNode& node, InstructionItem& item)
{
  const bool has_anyvalue_tag = mvvm::utils::HasTag(item, itemconstants::kAnyValueTag);
  std::string type_json;
  std::string value_json;

  for (const auto& [name, value] : node.attributes)
  {
    if (IsAttributeProperty(name) && FindAttributeDescriptor(node.descriptor, name))
    {
      SetPropertyFromAttributeString(value, *item.FindItem<sup::gui::AnyValueItem>(name));
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
      AddStringAttribute(name, value, item);
    }
  }

  if (!type_json.empty() && !value_json.empty())
  {
    SetAnyValueFromJsonString(type_json, value_json, item);
  }
}

//! Creates an item tree for the given node. Collects items with their automation indexes.
std::unique_ptr<InstructionItem> CreateItemTree(
    const InstructionNode& node, std::vector<std::pair<std::uint32_t, InstructionItem*>>& items)
{
  std::unique_ptr<InstructionItem> result = std::make_unique<UniversalInstructionItem>();
  PopulateInstructionItem(node.descriptor, *result);
  SetAttributeValues(node, *result);
  items.emplace_back(node.index, result.get());

  for (const auto& child : node.children)
  {
    (void)result->InsertItem(CreateItemTree(child, items), mvvm::TagIndex::Append());
  }
  return result;
}

}  // namespace

InstructionTree CreateInstructionItemTree(const InstructionNode& node)
{
  std::vector<std::pair<std::uint32_t, InstructionItem*>> items;
  auto root = CreateItemTree(node, items);

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

std::unique_ptr<instruction_t> CreateDomainInstruction(const InstructionItem& item)
{
  auto result = CreateDomainInstruction(item.GetDomainType());

  const auto properties = mvvm::utils::SinglePropertyItems(item);
  for (const auto* property : mvvm::utils::CastItems<const sup::gui::AnyValueItem>(properties))
  {
    if (const auto tag = property->GetTagIndex().GetTag(); tag != itemconstants::kAnyValueTag)
    {
      SetDomainAttribute(*property, tag, *result);
    }
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

std::unique_ptr<InstructionItem> CreateInstructionItem(const sup::oac_tree::InstructionInfo& info)
{
  auto result = oac_tree_gui::CreateInstructionItem(info.GetType());

  // We create domain instruction only to be able to use InitFromDomain mechanism for attribute
  // propagation.
  auto domain = CreateDomainInstruction(info);
  result->InitFromDomain(domain.get());

  return result;
}

std::unique_ptr<InstructionItem> CreateInstructionInfoItem(
    const sup::oac_tree::InstructionInfo& info)
{
  auto result = std::make_unique<InstructionInfoItem>();
  result->InitFromDomainInfo(info);

  return result;
}

InstructionTree CreateInstructionItemTree(const sup::oac_tree::InstructionInfo& info)
{
  return CreateInstructionItemTreeIntern(info, /*light_tree*/ false);
}

InstructionTree CreateInstructionInfoItemTree(const sup::oac_tree::InstructionInfo& info)
{
  return CreateInstructionItemTreeIntern(info, /*light_tree*/ true);
}

void RegisterChildrenTag(const instruction_t& instruction, mvvm::CompoundItem& item)
{
  RegisterChildrenTag(instruction.GetCategory(), item);
}

void RegisterChildrenTag(instruction_t::Category category, mvvm::CompoundItem& item)
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

void RegisterShowCollapsedProperty(instruction_t::Category category, const std::string& domain_type,
                                   mvvm::CompoundItem& item)
{
  static const std::vector<std::string> kCollapsedByDefault{
      domainconstants::kIncludeInstructionType, domainconstants::kIncludeProcedureInstructionType};

  if (category != sup::oac_tree::Instruction::kAction)
  {
    auto& property =
        item.AddProperty<sup::gui::AnyValueScalarItem>(domainconstants::kShowCollapsedAttribute);
    property.SetAnyTypeName(sup::dto::kBooleanTypeName);
    (void)property.SetDisplayName("Show collapsed");
    (void)property.SetToolTip("Show child branch collapsed during procedure execution");
    (void)property.SetData(mvvm::utils::Contains(kCollapsedByDefault, domain_type));
  }
}

}  // namespace oac_tree_gui
