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
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sequencergui/model/item_constants.h>

#include <sup/sequencer/variable.h>

namespace sequencergui
{

// ----------------------------------------------------------------------------
// ConnectableVariableItem
// ----------------------------------------------------------------------------

ConnectableVariableItem::ConnectableVariableItem(const std::string &variable_type)
    : VariableItem(variable_type)
{
  RegisterCommonProperties();
  AddProperty(itemconstants::kChannel, std::string())->SetDisplayName("channel");
  AddProperty(itemconstants::kIsAvailable, false)->SetDisplayName("connected")->SetEditable(false);
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
  return Property<std::string>(itemconstants::kChannel);
}

void ConnectableVariableItem::SetChannel(const std::string &value)
{
  SetProperty(itemconstants::kChannel, value);
}

bool ConnectableVariableItem::IsAvailable() const
{
  return Property<bool>(itemconstants::kIsAvailable);
}

void ConnectableVariableItem::SetIsAvailable(bool value)
{
  SetProperty(itemconstants::kIsAvailable, value);
}

void ConnectableVariableItem::InitFromDomainImpl(const variable_t *variable)
{
  if (variable->HasAttribute(domainconstants::kChannelAttribute))
  {
    SetChannel(variable->GetAttributeString(domainconstants::kChannelAttribute));
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
FileVariableItem::FileVariableItem() : sequencergui::UniversalVariableItem(Type) {}

std::unique_ptr<mvvm::SessionItem> FileVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<FileVariableItem>(*this, make_unique_id);
}

std::string FileVariableItem::GetFileName() const
{
  return Property<std::string>(sequencergui::domainconstants::kFileAttribute);
}

void FileVariableItem::SetFileName(const std::string &name)
{
  SetProperty(sequencergui::domainconstants::kFileAttribute, name);
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem() : sequencergui::UniversalVariableItem(Type) {}

std::unique_ptr<mvvm::SessionItem> LocalVariableItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LocalVariableItem>(*this, make_unique_id);
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

}  // namespace sequencergui
