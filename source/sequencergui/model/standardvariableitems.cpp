/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standardvariableitems.h"

#include "Variable.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"

namespace sequi
{
static inline const std::string kName = "kName";

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

static inline const std::string kType = "kType";
static inline const std::string kValue = "kValue";

LocalVariableItem::LocalVariableItem() : VariableItem(Type)
{
  AddProperty(kType, std::string())->SetDisplayName("type");
  AddProperty(kValue, std::string())->SetDisplayName("value");
}

std::unique_ptr<variable_t> LocalVariableItem::CreateDomainVariable() const
{
  auto result = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  result->AddAttribute(DomainConstants::kNameAttribute, GetName());
  result->AddAttribute(DomainConstants::kTypeAttribute, GetJsonType());
  result->AddAttribute(DomainConstants::kValueAttribute, GetJsonValue());
  return result;
}

void LocalVariableItem::InitFromDomain(const variable_t *variable)
{
  VariableItem::InitFromDomain(variable);
  if (variable->HasAttribute(DomainConstants::kNameAttribute))
  {
    SetProperty(kName, variable->GetAttribute(DomainConstants::kNameAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kTypeAttribute))
  {
    SetProperty(kType, variable->GetAttribute(DomainConstants::kTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kValueAttribute))
  {
    SetProperty(kValue, variable->GetAttribute(DomainConstants::kValueAttribute));
  }
}

std::string LocalVariableItem::GetJsonType() const
{
  return Property<std::string>(kType);
}

void LocalVariableItem::SetJsonType(const std::string &value)
{
  SetProperty(kType, value);
}

std::string LocalVariableItem::GetJsonValue() const
{
  return Property<std::string>(kValue);
}

void LocalVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kValue, value);
}

// ----------------------------------------------------------------------------
// UnknownVariableItem
// ----------------------------------------------------------------------------

UnknownVariableItem::UnknownVariableItem() : VariableItem(Type) {}

void UnknownVariableItem::InitFromDomain(const variable_t *variable)
{
  SetDisplayName(variable->GetType() + " (unknown)");
}

}  // namespace sequi
