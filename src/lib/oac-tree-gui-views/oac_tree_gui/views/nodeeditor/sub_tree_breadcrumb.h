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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_BREADCRUMB_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_BREADCRUMB_H_

#include <QWidget>

class QLabel;
class QMenu;
class QToolButton;

namespace oac_tree_gui
{

class NodeEditorSceneController;

/**
 * @brief The SubTreeBreadcrumb class shows the trail of SubTrees entered through Includes.
 *
 * For example "Main › dispatch → Dispatch › init → DoInit", where "dispatch" and "init" are
 * Includes. The trail starts at the last SubTree chosen otherwise. A click on an earlier SubTree
 * goes back to it. The "Included by" menu lists Includes pointing at the current SubTree, grouped
 * by their top-level instruction. A click shows and selects the Include.
 */
class SubTreeBreadcrumb : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param controller The scene controller. Must outlive this object.
   * @param parent_widget The parent widget.
   */
  explicit SubTreeBreadcrumb(NodeEditorSceneController* controller,
                             QWidget* parent_widget = nullptr);
  ~SubTreeBreadcrumb() override;

  SubTreeBreadcrumb(const SubTreeBreadcrumb&) = delete;
  SubTreeBreadcrumb& operator=(const SubTreeBreadcrumb&) = delete;
  SubTreeBreadcrumb(SubTreeBreadcrumb&&) = delete;
  SubTreeBreadcrumb& operator=(SubTreeBreadcrumb&&) = delete;

  /**
   * @brief Returns the trail as rich text. A link holds the history index.
   */
  QString GetText() const;

private:
  //! Builds the trail from the controller's history.
  void UpdateText();

  //! Fills the "Included by" menu for the current SubTree.
  void FillIncludedByMenu();

  NodeEditorSceneController* m_controller{nullptr};
  QLabel* m_label{nullptr};
  QToolButton* m_included_by_button{nullptr};
  QMenu* m_included_by_menu{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_BREADCRUMB_H_
