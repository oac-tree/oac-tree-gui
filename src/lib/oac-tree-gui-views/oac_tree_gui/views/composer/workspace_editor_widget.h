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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_

#include <oac_tree_gui/model/component_types.h>

#include <QWidget>

class QMenu;

namespace sup::gui
{
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class WorkspaceItem;
class WorkspaceEditor;
class WorkspaceVariableTreeView;

/**
 * @brief The WorkspaceEditorWidget class is intended for editing of workspace variables.
 *
 * It is located at the right of SequencerComposerView (second main view). Defines set of actions to
 * add and remove variables.
 */
class WorkspaceEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceEditorWidget(sup::gui::IAppCommandService& command_service,
                                 WorkspacePresentationType presentation,
                                 QWidget* parent_widget = nullptr);
  ~WorkspaceEditorWidget() override;

  WorkspaceEditorWidget(const WorkspaceEditorWidget&) = delete;
  WorkspaceEditorWidget& operator=(const WorkspaceEditorWidget&) = delete;
  WorkspaceEditorWidget(WorkspaceEditorWidget&&) = delete;
  WorkspaceEditorWidget& operator=(WorkspaceEditorWidget&&) = delete;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  void SetWorkspaceEditType(WorkspaceEditType edit_type);

private:
  void SetupConnections();

  /**
   * @brief Populates the tree context menu with editor actions to modify attributes.
   */
  void OnTreeContextMenuRequest(QMenu& menu);

  WorkspaceVariableTreeView* m_tree_view{nullptr};
  WorkspaceEditor* m_editor{nullptr};

  WorkspaceEditType m_edit_type;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
