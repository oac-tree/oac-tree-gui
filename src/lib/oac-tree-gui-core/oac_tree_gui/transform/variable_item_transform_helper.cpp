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

#include "variable_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/variable_info_item.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_descriptor_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>

#include <sup/dto/anytype.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace.h>
#include <sup/oac-tree/workspace_info.h>

#include <algorithm>

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
const AttributeDescriptor* FindAttributeDescriptor(const VariableDescriptor& descriptor,
                                                   const std::string& name)
{
  auto iter = std::find_if(descriptor.attributes.begin(), descriptor.attributes.end(),
                           [&name](const auto& attribute) { return attribute.name == name; });
  return iter == descriptor.attributes.end() ? nullptr : &*iter;
}

//! Adds a property for the attribute. Mandatory attributes are exposed.
void AddAttributeProperty(const AttributeDescriptor& attribute, VariableItem& item)
{
  auto& property = item.AddProperty<sup::gui::AnyValueScalarItem>(attribute.name);
  property.SetAnyTypeName(
      sup::gui::GetAnyValueFromScalar(attribute.default_value).GetType().GetTypeName());
  if (attribute.has_preset)
  {
    (void)property.SetData(attribute.default_value);
  }
  (void)property.SetDisplayName(attribute.name);
  SetAttributeExposedFlag(attribute.mandatory, property);
}

//! Adds an exposed string property for an attribute unknown to the descriptor.
void AddStringAttributeProperty(const std::string& name, const std::string& value,
                                VariableItem& item)
{
  auto& property = item.AddProperty<sup::gui::AnyValueScalarItem>(name);
  property.SetAnyTypeName(sup::dto::kStringTypeName);
  (void)property.SetDisplayName(name);
  (void)property.SetData(value);
  SetAttributeExposedFlag(true, property);
}

//! Sets up the item for the given descriptor: domain type, attributes, empty AnyValueItem.
void PopulateVariableItem(const VariableDescriptor& descriptor, VariableItem& item)
{
  item.SetDomainType(descriptor.domain_type);
  item.SetName(descriptor.domain_type);

  for (const auto& attribute : descriptor.attributes)
  {
    if (IsAttributeProperty(attribute.name))
    {
      AddAttributeProperty(attribute, item);
    }
  }

  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
  (void)item.InsertItem(CreateEmptyAnyValueItem(), mvvm::TagIndex::Append());
}

//! Sets attribute values of the node to the item.
void SetAttributeValues(const VariableNode& node, const anytype_registry_t* registry,
                        VariableItem& item)
{
  for (const auto& [name, value] : node.attributes)
  {
    if (name == domainconstants::kNameAttribute)
    {
      if (!value.empty())
      {
        item.SetName(value);
      }
    }
    else if (!IsAttributeProperty(name))
    {
      // type and value go to AnyValueItem below
    }
    else if (FindAttributeDescriptor(node.descriptor, name) != nullptr)
    {
      SetPropertyFromAttributeString(value, *item.FindItem<sup::gui::AnyValueItem>(name));
    }
    else
    {
      AddStringAttributeProperty(name, value, item);
    }
  }

  SetAnyValueFromAttributes(node.attributes, item, registry);
}

//! Returns property items representing domain attributes.
std::vector<const sup::gui::AnyValueItem*> GetAttributeProperties(const VariableItem& item)
{
  std::vector<const sup::gui::AnyValueItem*> result;
  for (const auto* property :
       mvvm::utils::CastItems<const sup::gui::AnyValueItem>(mvvm::utils::SinglePropertyItems(item)))
  {
    if (property->GetTagIndex().GetTag() != itemconstants::kAnyValueTag)
    {
      result.push_back(property);
    }
  }
  return result;
}

}  // namespace

std::unique_ptr<VariableItem> CreateVariableInfoItem(const sup::oac_tree::VariableInfo& info)
{
  auto result = std::make_unique<VariableInfoItem>();
  result->InitFromDomainInfo(info);

  return result;
}

std::unique_ptr<VariableItem> CreateVariableItem(const VariableNode& node,
                                                 const anytype_registry_t* registry)
{
  auto result = std::make_unique<VariableItem>();
  PopulateVariableItem(node.descriptor, *result);
  SetAttributeValues(node, registry, *result);
  return result;
}

std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type)
{
  return CreateVariableItem(CreateVariableNode(domain_type));
}

std::unique_ptr<variable_t> CreateDomainVariable(const VariableItem& item)
{
  auto result = CreateDomainVariable(item.GetDomainType());
  for (const auto* property : GetAttributeProperties(item))
  {
    SetDomainAttribute(*property, property->GetTagIndex().GetTag(), *result);
  }

  result->SetName(item.GetName());

  if (const auto* anyvalue_item = item.GetAnyValueItem(); anyvalue_item)
  {
    SetJsonAttributesFromItem(*anyvalue_item, *result);
  }
  return result;
}

std::vector<VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::WorkspaceInfo& info,
                                                       WorkspaceItem* workspace_item)
{
  std::vector<VariableItem*> index_to_variable_item;

  if (workspace_item->GetVariableCount() != 0)
  {
    throw RuntimeException("WorkspaceItem already contains some variables");
  }

  index_to_variable_item.resize(info.GetNumberOfVariables());

  for (const auto& name_and_info : info.GetVariableInfos())
  {
    const auto& variable_info = name_and_info.second;
    auto variable_item = CreateVariableInfoItem(variable_info);
    auto variable_item_ptr = variable_item.get();
    index_to_variable_item[static_cast<std::size_t>(variable_info.GetIndex())] = variable_item_ptr;
    (void)workspace_item->InsertItem(std::move(variable_item), mvvm::TagIndex::Append());
  }

  return index_to_variable_item;
}

std::vector<VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::Workspace& workspace,
                                                       const anytype_registry_t* registry,
                                                       WorkspaceItem* workspace_item)
{
  std::vector<VariableItem*> result;

  if (workspace_item->GetTotalItemCount() > 0)
  {
    throw std::runtime_error("Error: WorkspaceItem is not empty.");
  }

  for (auto variable : workspace.GetVariables())
  {
    auto item = CreateVariableItem(CreateVariableNode(*variable), registry);
    result.push_back(item.get());
    (void)workspace_item->InsertItem(std::move(item), mvvm::TagIndex::Append());
  }

  return result;
}

}  // namespace oac_tree_gui
