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

#ifndef SEQUENCERGUI_WIDGETS_TREE_HELPER_H_
#define SEQUENCERGUI_WIDGETS_TREE_HELPER_H_

//! Helper methods to deal with Qt trees.

#include <functional>
#include <memory>

class QMenu;
class QTreeView;
class QPoint;

namespace sequencergui
{

/**
 * @brief Adds collapse/expand section to the QMenu.
 */
void SetupCollapseExpandMenu(const QPoint& point, QMenu& menu, QTreeView& tree_view);

/**
 * @brief Summons collapse/expand menu.
 *
 * @param point Coordinate of context menu as reported by customContextMenuRequested slot.
 * @param tree_vew Tree view which is asking for context menu.
 */
void SummonCollapseExpandMenu(const QPoint& point, QTreeView& tree_view);

/**
 * @brief Creates a callback for given tree, which it can use to process context menu request to
 * from collapse/expand menu.
 */
std::function<void(const QPoint& point)> CreateOnCustomMenuCallback(QTreeView& tree_view);

/**
 * @brief Scrolls tree viewport to selection.
 *
 * @details Performs scrolling of the tree viewport to show the current selection. The scrolling
 * will be performed only if the current selection is located outside of the viewport. The viewport
 * will be positioned to show selected item at the top.
 */
void ScrollTreeViewportToSelection(QTreeView& tree_view);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_TREE_HELPER_H_
