/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/attribute_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/variable.h>

namespace
{
const int kDomainTypeNameRole = 10;  // role to store type name

// These attributes shouldn't be used from the domain to build properties.
const std::vector<std::string> kSkipDomainAttributeList = {
    sequencergui::domainconstants::kTypeAttribute,  // handled via AnyValueItem
    sequencergui::domainconstants::kValueAttribute  // handled via AnyValueItem
};

// these are properties that shouldn't go to domain
const std::vector<std::string> kSkipItemTagList = {sequencergui::itemconstants::kAnyValueTag};

}  // namespace

namespace sequencergui
{

UniversalVariableItem::UniversalVariableItem(const std::string &item_type)
    : VariableItem(item_type.empty() ? Type : item_type)
{
  if (IsVariableTypeAvailable(item_type))
  {
    SetDomainType(item_type);
  }
}

std::unique_ptr<mvvm::SessionItem> UniversalVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UniversalVariableItem>(*this, make_unique_id);
}

std::string UniversalVariableItem::GetDomainType() const
{
  return HasData(kDomainTypeNameRole) ? Data<std::string>(kDomainTypeNameRole) : std::string();
}

void UniversalVariableItem::SetDomainType(const std::string &domain_type)
{
  // temporary domain variable is used to create default properties
  auto domain_variable = ::sequencergui::CreateDomainVariable(domain_type);
  SetupFromDomain(domain_variable.get());
}

std::vector<UniversalVariableItem::Attribute> UniversalVariableItem::GetAttributeItems() const
{
  std::vector<UniversalVariableItem::Attribute> result;

  // for the moment any registered property has it's correspondance as domain attribute
  auto properties = mvvm::utils::SinglePropertyItems(*this);

  for (const auto property : mvvm::utils::CastItems<AttributeItem>(properties))
  {
    auto [tag, index] = property->GetTagIndex();

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

  SetAnyValueFromDomainVariable(*variable, *this, registry);
}

void UniversalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  for (const auto &[attribute_name, item] : GetAttributeItems())
  {
    SetDomainAttribute(*item, attribute_name, *variable);
  }

  if (GetAnyValueItem())
  {
    // Here we allow LocalVariableItem do not have AnyValueItem on board.
    // FIXME make sure variable needs such attributes
    SetJsonTypeAttribute(*this, *variable);
    SetJsonValueAttribute(*this, *variable);
  }
}

void UniversalVariableItem::SetupFromDomain(const variable_t *variable)
{
  if (!GetDomainType().empty())
  {
    throw LogicErrorException("It is not possible to setup variable twice");
  }

  SetData(variable->GetType(), kDomainTypeNameRole);

  SetDisplayName(variable->GetType());

  for (const auto &definition : variable->GetAttributeDefinitions())
  {
    if (!mvvm::utils::Contains(kSkipDomainAttributeList, definition.GetName()))
    {
      AddPropertyFromDefinition(definition, *this);
    }
  }

  RegisterAnyValueItemTag();
}

}  // namespace sequencergui
