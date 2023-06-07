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

#include "workspace_editor_widget.h"

#include <sequencergui/components/message_helper.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/anyvalue_editor_dialog.h>
#include <sequencergui/pvmonitor/workspace_editor_action_handler.h>
#include <sequencergui/pvmonitor/workspace_editor_actions.h>
#include <sequencergui/pvmonitor/workspace_editor_context.h>

#include <mvvm/widgets/all_items_tree_view.h>

#include <sup/gui/model/anyvalue_item.h>

#include <QVBoxLayout>

namespace sequencergui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new mvvm::AllItemsTreeView)
    , m_editor_actions(new WorkspaceEditorActions(this))
    , m_action_handler(
          std::make_unique<WorkspaceEditorActionHandler>(CreateWorkspaceEditorContext()))
{
  setWindowTitle("Workspace");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  SetupConnections();
  addActions(m_editor_actions->GetActions());
}

WorkspaceEditorWidget::~WorkspaceEditorWidget() = default;

void WorkspaceEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  m_procedure = procedure;
  m_tree_view->SetItem(procedure ? procedure->GetWorkspace() : nullptr);
}

mvvm::SessionItem *WorkspaceEditorWidget::GetSelectedItem() const
{
  return m_tree_view->GetSelectedItem();
}

void WorkspaceEditorWidget::SetupConnections()
{
  // propagate instruction related requests from WorkspaceEditorWidget to WorkspaceEditorActions
  connect(m_editor_actions, &WorkspaceEditorActions::InsertAfterRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnAddVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::RemoveSelectedRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnRemoveVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::EditAnyvalueRequest, m_action_handler.get(),
          &WorkspaceEditorActionHandler::OnEditAnyvalueRequest);
}

WorkspaceEditorContext WorkspaceEditorWidget::CreateWorkspaceEditorContext()
{
  WorkspaceEditorContext result;

  auto selected_workspace_callback = [this]()
  { return m_procedure ? m_procedure->GetWorkspace() : nullptr; };
  result.selected_workspace_callback = selected_workspace_callback;

  result.selected_item_callback = [this]() { return GetSelectedItem(); };

  auto send_message_callback = [](const auto &event) { SendWarningMessage(event); };
  result.send_message_callback = send_message_callback;

  auto edit_anyvalue_callback =
      [this](const sup::gui::AnyValueItem *item) -> std::unique_ptr<sup::gui::AnyValueItem>
  {
    AnyValueEditorDialog dialog(this);
    dialog.SetInitialValue(item);
    if (dialog.exec() == QDialog::Accepted)
    {
      return dialog.GetResult();
    }
    return {};
  };
  result.edit_anyvalue_callback = edit_anyvalue_callback;

  return result;
}

}  // namespace sequencergui
