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
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/anyvalue_editor_dialog.h>
#include <sequencergui/pvmonitor/workspace_editor_actions.h>
#include <sequencergui/pvmonitor/workspace_editor_context.h>
#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

WorkspaceEditorWidget::WorkspaceEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_tree_view(new mvvm::AllItemsTreeView)
    , m_workspace_editor_actions(
          std::make_unique<WorkspaceEditorActions>(CreateWorkspaceEditorContext()))
{
  setWindowTitle("WORKSPACE");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  SetupActions();
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

void WorkspaceEditorWidget::SetupActions()
{
  // We are using QToolButon wrapped into QWidgetAction here because:
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities, so instead we create
  // QToolButton with the menu and wrap it into QWidgetAction

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("Add");
  insert_after_button->setIcon(styleutils::GetIcon("plus-circle-outline"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert variable after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);
  addAction(m_insert_after_action);

  auto edit_anyvalue_button = new QToolButton;
  edit_anyvalue_button->setText("Edit");
  edit_anyvalue_button->setIcon(styleutils::GetIcon("file-tree-outline"));
  edit_anyvalue_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  edit_anyvalue_button->setToolTip("Edit value of currently selected variable");
  connect(edit_anyvalue_button, &QToolButton::clicked, this,
          &WorkspaceEditorWidget::EditAnyvalueRequest);
  m_edit_anyvalue_action = new QWidgetAction(this);
  m_edit_anyvalue_action->setDefaultWidget(edit_anyvalue_button);
  addAction(m_edit_anyvalue_action);

  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  remove_button->setToolTip("Remove currently selected variable");
  connect(remove_button, &QToolButton::clicked, this,
          &WorkspaceEditorWidget::RemoveSelectedRequest);
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(remove_button);
  addAction(m_insert_after_action);

  // propagate instruction related requests from WorkspaceEditorWidget to WorkspaceEditorActions
  connect(this, &WorkspaceEditorWidget::InsertAfterRequest, m_workspace_editor_actions.get(),
          &WorkspaceEditorActions::OnAddVariableRequest);
  connect(this, &WorkspaceEditorWidget::RemoveSelectedRequest, m_workspace_editor_actions.get(),
          &WorkspaceEditorActions::OnRemoveVariableRequest);
  connect(this, &WorkspaceEditorWidget::EditAnyvalueRequest, m_workspace_editor_actions.get(),
          &WorkspaceEditorActions::OnEditAnyvalueRequest);
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

//! Creates menu to insert Variables in a workspace.
std::unique_ptr<QMenu> WorkspaceEditorWidget::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainVariableNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
