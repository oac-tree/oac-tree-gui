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

#include "variable_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/make_clone.h>
#include <mvvm/utils/container_utils.h>

#include <sup/oac-tree/variable.h>

namespace oac_tree_gui
{

namespace
{

//! Returns true if the domain attribute becomes a property. Name, type and value are handled
//! separately.
bool IsAttributeProperty(const std::string& domain_attribute_name)
{
  static const std::vector<std::string> kSkipDomainAttributeList = {
      domainconstants::kNameAttribute, domainconstants::kTypeAttribute,
      domainconstants::kValueAttribute};

  return !mvvm::utils::Contains(kSkipDomainAttributeList, domain_attribute_name);
}

}  // namespace

VariableItem::VariableItem() : VariableItem(std::string(mvvm::GetTypeName<VariableItem>())) {}

VariableItem::VariableItem(const std::string& item_type) : CompoundItem(item_type)
{
  (void)SetFlag(mvvm::Appearance::kEditableDisplayName, true);
  (void)AddProperty(itemconstants::kIsAvailable, false)
      .SetDisplayName("connected")
      .SetEditable(false)
      .SetVisible(false);
}

std::unique_ptr<mvvm::SessionItem> VariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

std::string VariableItem::GetDomainType() const
{
  return HasData(itemconstants::kDomainTypeNameRole)
             ? Data<std::string>(itemconstants::kDomainTypeNameRole)
             : std::string();
}

void VariableItem::SetDomainType(const std::string& domain_type)
{
  // temporary domain variable is used to create default properties
  const auto domain_variable = ::oac_tree_gui::CreateDomainVariable(domain_type);
  SetupFromDomain(domain_variable.get());
}

std::unique_ptr<variable_t> VariableItem::CreateDomainVariable() const
{
  auto result = ::oac_tree_gui::CreateDomainVariable(GetDomainType());
  SetupDomainImpl(result.get());
  return result;
}

void VariableItem::InitFromDomain(const variable_t* variable, const anytype_registry_t* registry)
{
  InitFromDomainImpl(variable, registry);
}

std::string VariableItem::GetName() const
{
  return GetDisplayName();
}

void VariableItem::SetName(const std::string& value)
{
  (void)SetDisplayName(value);
}

void VariableItem::SetAttribute(const std::string& name, const mvvm::variant_t& value)
{
  auto* attribute = FindItem<sup::gui::AnyValueItem>(name);
  if (attribute == nullptr)
  {
    throw RuntimeException("Attribute [" + name + "] doesn't exist in variable [" + GetDomainType()
                           + "]");
  }
  if (value.index() != attribute->Data().index())
  {
    throw RuntimeException("Attribute [" + name + "] has type ["
                           + mvvm::utils::TypeName(attribute->Data()) + "], value type ["
                           + mvvm::utils::TypeName(value) + "]");
  }
  (void)attribute->SetData(value);
  SetAttributeExposedFlag(true, *attribute);
}

const sup::gui::AnyValueItem* VariableItem::GetAnyValueItem() const
{
  return FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
}

sup::gui::AnyValueItem* VariableItem::GetAnyValueItem()
{
  return FindItem<sup::gui::AnyValueItem>(itemconstants::kAnyValueTag);
}

bool VariableItem::IsAvailable() const
{
  return Property<bool>(itemconstants::kIsAvailable);
}

void VariableItem::SetIsAvailable(bool value)
{
  (void)SetProperty(itemconstants::kIsAvailable, value);
}

void VariableItem::RegisterAnyValueItemTag()
{
  RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);
}

void VariableItem::SetDomainTypeRole(const std::string& domain_type)
{
  (void)SetData(domain_type, itemconstants::kDomainTypeNameRole);
}

void VariableItem::InitFromDomainImpl(const variable_t* variable,
                                      const anytype_registry_t* registry)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(variable);
  }

  for (auto* item : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*variable, item->GetTagIndex().GetTag(), *item);
  }

  (void)SetDisplayName(variable->GetName().empty() ? variable->GetType() : variable->GetName());

  SetAnyValueFromDomainVariable(*variable, *this, registry);
}

void VariableItem::SetupDomainImpl(variable_t* variable) const
{
  for (const auto* item : GetAttributeItems())
  {
    SetDomainAttribute(*item, item->GetTagIndex().GetTag(), *variable);
  }

  variable->SetName(GetDisplayName());

  if (auto anyvalue_item = GetAnyValueItem(); anyvalue_item)
  {
    SetJsonAttributesFromItem(*anyvalue_item, *variable);
  }
}

void VariableItem::SetupFromDomain(const variable_t* variable)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup variable twice");
  }

  SetDomainTypeRole(variable->GetType());
  (void)SetDisplayName(variable->GetType());

  for (const auto& definition : variable->GetAttributeDefinitions())
  {
    if (IsAttributeProperty(definition.GetName()))
    {
      (void)AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterAnyValueItemTag();
}

std::vector<sup::gui::AnyValueItem*> VariableItem::GetAttributeItems() const
{
  // the tag of a property coincides with the domain attribute name
  std::vector<sup::gui::AnyValueItem*> result;
  for (auto* item : mvvm::utils::RemoveConst(mvvm::utils::CastItems<const sup::gui::AnyValueItem>(
           mvvm::utils::SinglePropertyItems(*this))))
  {
    if (item->GetTagIndex().GetTag() != itemconstants::kAnyValueTag)
    {
      result.push_back(item);
    }
  }
  return result;
}

}  // namespace oac_tree_gui
