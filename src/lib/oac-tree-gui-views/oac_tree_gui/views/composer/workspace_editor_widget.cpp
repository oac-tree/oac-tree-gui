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

#include "workspace_editor_widget.h"

#include "workspace_editor.h"
#include "workspace_variable_tree_view.h"

#include <mvvm/model/session_item.h>

#include <QMenu>
#include <QVBoxLayout>

namespace oac_tree_gui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(sup::gui::IAppCommandService& command_service,
                                             WorkspacePresentationType presentation,
                                             QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new WorkspaceVariableTreeView(presentation))
    , m_editor(new WorkspaceEditor(
          command_service, [this]() { return m_tree_view->GetSelectedItems(); }, this))
    , m_edit_type(WorkspaceEditType::kEditorEnabled)
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  addActions(m_editor->GetToolBarActions());

  m_tree_view->SetContextMenuCallback([this](QMenu& menu) { OnTreeContextMenuRequest(menu); });

  SetupConnections();
}

WorkspaceEditorWidget::~WorkspaceEditorWidget() = default;

void WorkspaceEditorWidget::SetWorkspaceItem(WorkspaceItem* workspace)
{
  m_editor->SetWorkspaceItem(workspace);
  m_tree_view->SetWorkspaceItem(workspace);
}

void WorkspaceEditorWidget::SetWorkspaceEditType(WorkspaceEditType edit_type)
{
  m_edit_type = edit_type;

  m_tree_view->SetEditable(edit_type != WorkspaceEditType::kReadOnly);

  for (auto action : actions())
  {
    action->setEnabled(edit_type == WorkspaceEditType::kEditorEnabled);
  }
}

void WorkspaceEditorWidget::SetupConnections()
{
  connect(m_editor, &WorkspaceEditor::ItemSelectRequest, m_tree_view,
          &WorkspaceVariableTreeView::SelectItem);
}

void WorkspaceEditorWidget::OnTreeContextMenuRequest(QMenu& menu)
{
  if (m_edit_type == WorkspaceEditType::kEditorEnabled)
  {
    m_editor->SetupContextMenu(menu);
  }
}

}  // namespace oac_tree_gui
