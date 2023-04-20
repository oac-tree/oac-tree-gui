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

#include "standard_variable_items.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/transform_helpers.h>

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
  RegisterAnyValueItemTag();
}

std::unique_ptr<mvvm::SessionItem> ConnectableVariableItem::Clone(bool make_unique_id) const
{
  (void)make_unique_id;
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for ConnectableVariableItem is not implemented");
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
}

// ----------------------------------------------------------------------------
// ChannelAccessVariableItem
// ----------------------------------------------------------------------------

ChannelAccessVariableItem::ChannelAccessVariableItem() : ConnectableVariableItem(Type) {}

std::unique_ptr<mvvm::SessionItem> ChannelAccessVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ChannelAccessVariableItem>(*this, make_unique_id);
}

std::string ChannelAccessVariableItem::GetDomainType() const
{
  return domainconstants::kChannelAccessVariableType;
}

void ChannelAccessVariableItem::SetupDomainImpl(variable_t *variable) const
{
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), *variable);
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

std::unique_ptr<mvvm::SessionItem> FileVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<FileVariableItem>(*this, make_unique_id);
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
  AddNonEmptyAttribute(domainconstants::kFileAttribute, GetFileName(), *variable);
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem() : VariableItem(Type)
{
  RegisterAnyValueItemTag();
}

std::unique_ptr<mvvm::SessionItem> LocalVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LocalVariableItem>(*this, make_unique_id);
}

std::string LocalVariableItem::GetDomainType() const
{
  return domainconstants::kLocalVariableType;
}

void LocalVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  // nothing to od besides what is done in a base class
}

void LocalVariableItem::SetupDomainImpl(variable_t *variable) const
{
  if (GetAnyValueItem())
  {
    // Here we allow LocalVariableItem do not have AnyValueItem on board. LocalVariable is the only
    // one for the moment that can be without JSON type and value attributes (see
    // ResetVariableInstruction).
    SetJsonTypeAttribute(*this, *variable);
    SetJsonValueAttribute(*this, *variable);
  }
}

// ----------------------------------------------------------------------------
// PvAccessClientVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessClientVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessClientVariableItem>(*this, make_unique_id);
}

PvAccessClientVariableItem::PvAccessClientVariableItem() : ConnectableVariableItem(Type) {}

std::string PvAccessClientVariableItem::GetDomainType() const
{
  return domainconstants::kPvAccessClientVariableType;
}

void PvAccessClientVariableItem::SetupDomainImpl(variable_t *variable) const
{
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), *variable);
  SetJsonTypeAttribute(*this, *variable);
}

// ----------------------------------------------------------------------------
// PvAccessServerVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessServerVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PvAccessServerVariableItem>(*this, make_unique_id);
}

PvAccessServerVariableItem::PvAccessServerVariableItem() : ConnectableVariableItem(Type) {}

std::string PvAccessServerVariableItem::GetDomainType() const
{
  return domainconstants::kPvAccessServerVariableType;
}

void PvAccessServerVariableItem::SetupDomainImpl(variable_t *variable) const
{
  AddNonEmptyAttribute(domainconstants::kChannelAttribute, GetChannel(), *variable);
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

std::unique_ptr<mvvm::SessionItem> UnknownVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<UnknownVariableItem>(*this, make_unique_id);
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
