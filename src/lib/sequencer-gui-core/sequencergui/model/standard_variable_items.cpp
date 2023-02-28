/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standard_variable_items.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/variable_item_transform_utils.h>

#include <sup/sequencer/variable.h>

namespace sequencergui
{
static inline const std::string kChannel = "kChannel";
static inline const std::string kJsonType = "kJsonType";
static inline const std::string kJsonValue = "kJsonValue";
static inline const std::string kIsAvailable = "kIsAvailable";

// ----------------------------------------------------------------------------
// ConnectableVariableItem
// ----------------------------------------------------------------------------

ConnectableVariableItem::ConnectableVariableItem(const std::string &variable_type)
    : VariableItem(variable_type)
{
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kIsAvailable, false)->SetDisplayName("connected")->SetEditable(false);
  AddProperty(kJsonType, std::string())->SetDisplayName("json type")->SetVisible(false);
  AddProperty(kJsonValue, std::string())->SetDisplayName("json value")->SetVisible(false);
  RegisterAnyValueItemTag();
}

std::string ConnectableVariableItem::GetChannel() const
{
  return Property<std::string>(kChannel);
}

void ConnectableVariableItem::SetChannel(const std::string &value)
{
  SetProperty(kChannel, value);
}

std::string ConnectableVariableItem::GetJsonType() const
{
  return Property<std::string>(kJsonType);
}

void ConnectableVariableItem::SetJsonType(const std::string &value)
{
  SetProperty(kJsonType, value);
}

std::string ConnectableVariableItem::GetJsonValue() const
{
  return Property<std::string>(kJsonValue);
}

// Temporary method to update value from SequencerObserver
void ConnectableVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kJsonValue, value);
}

bool ConnectableVariableItem::IsAvailable() const
{
  return Property<bool>(kIsAvailable);
}

void ConnectableVariableItem::SetIsAvailable(bool value)
{
  SetProperty(kIsAvailable, value);
}

void ConnectableVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(domainconstants::kChannelAttribute))
  {
    SetChannel(variable->GetAttribute(domainconstants::kChannelAttribute));
  }

  if (variable->HasAttribute(domainconstants::kTypeAttribute))
  {
    SetJsonType(variable->GetAttribute(domainconstants::kTypeAttribute));
  }

  if (variable->HasAttribute(domainconstants::kValueAttribute))
  {
    SetJsonValue(variable->GetAttribute(domainconstants::kValueAttribute));
  }
}

// ----------------------------------------------------------------------------
// ChannelAccessVariableItem
// ----------------------------------------------------------------------------

ChannelAccessVariableItem::ChannelAccessVariableItem() : ConnectableVariableItem(Type) {}

std::string ChannelAccessVariableItem::GetDomainType() const
{
  return domainconstants::kChannelAccessVariableType;
}

void ChannelAccessVariableItem::SetupDomainImpl(variable_t *variable) const
{
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kChannelAttribute, GetChannel());
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kTypeAttribute, GetJsonType());
}

// ----------------------------------------------------------------------------
// FileVariableItem
// ----------------------------------------------------------------------------
static inline const std::string kFileName = "kFileName";

FileVariableItem::FileVariableItem() : VariableItem(Type)
{
  AddProperty(kFileName, std::string())->SetDisplayName("File name");
  RegisterAnyValueItemTag();
}

std::string FileVariableItem::GetDomainType() const
{
  return domainconstants::kFileVariableType;
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
  if (variable->HasAttribute(domainconstants::kFileAttribute))
  {
    SetProperty(kFileName, variable->GetAttribute(domainconstants::kFileAttribute));
  }
}

void FileVariableItem::SetupDomainImpl(variable_t *variable) const
{
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kFileAttribute, GetFileName());
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem() : VariableItem(Type)
{
  AddProperty(kJsonType, std::string())->SetDisplayName("json type")->SetVisible(false);
  AddProperty(kJsonValue, std::string())->SetDisplayName("json value")->SetVisible(false);
  RegisterAnyValueItemTag();
}

std::string LocalVariableItem::GetDomainType() const
{
  return domainconstants::kLocalVariableType;
}

std::string LocalVariableItem::GetJsonType() const
{
  return Property<std::string>(kJsonType);
}

void LocalVariableItem::SetJsonType(const std::string &value)
{
  SetProperty(kJsonType, value);
}

std::string LocalVariableItem::GetJsonValue() const
{
  return Property<std::string>(kJsonValue);
}

void LocalVariableItem::SetJsonValue(const std::string &value)
{
  SetProperty(kJsonValue, value);
}

void LocalVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(domainconstants::kTypeAttribute))
  {
    SetJsonType(variable->GetAttribute(domainconstants::kTypeAttribute));
  }

  if (variable->HasAttribute(domainconstants::kValueAttribute))
  {
    SetJsonValue(variable->GetAttribute(domainconstants::kValueAttribute));
  }
}

void LocalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kTypeAttribute, GetJsonType());
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kValueAttribute, GetJsonValue());
}

// ----------------------------------------------------------------------------
// PVClientVariableItem
// ----------------------------------------------------------------------------

PVClientVariableItem::PVClientVariableItem() : ConnectableVariableItem(Type) {}

std::string PVClientVariableItem::GetDomainType() const
{
  return domainconstants::kPVClientVariableType;
}

void PVClientVariableItem::SetupDomainImpl(variable_t *variable) const
{
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kChannelAttribute, GetChannel());
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kTypeAttribute, GetJsonType());
}

// ----------------------------------------------------------------------------
// PVServerVariableItem
// ----------------------------------------------------------------------------

PVServerVariableItem::PVServerVariableItem() : ConnectableVariableItem(Type) {}

std::string PVServerVariableItem::GetDomainType() const
{
  return domainconstants::kPVServerVariableType;
}

void PVServerVariableItem::SetupDomainImpl(variable_t *variable) const
{
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kChannelAttribute, GetChannel());
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kTypeAttribute, GetJsonType());
  DomainUtils::AddNonEmptyAttribute(variable, domainconstants::kValueAttribute, GetJsonValue());
}

// ----------------------------------------------------------------------------
// UnknownVariableItem
// ----------------------------------------------------------------------------

UnknownVariableItem::UnknownVariableItem() : VariableItem(Type)
{
  RegisterAnyValueItemTag();
}

std::string UnknownVariableItem::GetDomainType() const
{
  return m_domain_name;
}

void UnknownVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  m_domain_name = variable->GetType();

  SetDisplayName(variable->GetType() + " (unknown)");

  // creating string properties for every domain attribute found
  for (const auto &[name, value] : DomainUtils::GetAttributes(variable))
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

}  // namespace sequencergui
