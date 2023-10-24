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

#include "tree_helper.h"

#include <QTreeView>

namespace sequencergui
{

void ScrollTreeViewportToSelection(QTreeView &tree_view)
{
  auto indexes = tree_view.selectionModel()->selectedIndexes();
  if (indexes.empty())
  {
    return;
  }

  auto visible_rectangle = tree_view.visualRect(indexes.front());
  if (visible_rectangle.top() < 0 || visible_rectangle.bottom() > tree_view.rect().bottom())
  {
    tree_view.scrollTo(indexes.front(), QAbstractItemView::PositionAtTop);
  }
}

QModelIndex FindVisibleCandidate(const QTreeView &tree, const QModelIndex &child)
{
  if (!child.isValid())
  {
    return {};
  }
  QModelIndex result = child;

  QModelIndex current = child;
  while (current.isValid())
  {
    const int row_count = tree.model()->rowCount(current);
    if (!tree.isExpanded(current) && row_count > 0)
    {
      result = current;
    }

    current = current.parent();
  }

  return result;
}

}  // namespace sequencergui
