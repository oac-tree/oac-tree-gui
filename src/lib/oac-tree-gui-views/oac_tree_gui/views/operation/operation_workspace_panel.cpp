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

#include "operation_workspace_panel.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/views/composer/workspace_editor_widget.h>

#include <sup/gui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace
{

const QString kWorkspaceSettingsKey = "OperationWorkspacePanel/stack_widget";

}  // namespace

namespace oac_tree_gui
{

OperationWorkspacePanel::OperationWorkspacePanel(sup::gui::IAppCommandService &command_service,
                                                 QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_stack_widget(new sup::gui::ItemStackWidget(kWorkspaceSettingsKey))
    , m_workspace_tree_widget(
          new WorkspaceEditorWidget(command_service, WorkspacePresentationType::kWorkspaceTree))
    , m_workspace_table_widget(
          new WorkspaceEditorWidget(command_service, WorkspacePresentationType::kWorkspaceTable))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_workspace_tree_widget->setWindowTitle("Workspace Tree");
  m_workspace_table_widget->setWindowTitle("Workspace Table");

  m_stack_widget->AddWidget(m_workspace_tree_widget);
  m_stack_widget->AddWidget(m_workspace_table_widget);

  layout->addWidget(m_stack_widget);

  ReadSettings();
}

OperationWorkspacePanel::~OperationWorkspacePanel()
{
  WriteSettings();
}

void OperationWorkspacePanel::SetProcedure(ProcedureItem *item)
{
  auto workspace_item = item ? item->GetWorkspace() : nullptr;
  m_workspace_tree_widget->SetWorkspaceItem(workspace_item);
  m_workspace_table_widget->SetWorkspaceItem(workspace_item);
}

void OperationWorkspacePanel::ReadSettings()
{
  m_stack_widget->ReadSettings(sup::gui::GetSettingsReadFunc());
}

void OperationWorkspacePanel::WriteSettings()
{
  m_stack_widget->WriteSettings(sup::gui::GetSettingsWriteFunc());
}

}  // namespace oac_tree_gui
