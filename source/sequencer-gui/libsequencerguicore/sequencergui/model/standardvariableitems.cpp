/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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
#include "sequencergui/model/shallnotbenamedvalueutils.h"

namespace sequencergui
{
static inline const std::string kValue = "kValue";

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

static inline const std::string kDataType = "kDataType";
static inline const std::string kChannel = "kChannel";
ChannelAccessVariableItem::ChannelAccessVariableItem() : VariableItem(Type)
{
  AddProperty(kDataType, std::string())->SetDisplayName("datatype");
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kValue, std::string())->SetDisplayName("value");
}

std::string ChannelAccessVariableItem::GetDomainType() const
{
  return DomainConstants::kChannelAccessVariableType;
}

std::string ChannelAccessVariableItem::GetDataType() const
{
  return Property<std::string>(kDataType);
}

void ChannelAccessVariableItem::SetDataType(const std::string &value)
{
  SetProperty(kDataType, value);
}

std::string ChannelAccessVariableItem::GetChannel() const
{
  return Property<std::string>(kChannel);
}

void ChannelAccessVariableItem::SetChannel(const std::string &value)
{
  SetProperty(kChannel, value);
}

// Temporary method to update value from SequencerObserver
void ChannelAccessVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kValue, value);
}

void ChannelAccessVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(DomainConstants::kDataTypeAttribute))
  {
    SetDataType(variable->GetAttribute(DomainConstants::kDataTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kChannelAttribute))
  {
    SetChannel(variable->GetAttribute(DomainConstants::kChannelAttribute));
  }
  SetJsonValue(DomainUtils::GetValueJsonString(variable));
}

void ChannelAccessVariableItem::SetupDomainImpl(variable_t *variable) const
{
  variable->AddAttribute(DomainConstants::kDataTypeAttribute, GetDataType());
  variable->AddAttribute(DomainConstants::kChannelAttribute, GetChannel());
}

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
    SetJsonType(variable->GetAttribute(DomainConstants::kTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kValueAttribute))
  {
    SetJsonValue(variable->GetAttribute(DomainConstants::kValueAttribute));
  }
}

void LocalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  variable->AddAttribute(DomainConstants::kTypeAttribute, GetJsonType());
  if (!GetJsonValue().empty())
  {
    variable->AddAttribute(DomainConstants::kValueAttribute, GetJsonValue());
  }
}

// ----------------------------------------------------------------------------
// PVClientVariableItem
// ----------------------------------------------------------------------------

PVClientVariableItem::PVClientVariableItem() : VariableItem(Type)
{
  AddProperty(kDataType, std::string())->SetDisplayName("datatype");
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kValue, std::string())->SetDisplayName("value");
}

std::string PVClientVariableItem::GetDomainType() const
{
  return DomainConstants::kPVClientVariableType;
}

std::string PVClientVariableItem::GetDataType() const
{
  return Property<std::string>(kDataType);
}

void PVClientVariableItem::SetDataType(const std::string &value)
{
  SetProperty(kDataType, value);
}

std::string PVClientVariableItem::GetChannel() const
{
  return Property<std::string>(kChannel);
}

void PVClientVariableItem::SetChannel(const std::string &value)
{
  SetProperty(kChannel, value);
}

void PVClientVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kValue, value);
}

void PVClientVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(DomainConstants::kDataTypeAttribute))
  {
    SetDataType(variable->GetAttribute(DomainConstants::kDataTypeAttribute));
  }

  if (variable->HasAttribute(DomainConstants::kChannelAttribute))
  {
    SetChannel(variable->GetAttribute(DomainConstants::kChannelAttribute));
  }

  SetJsonValue(DomainUtils::GetValueJsonString(variable));
}

void PVClientVariableItem::SetupDomainImpl(variable_t *variable) const
{
  variable->AddAttribute(DomainConstants::kDataTypeAttribute, GetDataType());
  variable->AddAttribute(DomainConstants::kChannelAttribute, GetChannel());
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
