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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_WORKSPACE_PANEL_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_WORKSPACE_PANEL_H_

#include <QWidget>

namespace sup::gui
{
class ItemStackWidget;
}

namespace sup::gui
{
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;
class WorkspaceEditorWidget;
class NodeEditorWidget;

/**
 * @brief The OperationWorkspacePanel class represents vertical panel on the right part of
 * OperationMonitorView with Workspace variables.
 *
 * It is a stack of two widgets with possibility to choose either a workspace tree or workspace
 * table.
 */
class OperationWorkspacePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationWorkspacePanel(sup::gui::IAppCommandService& command_service,
                                   QWidget* parent_widget = nullptr);
  ~OperationWorkspacePanel() override;

  OperationWorkspacePanel(const OperationWorkspacePanel&) = delete;
  OperationWorkspacePanel& operator=(const OperationWorkspacePanel&) = delete;
  OperationWorkspacePanel(OperationWorkspacePanel&&) = delete;
  OperationWorkspacePanel& operator=(OperationWorkspacePanel&&) = delete;

  void SetProcedure(ProcedureItem* item);

private:
  void ReadSettings();
  void WriteSettings();

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_tree_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_table_widget{nullptr};
  NodeEditorWidget* m_node_editor_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_WORKSPACE_PANEL_H_
