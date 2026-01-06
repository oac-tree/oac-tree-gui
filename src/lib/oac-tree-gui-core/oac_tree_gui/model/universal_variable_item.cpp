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

#include "universal_variable_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/variable.h>

namespace
{

/**
 * @brief Returns domain attributes that shouldn't be used to build properties.
 * @return
 */
const std::vector<std::string>& GetSkipDomainAttributeList()
{
  static const std::vector<std::string> kSkipDomainAttributeList = {
      oac_tree_gui::domainconstants::kNameAttribute,  // handled via display name
      oac_tree_gui::domainconstants::kTypeAttribute,  // handled via AnyValueItem
      oac_tree_gui::domainconstants::kValueAttribute  // handled via AnyValueItem
  };
  return kSkipDomainAttributeList;
}

/**
 * @brief Returns list of properties that shouldn't go to domain.
 */
const std::vector<std::string>& GetSkipItemTagList()
{
  static const std::vector<std::string> kSkipItemTagList = {
      oac_tree_gui::itemconstants::kAnyValueTag};
  return kSkipItemTagList;
}

}  // namespace

namespace oac_tree_gui
{

UniversalVariableItem::UniversalVariableItem()
    : VariableItem(mvvm::GetTypeName<UniversalVariableItem>())
{
}

UniversalVariableItem::UniversalVariableItem(const std::string& item_type)
    : VariableItem(item_type.empty() ? mvvm::GetTypeName<UniversalVariableItem>() : item_type)
{
  SetDomainTypeImpl(item_type);
}

std::unique_ptr<mvvm::SessionItem> UniversalVariableItem::Clone() const
{
  return std::make_unique<UniversalVariableItem>(*this);
}

void UniversalVariableItem::SetDomainType(const std::string& domain_type)
{
  SetDomainTypeImpl(domain_type);
}

std::vector<UniversalVariableItem::Attribute> UniversalVariableItem::GetAttributeItems() const
{
  std::vector<UniversalVariableItem::Attribute> result;

  // for the moment any registered property has it's correspondance as domain attribute
  auto properties = mvvm::utils::SinglePropertyItems(*this);

  for (const auto property : mvvm::utils::CastItems<sup::gui::AnyValueItem>(properties))
  {
    auto tag = property->GetTagIndex().GetTag();

    if (!mvvm::utils::Contains(GetSkipItemTagList(), tag))
    {
      // tag of property item should coincide to domain's attribute name
      result.push_back({tag, property});
    }
  }

  return result;
}

void UniversalVariableItem::InitFromDomainImpl(const variable_t* variable,
                                               const anytype_registry_t* registry)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(variable);
  }

  for (const auto& [attribute_name, item] : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*variable, attribute_name, *item);
  }

  (void)SetDisplayName(variable->GetName().empty() ? variable->GetType() : variable->GetName());

  SetAnyValueFromDomainVariable(*variable, *this, registry);
}

void UniversalVariableItem::SetupDomainImpl(variable_t* variable) const
{
  for (const auto& [attribute_name, item] : GetAttributeItems())
  {
    SetDomainAttribute(*item, attribute_name, *variable);
  }

  variable->SetName(GetDisplayName());

  if (auto anyvalue_item = GetAnyValueItem(); anyvalue_item)
  {
    SetJsonAttributesFromItem(*anyvalue_item, *variable);
  }
}

void UniversalVariableItem::SetDomainTypeImpl(const std::string& domain_type)
{
  // temporary domain variable is used to create default properties
  auto domain_variable = ::oac_tree_gui::CreateDomainVariable(domain_type);
  SetupFromDomain(domain_variable.get());
}

void UniversalVariableItem::SetupFromDomain(const variable_t* variable)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup variable twice");
  }

  (void)SetData(variable->GetType(), itemconstants::kDomainTypeNameRole);

  for (const auto& definition : variable->GetAttributeDefinitions())
  {
    if (!mvvm::utils::Contains(GetSkipDomainAttributeList(), definition.GetName()))
    {
      (void)AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterAnyValueItemTag();
}

}  // namespace oac_tree_gui
