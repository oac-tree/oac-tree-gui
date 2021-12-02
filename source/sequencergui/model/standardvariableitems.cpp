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
// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------
static inline const std::string kFileName = "kFileName";

FileVariableItem::FileVariableItem() : VariableItem(Type)
{
  AddProperty(kFileName, std::string())->SetDisplayName("File name");
}

std::string FileVariableItem::GetDomainType() const
{
  return DomainConstants::kFileVariableType;
}

std::string FileVariableItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void FileVariableItem::SetFileName(const std::string &name)
{
  SetProperty(kFileName, name);
}

void FileVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(DomainConstants::kFileAttribute))
  {
    SetProperty(kFileName, variable->GetAttribute(DomainConstants::kFileAttribute));
  }
}

void FileVariableItem::SetupDomainImpl(variable_t *variable) const
{
  variable->AddAttribute(DomainConstants::kFileAttribute, GetFileName());
}

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

std::string LocalVariableItem::GetDomainType() const
{
  return DomainConstants::kLocalVariableType;
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

void LocalVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(DomainConstants::kTypeAttribute))
  {
    SetProperty(kType, variable->GetAttribute(DomainConstants::kTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kValueAttribute))
  {
    SetProperty(kValue, variable->GetAttribute(DomainConstants::kValueAttribute));
  }
}

void LocalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  variable->AddAttribute(DomainConstants::kTypeAttribute, GetJsonType());
  variable->AddAttribute(DomainConstants::kValueAttribute, GetJsonValue());
}

// ----------------------------------------------------------------------------
// UnknownVariableItem
// ----------------------------------------------------------------------------

UnknownVariableItem::UnknownVariableItem() : VariableItem(Type) {}

std::string UnknownVariableItem::GetDomainType() const
{
  return m_domain_name;
}

void UnknownVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  m_domain_name = variable->GetType();

  SetDisplayName(variable->GetType() + " (unknown)");

  // creating string properties for every domain attribute found
  for (auto [name, value] : DomainUtils::GetAttributes(variable))
  {
    m_domain_attributes.push_back(name);
    AddProperty(name, value);
  }
}

void UnknownVariableItem::SetupDomainImpl(variable_t *variable) const
{
  for (const auto &name : m_domain_attributes)
  {
    variable->AddAttribute(name, Property<std::string>(name));
  }
}

}  // namespace sequi
