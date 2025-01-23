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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_

#include <oac-tree-gui/components/component_types.h>

#include <QObject>
#include <memory>

class QTreeView;
class QWidget;
class QAction;
class QMenu;

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

class WorkspaceItem;
class WorkspaceEditorContext;
class WorkspaceEditorActionHandler;
class WorkspaceEditorActions;
class AttributeEditorContext;
class AttributeEditorActions;
class AttributeEditorActionHandler;
class WorkspaceViewComponentProvider;

/**
 * @brief The WorkspaceEditor class contains all logic for editing of workspace variables.
 */
class WorkspaceEditor : public QObject
{
public:
  explicit WorkspaceEditor(WorkspacePresentationType presentation, QTreeView* tree,
                           QWidget* parent_widget = nullptr);
  ~WorkspaceEditor() override;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  mvvm::SessionItem* GetSelectedItem() const;

  QList<QAction*> GetToolBarActions() const;

  void SetFilterPattern(const QString& pattern);

  /**
   * @brief Setups tree context menu with all available actions.
   */
  void SetupContextMenu(QMenu& menu);

private:
  /**
   * @brief Creates tree component provider for given type of workspace presentation.
   *
   * Tree will be equipped with the model, generating either variable tree, or variable table.
   */
  std::unique_ptr<WorkspaceViewComponentProvider> CreateProvider(
      WorkspacePresentationType presentation) const;

  void SetupConnections();

  WorkspaceEditorContext CreateWorkspaceEditorContext();

  AttributeEditorContext CreateAttributeEditorContext();

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<WorkspaceViewComponentProvider> m_component_provider;
  WorkspaceItem* m_workspace_item{nullptr};
  WorkspaceEditorActionHandler* m_action_handler{nullptr};
  WorkspaceEditorActions* m_editor_actions{nullptr};
  AttributeEditorActionHandler* m_attribute_action_handler{nullptr};
  AttributeEditorActions* m_attribute_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_H_
