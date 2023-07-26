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

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QTreeView>

namespace sequencergui
{

void SetupCollapseExpandMenu(const QPoint &point, QMenu &menu, QTreeView &tree_view)
{
  menu.setToolTipsVisible(true);

  auto expand_all_action = menu.addAction("Expand all");
  QObject::connect(expand_all_action, &QAction::triggered, &tree_view, &QTreeView::expandAll);

  auto expand_to_depth_menu = menu.addMenu("Expand all to depth");
  const int max_depth_level = 5;
  for (int depth = 0; depth < max_depth_level; ++depth)
  {
    auto action = expand_to_depth_menu->addAction(QString("depth %1").arg(depth+1));
    QObject::connect(action, &QAction::triggered,
                     [&tree_view, depth]() { tree_view.expandToDepth(depth); });
  }

  auto collapse_all_action = menu.addAction("Collapse all");
  QObject::connect(collapse_all_action, &QAction::triggered, &tree_view, &QTreeView::collapseAll);
}

}  // namespace sequencergui
