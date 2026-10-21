/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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
#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

// ----------------------------------------------------------------------------
// ConnectableVariableItem
// ----------------------------------------------------------------------------

ConnectableVariableItem::ConnectableVariableItem(const std::string& variable_type)
    : UniversalVariableItem(variable_type)
{
}

std::unique_ptr<mvvm::SessionItem> ConnectableVariableItem::Clone() const
{
  // This base is not intended to be used directly
  throw NotImplementedException("Clone for ConnectableVariableItem is not implemented");
}

// ----------------------------------------------------------------------------
// ChannelAccessVariableItem
// ----------------------------------------------------------------------------

ChannelAccessVariableItem::ChannelAccessVariableItem()
    : ConnectableVariableItem(mvvm::GetTypeName<ChannelAccessVariableItem>())
{
}

std::unique_ptr<mvvm::SessionItem> ChannelAccessVariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

// ----------------------------------------------------------------------------
// FileVariableItem
// ----------------------------------------------------------------------------
FileVariableItem::FileVariableItem()
    : oac_tree_gui::UniversalVariableItem(mvvm::GetTypeName<FileVariableItem>())
{
}

std::unique_ptr<mvvm::SessionItem> FileVariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

// ----------------------------------------------------------------------------
// LocalVariableItem
// ----------------------------------------------------------------------------

LocalVariableItem::LocalVariableItem()
    : oac_tree_gui::UniversalVariableItem(mvvm::GetTypeName<LocalVariableItem>())
{
}

std::unique_ptr<mvvm::SessionItem> LocalVariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

// ----------------------------------------------------------------------------
// PvAccessClientVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessClientVariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

PvAccessClientVariableItem::PvAccessClientVariableItem()
    : ConnectableVariableItem(mvvm::GetTypeName<PvAccessClientVariableItem>())
{
}

// ----------------------------------------------------------------------------
// PvAccessServerVariableItem
// ----------------------------------------------------------------------------

std::unique_ptr<mvvm::SessionItem> PvAccessServerVariableItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

PvAccessServerVariableItem::PvAccessServerVariableItem()
    : ConnectableVariableItem(mvvm::GetTypeName<PvAccessServerVariableItem>())
{
}

}  // namespace oac_tree_gui
