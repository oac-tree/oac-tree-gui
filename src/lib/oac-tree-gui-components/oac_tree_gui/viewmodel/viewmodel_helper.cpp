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

#include "viewmodel_helper.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <stack>

namespace oac_tree_gui
{

void IterateFirstColumn(const QAbstractItemModel &viewmodel, const QModelIndex &parent,
                        const std::function<void(const QModelIndex &)> &func)
{
  struct Node
  {
    QModelIndex parent;
  };

  std::stack<Node> stack;

  stack.push({parent});

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    func(node.parent);

    for (int row = viewmodel.rowCount(node.parent) - 1; row >= 0; --row)
    {
      stack.push({viewmodel.index(row, 0, node.parent)});
    }
  }
}

}  // namespace oac_tree_gui
