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
#include <sequencergui/transform/variable_transform_helper.h>

#include <sup/sequencer/variable.h>

namespace sequencergui
{
static inline const std::string kChannel = "kChannel";
static inline const std::string kIsAvailable = "kIsAvailable";

// ----------------------------------------------------------------------------
// ConnectableVariableItem
// ----------------------------------------------------------------------------

ConnectableVariableItem::ConnectableVariableItem(const std::string &variable_type)
    : VariableItem(variable_type)
{
  AddProperty(kChannel, std::string())->SetDisplayName("channel");
  AddProperty(kIsAvailable, false)->SetDisplayName("connected")->SetEditable(false);
  RegisterJsonTypeAndValue();
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
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), variable);
  SetJsonTypeAttribute(*this, *variable);
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
  AddNonEmptyAttribute(domainconstants::kFileAttribute, GetFileName(), variable);
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem() : VariableItem(Type)
{
  RegisterJsonTypeAndValue();
  RegisterAnyValueItemTag();
}

std::string LocalVariableItem::GetDomainType() const
{
  return domainconstants::kLocalVariableType;
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
  SetJsonTypeAttribute(*this, *variable);
  SetJsonValueAttribute(*this, *variable);
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
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), variable);
  SetJsonTypeAttribute(*this, *variable);
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
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), variable);
  SetJsonTypeAttribute(*this, *variable);
  SetJsonValueAttribute(*this, *variable);
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
  for (const auto &[name, value] : GetAttributes(variable))
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
