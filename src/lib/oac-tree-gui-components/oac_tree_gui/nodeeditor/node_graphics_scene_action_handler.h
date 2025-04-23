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

#ifndef OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_ACTION_HANDLER_H_
#define OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_ACTION_HANDLER_H_

#include <oac_tree_gui/composer/instruction_editor_action_handler.h>

namespace oac_tree_gui
{

class NodeGraphicsSceneActionHandler : public InstructionEditorActionHandler
{
public:
  explicit NodeGraphicsSceneActionHandler(InstructionEditorContext context);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_NODE_GRAPHICS_SCENE_ACTION_HANDLER_H_
