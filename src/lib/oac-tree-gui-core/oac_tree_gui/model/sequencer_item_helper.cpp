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

#include "sequencer_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/session_item.h>

namespace oac_tree_gui
{

const mvvm::SessionItem* GetPropertyItem(const mvvm::SessionItem& parent, const std::string& tag)
{
  // FindItem is total, it returns nullptr when the tag is not registered
  return parent.FindItem(tag);
}

mvvm::SessionItem* GetPropertyItem(mvvm::SessionItem& parent, const std::string& tag)
{
  return parent.FindItem(tag);
}

const mvvm::SessionItem* GetStatusItem(const mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kStatus);
}

mvvm::SessionItem* GetStatusItem(mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kStatus);
}

const mvvm::SessionItem* GetBreakpointItem(const mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kBreakpoint);
}

mvvm::SessionItem* GetBreakpointItem(mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kBreakpoint);
}

const mvvm::SessionItem* GetIsAvailableItem(const mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kIsAvailable);
}

mvvm::SessionItem* GetIsAvailableItem(mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, itemconstants::kIsAvailable);
}

const mvvm::SessionItem* GetChannelItem(const mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, domainconstants::kChannelAttribute);
}

mvvm::SessionItem* GetChannelItem(mvvm::SessionItem& parent)
{
  return GetPropertyItem(parent, domainconstants::kChannelAttribute);
}

}  // namespace oac_tree_gui
