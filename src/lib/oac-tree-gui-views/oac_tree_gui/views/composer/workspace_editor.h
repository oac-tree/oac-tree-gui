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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_

#include <QObject>
#include <memory>

class QAction;
class QMenu;

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class WorkspaceItem;
class WorkspaceEditorContext;
class WorkspaceEditorActionHandler;
class WorkspaceEditorActions;
class AttributeEditorContext;
class AttributeEditorActions;
class AttributeEditorActionHandler;

/**
 * @brief The WorkspaceEditor class contains all logic for editing of workspace variables.
 */
class WorkspaceEditor : public QObject
{
  Q_OBJECT

public:
  explicit WorkspaceEditor(sup::gui::IAppCommandService& command_service, const std::function<std::vector<mvvm::SessionItem*>()>& selected_items,
                           QWidget* parent_widget = nullptr);
  ~WorkspaceEditor() override;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  QList<QAction*> GetToolBarActions() const;

  /**
   * @brief Setups tree context menu with all available actions.
   */
  void SetupContextMenu(QMenu& menu);

signals:
  void ItemSelectRequest(mvvm::SessionItem*);

private:
  void SetupConnections();
  WorkspaceEditorContext CreateWorkspaceEditorContext();
  AttributeEditorContext CreateAttributeEditorContext();

  std::function<std::vector<mvvm::SessionItem*>()> m_get_selected_items;
  WorkspaceItem* m_workspace_item{nullptr};
  std::unique_ptr<WorkspaceEditorActionHandler> m_action_handler;
  WorkspaceEditorActions* m_editor_actions{nullptr};
  std::unique_ptr<AttributeEditorActionHandler> m_attribute_action_handler;
  AttributeEditorActions* m_attribute_actions{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_
