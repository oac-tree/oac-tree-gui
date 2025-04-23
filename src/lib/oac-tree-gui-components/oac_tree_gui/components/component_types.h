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

#ifndef OAC_TREE_GUI_COMPONENTS_COMPONENT_TYPES_H_
#define OAC_TREE_GUI_COMPONENTS_COMPONENT_TYPES_H_

//! @file
//! Common types for application widgets.

#include <cstdint>

namespace oac_tree_gui
{

/**
 * @brief The BehaviorTreeStyle enum defines presentation of behavior tree in the node editor.
 */
enum class BehaviorTreeStyle : std::uint8_t
{
  kTopToBottom,
  kLeftToRight
};

/**
 * @brief The WorkspacePresentationType enum defines how variables are presented in workspace
 * editor.
 */
enum class WorkspacePresentationType : std::uint8_t
{
  kWorkspaceTree,      //!< full tree with workspace variables, connection status hidded
  kWorkspaceTechTree,  //!< full tree with workspace variables, connection status shown
  kWorkspaceTable      //!< experimental table with workspace variables
};

/**
 * @brief The OperationPresentationMode enum defines if widget is a part of full-fledged
 * sequencer-gui or operation-gui.
 */
enum class OperationPresentationMode : std::uint8_t
{
  kOperationMode,  //!< widget is a part of sequencer-operation application
  kIdeMode         //!< widget is a part of sequencer-gui
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_COMPONENT_TYPES_H_
