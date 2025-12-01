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

#ifndef OAC_TREE_GUI_MAINWINDOW_CONTEXT_HELPER_H_
#define OAC_TREE_GUI_MAINWINDOW_CONTEXT_HELPER_H_

//! @file
//! Collection of helper functions to querry main window context.

#include <vector>

namespace oac_tree_gui
{

struct ObjectGroupInfo;

/**
 * @brief Creates a collection of global registered sequencer instruction types grouped according to
 * a plugin name.
 *
 * This method uses global object type registry. A single group can contain instructions from
 * several plugins.
 *
 * @param type_registry The registry with all loaded types.
 */
std::vector<ObjectGroupInfo> CreateInstructionTypeGroups();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_CONTEXT_HELPER_H_
