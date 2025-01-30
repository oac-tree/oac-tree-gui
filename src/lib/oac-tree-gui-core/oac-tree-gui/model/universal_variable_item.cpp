/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "universal_variable_item.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/transform/anyvalue_item_transform_helper.h>
#include <oac-tree-gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/variable.h>

namespace
{
const int kDomainTypeNameRole = 10;  // role to store type name

// These attributes shouldn't be used from the domain to build properties.
const std::vector<std::string> kSkipDomainAttributeList = {
    oac_tree_gui::domainconstants::kNameAttribute,  // handled via display name
    oac_tree_gui::domainconstants::kTypeAttribute,  // handled via AnyValueItem
    oac_tree_gui::domainconstants::kValueAttribute  // handled via AnyValueItem
};

// these are properties that shouldn't go to domain
const std::vector<std::string> kSkipItemTagList = {oac_tree_gui::itemconstants::kAnyValueTag};

}  // namespace

namespace oac_tree_gui
{

UniversalVariableItem::UniversalVariableItem(const std::string &item_type)
    : VariableItem(item_type.empty() ? GetStaticType() : item_type)
{
  if (IsVariableTypeAvailable(item_type))
  {
    SetDomainType(item_type);
  }
}

std::string UniversalVariableItem::GetStaticType()
{
  return "UniversalVariable";
}

std::unique_ptr<mvvm::SessionItem> UniversalVariableItem::Clone() const
{
  return std::make_unique<UniversalVariableItem>(*this);
}

std::string UniversalVariableItem::GetDomainType() const
{
  return HasData(kDomainTypeNameRole) ? Data<std::string>(kDomainTypeNameRole) : std::string();
}

void UniversalVariableItem::SetDomainType(const std::string &domain_type)
{
  // temporary domain variable is used to create default properties
  auto domain_variable = ::oac_tree_gui::CreateDomainVariable(domain_type);
  SetupFromDomain(domain_variable.get());
}

std::vector<UniversalVariableItem::Attribute> UniversalVariableItem::GetAttributeItems() const
{
  std::vector<UniversalVariableItem::Attribute> result;

  // for the moment any registered property has it's correspondance as domain attribute
  auto properties = mvvm::utils::SinglePropertyItems(*this);

  for (const auto property : mvvm::utils::CastItems<sup::gui::AnyValueItem>(properties))
  {
    auto tag = property->GetTagIndex().GetTag();

    if (!mvvm::utils::Contains(kSkipItemTagList, tag))
    {
      // tag of property item should coincide to domain's attribute name
      result.push_back({tag, property});
    }
  }

  return result;
}

void UniversalVariableItem::InitFromDomainImpl(const variable_t *variable,
                                               const anytype_registry_t *registry)
{
  if (GetDomainType().empty())
  {
    SetupFromDomain(variable);
  }

  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetPropertyFromDomainAttribute(*variable, attribute_name, *item);
  }

  SetDisplayName(variable->GetName().empty() ? variable->GetType() : variable->GetName());

  SetAnyValueFromDomainVariable(*variable, *this, registry);
}

void UniversalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetDomainAttribute(*item, attribute_name, *variable);
  }

  variable->SetName(GetDisplayName());

  if (auto anyvalue_item = GetAnyValueItem(); anyvalue_item)
  {
    SetJsonAttributesFromItem(*anyvalue_item, *variable);
  }
}

void UniversalVariableItem::SetupFromDomain(const variable_t *variable)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup variable twice");
  }

  SetData(variable->GetType(), kDomainTypeNameRole);

  for (const auto &definition : variable->GetAttributeDefinitions())
  {
    if (!mvvm::utils::Contains(kSkipDomainAttributeList, definition.GetName()))
    {
      AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterAnyValueItemTag();
}

}  // namespace oac_tree_gui
