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

#ifndef OAC_TREE_GUI_COMPONENTS_CUSTOM_VIEWITEM_FACTORY_H_
#define OAC_TREE_GUI_COMPONENTS_CUSTOM_VIEWITEM_FACTORY_H_

//! @file
//! Collection of factory functions to create custom view items.

#include <memory>

namespace mvvm
{
class ViewItem;
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief Returns view item to present channel name and is_available status.
 *
 * @details If item doesn't have corresponding properties (channel name and is_available properties)
 * will return placeholder.
 */
std::unique_ptr<mvvm::ViewItem> CreateChannelPresentationViewItem(mvvm::SessionItem& item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_VIEWITEM_FACTORY_H_
