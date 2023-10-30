/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
#define SEQUENCERGUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_

//! @file
//! Various strategies for instruction and workspace viewmodels.

#include <mvvm/interfaces/children_strategy_interface.h>

namespace sequencergui
{

/**
 * @brief The VariableChildrenStrategy class reports which items should be in the tree representing
 * WorkspaceItem.
 *
 * @details It allows all children items to appear in the tree, except PropertyItem representing the
 * name and IsAvailable status.
 */
class VariableChildrenStrategy : public mvvm::ChildrenStrategyInterface
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override;
};

/**
 * @brief The VariableTableChildrenStrategy class reports which items should be in the tree
 * representing WorkspaceItem.
 *
 * @details All variable's properties are hidden, except AnyValueStructItem.
 */
class VariableTableChildrenStrategy : public mvvm::ChildrenStrategyInterface
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_CUSTOM_CHILDREN_STRATEGIES_H_
