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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "standard_variable_items.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/oac-tree/variable.h>

namespace oac_tree_gui
{

// ----------------------------------------------------------------------------
// ConnectableVariableItem
// ----------------------------------------------------------------------------

ConnectableVariableItem::ConnectableVariableItem(const std::string &variable_type)
    : UniversalVariableItem(variable_type)
{
}

std::unique_ptr<mvvm::SessionItem> ConnectableVariableItem::Clone() const
{
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for ConnectableVariableItem is not implemented");
}

std::string ConnectableVariableItem::GetChannel() const
{
  return Property<std::string>(domainconstants::kChannelAttribute);
}

void ConnectableVariableItem::SetChannel(const std::string &value)
{
  SetProperty(domainconstants::kChannelAttribute, value);
}

// ----------------------------------------------------------------------------
// ChannelAccessVariableItem
// ----------------------------------------------------------------------------

ChannelAccessVariableItem::ChannelAccessVariableItem() : ConnectableVariableItem(GetStaticType()) {}

std::string ChannelAccessVariableItem::GetStaticType()
{
  return domainconstants::kChannelAccessVariableType;
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessVariableItem::Clone() const
{
  return std::make_unique<ChannelAccessVariableItem>(*this);
}

// ----------------------------------------------------------------------------
// FileVariableItem
// ----------------------------------------------------------------------------
FileVariableItem::FileVariableItem() : oac_tree_gui::UniversalVariableItem(GetStaticType()) {}

std::string FileVariableItem::GetStaticType()
{
  return domainconstants::kFileVariableType;
}

std::unique_ptr<mvvm::SessionItem> FileVariableItem::Clone() const
{
  return std::make_unique<FileVariableItem>(*this);
}

std::string FileVariableItem::GetFileName() const
{
  return Property<std::string>(oac_tree_gui::domainconstants::kFileNameAttribute);
}

void FileVariableItem::SetFileName(const std::string &name)
{
  SetProperty(oac_tree_gui::domainconstants::kFileNameAttribute, name);
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem() : oac_tree_gui::UniversalVariableItem(GetStaticType()) {}

std::string LocalVariableItem::GetStaticType()
{
  return domainconstants::kLocalVariableType;
}

std::unique_ptr<mvvm::SessionItem> LocalVariableItem::Clone() const
{
  return std::make_unique<LocalVariableItem>(*this);
}

// ----------------------------------------------------------------------------
// PvAccessClientVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessClientVariableItem::Clone() const
{
  return std::make_unique<PvAccessClientVariableItem>(*this);
}

PvAccessClientVariableItem::PvAccessClientVariableItem() : ConnectableVariableItem(GetStaticType())
{
}

std::string PvAccessClientVariableItem::GetStaticType()
{
  return domainconstants::kPvAccessClientVariableType;
}

// ----------------------------------------------------------------------------
// PvAccessServerVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessServerVariableItem::Clone() const
{
  return std::make_unique<PvAccessServerVariableItem>(*this);
}

PvAccessServerVariableItem::PvAccessServerVariableItem() : ConnectableVariableItem(GetStaticType())
{
}

std::string PvAccessServerVariableItem::GetStaticType()
{
  return domainconstants::kPvAccessServerVariableType;
}

}  // namespace oac_tree_gui
