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

#ifndef OAC_TREE_GUI_MODEL_SEQUENCER_ITEM_HELPER_H_
#define OAC_TREE_GUI_MODEL_SEQUENCER_ITEM_HELPER_H_

//! @file
//! Functions to find property items of sequencer items.

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief Returns an item representing a status property, or nullptr if the given parent doesn't
 * have such property registered.
 */
const mvvm::SessionItem* GetStatusItem(const mvvm::SessionItem& parent);

mvvm::SessionItem* GetStatusItem(mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing a breakpoint property, or nullptr if the given parent doesn't
 * have such property registered.
 */
const mvvm::SessionItem* GetBreakpointItem(const mvvm::SessionItem& parent);

mvvm::SessionItem* GetBreakpointItem(mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing is_available property, or nullptr if the given parent doesn't
 * have such property registered.
 */
const mvvm::SessionItem* GetIsAvailableItem(const mvvm::SessionItem& parent);

mvvm::SessionItem* GetIsAvailableItem(mvvm::SessionItem& parent);

/**
 * @brief Returns the channel attribute from the attribute container of the given parent, or nullptr
 * if there is no such attribute.
 */
const mvvm::SessionItem* GetChannelItem(const mvvm::SessionItem& parent);

mvvm::SessionItem* GetChannelItem(mvvm::SessionItem& parent);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_SEQUENCER_ITEM_HELPER_H_
