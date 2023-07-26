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

  // expand all
  auto expand_all_action = menu.addAction("Expand all");
  QObject::connect(expand_all_action, &QAction::triggered, &tree_view, &QTreeView::expandAll);

  // expand to depth
  auto expand_to_depth_menu = menu.addMenu("Expand all to depth");
  const int max_depth_level = 5;
  for (int depth = 0; depth < max_depth_level; ++depth)
  {
    auto action = expand_to_depth_menu->addAction(QString("depth %1").arg(depth + 1));
    auto on_action = [&tree_view, depth]() { tree_view.expandToDepth(depth); };
    QObject::connect(action, &QAction::triggered, &tree_view, on_action);
  }

  // collapse all
  auto collapse_all_action = menu.addAction("Collapse all");
  QObject::connect(collapse_all_action, &QAction::triggered, &tree_view, &QTreeView::collapseAll);

  menu.addSeparator();

  // expand selected
  auto expand_selected_action = menu.addAction("Expand selected");
  auto on_expand_selected = [&tree_view, &point]() { tree_view.expand(tree_view.indexAt(point)); };
  QObject::connect(expand_selected_action, &QAction::triggered, &tree_view, on_expand_selected);

  // expand selected depth

  // FEATURE Current behavior of "Expand all to depth" and "Expand selected to depth" differs for
  // some reason. "Expand all to depth" works correctly in both directions. "Expand selected to
  // depth" doesn't work when a tree is expanded to level 2, and we want to go to level 1, for
  // example. I think it's because of the difference in QTreeView::expandToDepth and
  // QTreeView::expandRecursively which is used below.

  auto expand_selected_to_depth_menu = menu.addMenu("Expand selected to depth");
  for (int depth = 0; depth < max_depth_level; ++depth)
  {
    auto action = expand_selected_to_depth_menu->addAction(QString("depth %1").arg(depth + 1));
    auto on_action = [&tree_view, depth, point]()
    { tree_view.expandRecursively(tree_view.indexAt(point), depth); };
    QObject::connect(action, &QAction::triggered, &tree_view, on_action);
  }

  // collapse selected
  auto collapse_selected_action = menu.addAction("Collapse selected");
  auto on_collapse_selected = [&tree_view, point]()
  { tree_view.collapse(tree_view.indexAt(point)); };
  QObject::connect(collapse_selected_action, &QAction::triggered, &tree_view, on_collapse_selected);
}

void SummonCollapseExpandMenu(const QPoint &point, QTreeView &tree_view)
{
  QMenu menu;
  SetupCollapseExpandMenu(point, menu, tree_view);
  menu.exec(tree_view.mapToGlobal(point));
}

std::function<void(const QPoint &)> CreateOnCustomMenuCallback(QTreeView &tree_view)
{
  auto result = [&tree_view](const QPoint &point) { SummonCollapseExpandMenu(point, tree_view); };
  return result;
}

}  // namespace sequencergui
