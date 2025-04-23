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

#ifndef OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_CONTEXT_H_
#define OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_CONTEXT_H_

#include <functional>
#include <vector>

namespace mvvm
{
class SessionItem;
}

namespace oac_tree_gui
{

/**
 * @brief The AttributeEditorContext is an aggregate with callbacks to query the current
 * state of InstructionAttributeEditor.
 *
 * @details It is necessary to setup InstructionAttributeEditorActions.
 */
struct AttributeEditorContext
{
  std::function<std::vector<mvvm::SessionItem*>()> selected_items_callback;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_CONTEXT_H_
