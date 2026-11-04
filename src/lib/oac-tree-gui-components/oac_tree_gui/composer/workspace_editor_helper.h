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

#ifndef OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_HELPER_H_
#define OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_HELPER_H_

//! @file
//! Helper functions to edit workspace variables.

#include <cstddef>

namespace oac_tree_gui
{
class WorkspaceItem;
class VariableItem;
}  // namespace oac_tree_gui

namespace oac_tree_gui
{

/**
 * @brief Marks the channel attribute of variables as read-only while the workspace is running.
 *
 * @param is_running Flag denoting if workspace is running.
 * @param item Workspace item to update variables.
 */
void UpdateVariableEditableProperty(bool is_running, WorkspaceItem& item);

/**
 * @brief Provide initial setup of just created variable.
 *
 * @param item Variable item to setup.
 * @param total_variable_count Number of variables already existing in a workspace.
 *
 * @details Provide reasonable default name, and initial AnyValueItem if necessary.
 */
void SetupNewVariable(VariableItem* item, std::size_t total_variable_count = 0);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_WORKSPACE_EDITOR_HELPER_H_
