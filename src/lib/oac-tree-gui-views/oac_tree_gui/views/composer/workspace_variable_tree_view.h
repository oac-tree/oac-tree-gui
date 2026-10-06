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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_VARIABLE_TREE_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_VARIABLE_TREE_VIEW_H_

#include <oac_tree_gui/model/component_types.h>

#include <QWidget>
#include <functional>
#include <memory>
#include <vector>

class QTreeView;
class QLineEdit;
class QMenu;

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{
class ISettingsManager;
class SettingsHeaderView;
class VisibilityAgentBase;
}  // namespace sup::gui

namespace oac_tree_gui
{

class WorkspaceItem;
class WorkspaceViewComponentProvider;

/**
 * @brief The WorkspaceVariableTreeView class shows workspace variables as a tree (or a table)
 * without any editing capabilities of its own.
 *
 * It owns the tree, the component provider generating either a variable tree or a variable table
 * (depending on the given WorkspacePresentationType), the filtering line edit, and the visibility
 * agent used to (un)subscribe from model notifications. Editing logic, if any, is provided by the
 * owner via a context-menu callback and by toggling edit triggers with SetEditable().
 */
class WorkspaceVariableTreeView : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Tree and table presentations store column widths in different settings groups. The settings
   * manager must outlive the view.
   */
  WorkspaceVariableTreeView(WorkspacePresentationType presentation,
                            sup::gui::ISettingsManager& settings_manager,
                            QWidget* parent_widget = nullptr);
  ~WorkspaceVariableTreeView() override;

  WorkspaceVariableTreeView(const WorkspaceVariableTreeView&) = delete;
  WorkspaceVariableTreeView& operator=(const WorkspaceVariableTreeView&) = delete;
  WorkspaceVariableTreeView(WorkspaceVariableTreeView&&) = delete;
  WorkspaceVariableTreeView& operator=(WorkspaceVariableTreeView&&) = delete;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  /**
   * @brief Enables or disables in-place editing of variables in the tree.
   */
  void SetEditable(bool value);

  /**
   * @brief Returns items currently selected in the tree.
   */
  std::vector<mvvm::SessionItem*> GetSelectedItems() const;

  /**
   * @brief Selects given item in the tree and expands it.
   */
  void SelectItem(const mvvm::SessionItem* item);

  /**
   * @brief Sets a callback allowing the owner to populate the tree context menu.
   *
   * The callback is invoked before the built-in "Tree settings" submenu is added.
   */
  void SetContextMenuCallback(std::function<void(QMenu&)> callback);

protected:
  void resizeEvent(QResizeEvent* event) override;

private:
  void SetupTree();
  void AdjustTreeAppearance();

  /**
   * @brief Creates tree component provider for given type of workspace presentation.
   *
   * Tree will be equipped with the model, generating either variable tree, or variable table.
   */
  std::unique_ptr<WorkspaceViewComponentProvider> CreateProvider(
      WorkspacePresentationType presentation) const;

  /**
   * @brief Summons custom tree context menu.
   */
  void OnTreeContextMenuRequest(const QPoint& point);

  void SetWorkspaceItemIntern(WorkspaceItem* workspace_item);

  QTreeView* m_tree_view{nullptr};
  sup::gui::SettingsHeaderView* m_header{nullptr};
  std::unique_ptr<WorkspaceViewComponentProvider> m_component_provider;
  std::unique_ptr<sup::gui::VisibilityAgentBase> m_visibility_agent;
  QLineEdit* m_line_edit{nullptr};
  std::function<void(QMenu&)> m_context_menu_callback;

  WorkspaceItem* m_workspace_item{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_VARIABLE_TREE_VIEW_H_
