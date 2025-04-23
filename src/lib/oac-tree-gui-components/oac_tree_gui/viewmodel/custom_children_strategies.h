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

#ifndef OAC_TREE_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
#define OAC_TREE_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_

//! @file
//! Various strategies for instruction and workspace viewmodels.

#include <mvvm/viewmodel/i_children_strategy.h>

namespace oac_tree_gui
{

/**
 * @brief The VariableTableChildrenStrategy class reports which items should be in the tree
 * representing WorkspaceItem.
 *
 * All variable's properties are hidden, except AnyValueStructItem.
 */
class VariableTableChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
