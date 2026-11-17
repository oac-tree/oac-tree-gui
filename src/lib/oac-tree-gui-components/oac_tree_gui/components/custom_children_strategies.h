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

#ifndef OAC_TREE_GUI_COMPONENTS_CUSTOM_CHILDREN_STRATEGIES_H_
#define OAC_TREE_GUI_COMPONENTS_CUSTOM_CHILDREN_STRATEGIES_H_

//! @file
//! Various strategies for instruction and workspace viewmodels.

#include <mvvm/providers/i_children_strategy.h>

#include <set>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The VariableTableChildrenStrategy class reports which items should be in the table
 * representing WorkspaceItem.
 *
 * This strategy is intended to work with WorkspaceOperationViewModel. Scalars and structs with a
 * single scalar field are shown in the variable row. Other structs and arrays are shown as a branch
 * beneath.
 */
class VariableTableChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;
};

/**
 * @brief The InstructionNodeChildrenStrategy class reports children of InstructionItem.
 *
 * It takes into accound possible collapse/expand status of instruction item.
 */
class InstructionNodeChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;
};

/**
 * @brief The ScopedInstructionNodeChildrenStrategy class reports children of InstructionItem,
 * except hidden top-level instructions and their children.
 *
 * Instructions in visible top-level trees are reported as by InstructionNodeChildrenStrategy.
 */
class ScopedInstructionNodeChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param hidden_identifiers Identifiers of hidden top-level instructions. An empty set shows all.
   */
  explicit ScopedInstructionNodeChildrenStrategy(std::set<std::string> hidden_identifiers);

  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;

private:
  //! Returns true if the item is a hidden top-level instruction.
  bool IsHidden(const mvvm::SessionItem& item) const;

  std::set<std::string> m_hidden_identifiers;
  InstructionNodeChildrenStrategy m_instruction_strategy;
};

/**
 * @brief The WorkspaceEditorChildrenStrategy class reports all visible children, with the
 * visible attributes of a variable in place of their container.
 */
class WorkspaceEditorChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;

  mvvm::SessionItem* GetViewParent(mvvm::SessionItem* item) const override;
};

/**
 * @brief The InstructionAttributeChildrenStrategy class reports specific attributes of
 * InstructionItem.
 *
 * Introduced to mainly exclude isRoot attribute from property editor.
 */
class InstructionAttributeChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem*> GetChildren(mvvm::SessionItem* item) const override;

  mvvm::SessionItem* GetViewParent(mvvm::SessionItem* item) const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_CHILDREN_STRATEGIES_H_
