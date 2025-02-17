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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MAINWINDOW_CLIPBOARD_HELPER_H_
#define OAC_TREE_GUI_MAINWINDOW_CLIPBOARD_HELPER_H_

//! @file
//! Collection of helper functions to deal with clipboard.

#include <functional>
#include <memory>

class QMimeData;

namespace oac_tree_gui
{

//!< A function to get clipboard data.
using get_mime_data_func_t = std::function<const QMimeData*()>;

//!< A function to set data to clipboard.
using set_mime_data_func_t = std::function<void(std::unique_ptr<QMimeData>)>;

/**
 * @brief Returns a function to retrieve clipboard data.
 */
get_mime_data_func_t DefaultClipboardGetFunc();

/**
 * @brief Returns a function to set clipboard data.
 */
set_mime_data_func_t DefaultClipboardSetFunc();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_COLOR_PALETTE_H_
