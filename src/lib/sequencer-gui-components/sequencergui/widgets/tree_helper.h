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

//! @file
//! Helper methods to deal with Qt trees.

#include <vector>

class QTreeView;
class QModelIndex;

namespace sequencergui
{

/**
 * @brief Scrolls tree viewport to selection.
 *
 * @details Performs scrolling of the tree viewport to show the current selection. The scrolling
 * will be performed only if the current selection is located outside of the viewport. The viewport
 * will be positioned to show selected item at the top.
 */
void ScrollTreeViewportToSelection(QTreeView& tree_view);

/**
 * @brief Returns index of item which should be highlighted instead of the given item.
 *
 * @param tree The tree with some branches collapsed and some expanded.
 * @param child The index of a child which we would like to highlight.
 *
 * @return The actual index of a parent that we have to highlight instead.
 *
 * @details The algorithm is used in the context of highlighting the current running instruction,
 * when it is hidden inside collapsed branch of its parent. In this case we want to highlight the
 * parent itself.
 *
 * @details The algorithm will go up in the hierarchy and look for parent, containing our child in
 * one of collapsed branches. If all branches are expanded, will return child back.
 */
QModelIndex FindVisibleCandidate(const QTreeView& tree, const QModelIndex& child);


/**
 * @brief Adjusts column width so they occupy whole horizontal space available for the tree.
 *
 * @param tree The tree to ajust
 * @param stretch_factors Relative stretch factors
 */
void AdjustWidthOfColumns(QTreeView& tree, std::vector<int> stretch_factors = {});

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_TREE_HELPER_H_
