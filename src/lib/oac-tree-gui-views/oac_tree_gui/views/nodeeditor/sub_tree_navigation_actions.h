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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_NAVIGATION_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_NAVIGATION_ACTIONS_H_

#include <QList>
#include <QObject>
#include <memory>
#include <string>

class QAction;
class QMenu;
class QToolButton;
class QWidgetAction;

namespace oac_tree_gui
{

class InstructionItem;
class NodeEditorSceneController;

/**
 * @brief The SubTreeNavigationActions class holds node editor actions to navigate between
 * SubTrees.
 *
 * Back and Forward walk the history of shown SubTrees. The SubTree selector is a tool button with
 * a menu of top-level instructions. The root comes first, in bold. The current SubTree is checked.
 * "Follow selection" is a checkable action, on by default. It controls how the node editor reacts
 * to a selection made in another panel.
 */
class SubTreeNavigationActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param controller The scene controller. Must outlive this object.
   * @param parent_object The parent object.
   */
  explicit SubTreeNavigationActions(NodeEditorSceneController* controller,
                                    QObject* parent_object = nullptr);
  ~SubTreeNavigationActions() override;

  SubTreeNavigationActions(const SubTreeNavigationActions&) = delete;
  SubTreeNavigationActions& operator=(const SubTreeNavigationActions&) = delete;
  SubTreeNavigationActions(SubTreeNavigationActions&&) = delete;
  SubTreeNavigationActions& operator=(SubTreeNavigationActions&&) = delete;

  /**
   * @brief Returns actions for the toolbar.
   */
  QList<QAction*> GetActions() const;

  /**
   * @brief Checks if "Follow selection" is on.
   */
  bool IsFollowSelectionEnabled() const;

  /**
   * @brief Adds actions for the given instruction to a context menu.
   *
   * For an Include or IncludeProcedure: "Open SubTree", disabled with the reason as tooltip if it
   * can't be opened. Nothing for other instructions.
   *
   * @param instruction The instruction under the cursor, or nullptr. Must outlive the menu.
   * @param menu The menu to fill.
   */
  void AddContextMenuActions(const InstructionItem* instruction, QMenu& menu);

private:
  //! Fills the selector menu with top-level instructions.
  void FillSubTreeMenu();

  //! Sets the selector text to the current SubTree name.
  void UpdateSubTreeText();

  //! Switches to the top-level instruction with the given identifier, if it still exists.
  void SetScope(const std::string& identifier);

  //! Enables Back and Forward according to the history.
  void UpdateHistoryActions();

  NodeEditorSceneController* m_controller{nullptr};
  QAction* m_back_action{nullptr};
  QAction* m_forward_action{nullptr};
  std::unique_ptr<QMenu> m_subtree_menu;
  QToolButton* m_subtree_button{nullptr};
  QWidgetAction* m_subtree_action{nullptr};
  QAction* m_follow_selection_action{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_SUB_TREE_NAVIGATION_ACTIONS_H_
