/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "workspace_editor.h"

#include "attribute_editor_actions.h"

#include <sequencergui/composer/attribute_editor_action_handler.h>
#include <sequencergui/composer/workspace_editor_action_handler.h>
#include <sequencergui/composer/workspace_editor_context.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/operation/workspace_view_component_provider.h>
#include <sequencergui/viewmodel/workspace_editor_viewmodel.h>
#include <sequencergui/viewmodel/workspace_operation_viewmodel.h>
#include <sequencergui/views/composer/workspace_editor_actions.h>
#include <sequencergui/views/editors/anyvalue_editor_dialog_factory.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/widgets/message_helper.h>

#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QMimeData>
#include <QTreeView>

namespace sequencergui
{

WorkspaceEditor::WorkspaceEditor(WorkspacePresentationType presentation, QTreeView *tree,
                                 QWidget *parent)
    : QObject(parent)
    , m_tree_view(tree)
    , m_component_provider(CreateProvider(presentation))
    , m_action_handler(new WorkspaceEditorActionHandler(CreateWorkspaceEditorContext(), this))
    , m_editor_actions(new WorkspaceEditorActions(this))
    , m_attribute_action_handler(
          new AttributeEditorActionHandler(CreateAttributeEditorContext(), this))
    , m_attribute_actions(new AttributeEditorActions(m_attribute_action_handler, this))
{
  SetupConnections();

  m_editor_actions->RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(parent));
}

WorkspaceEditor::~WorkspaceEditor() = default;

void WorkspaceEditor::SetWorkspaceItem(WorkspaceItem *workspace)
{
  m_workspace_item = workspace;
  m_component_provider->SetItem(workspace);
}

mvvm::SessionItem *WorkspaceEditor::GetSelectedItem() const
{
  return m_component_provider->GetSelectedItem();
}

QList<QAction *> WorkspaceEditor::GetToolBarActions() const
{
  auto editor_toolbar_actions = m_editor_actions->GetActions(
      {WorkspaceEditorActions::ActionKey::kAdd, WorkspaceEditorActions::ActionKey::kRemove});
  auto attribute_toolbar_actions =
      m_attribute_actions->GetActions({AttributeEditorActions::ActionKey::kModifyAttribute,
                                       AttributeEditorActions::ActionKey::kEditAnyValue});
  return editor_toolbar_actions + attribute_toolbar_actions;
}

void WorkspaceEditor::SetFilterPattern(const QString &pattern)
{
  m_component_provider->SetFilterPattern(pattern);
}

void WorkspaceEditor::SetupContextMenu(QMenu &menu)
{
  // populate cut/copy/paste menu
  m_editor_actions->SetupMenu(menu, m_action_handler);

  // populate attribute menu
  menu.addSeparator();
  auto attribute_menu = menu.addMenu("Modify attribute");
  m_attribute_actions->SetupMenu(*attribute_menu);
}

std::unique_ptr<WorkspaceViewComponentProvider> WorkspaceEditor::CreateProvider(
    WorkspacePresentationType presentation) const
{
  std::unique_ptr<WorkspaceViewComponentProvider> result;

  if (presentation == WorkspacePresentationType::kWorkspaceTree)
  {
    result = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceEditorViewModel>(nullptr), m_tree_view);
  }
  else if (presentation == WorkspacePresentationType::kWorkspaceTechTree)
  {
    result = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceEditorViewModel>(nullptr, /*show_hidded*/ true), m_tree_view);
  }
  else if (presentation == WorkspacePresentationType::kWorkspaceTable)
  {
    result = std::make_unique<WorkspaceViewComponentProvider>(
        std::make_unique<WorkspaceOperationViewModel>(nullptr), m_tree_view);
  }
  else
  {
    throw RuntimeException("Unknown presentation");
  }
  return result;
}

void WorkspaceEditor::SetupConnections()
{
  // propagate variable related requests from WorkspaceEditorActions to WorkspaceEditorActionHandler
  connect(m_editor_actions, &WorkspaceEditorActions::AddVariableRequest, m_action_handler,
          [this](auto str) { m_action_handler->OnAddVariableRequest(str.toStdString()); });
  connect(m_editor_actions, &WorkspaceEditorActions::RemoveVariableRequest, m_action_handler,
          &WorkspaceEditorActionHandler::OnRemoveVariableRequest);
  connect(m_editor_actions, &WorkspaceEditorActions::CutRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Cut);
  connect(m_editor_actions, &WorkspaceEditorActions::CopyRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Copy);
  connect(m_editor_actions, &WorkspaceEditorActions::PasteRequest, m_action_handler,
          &WorkspaceEditorActionHandler::Paste);

  connect(m_attribute_actions, &AttributeEditorActions::EditAnyvalueRequest, m_action_handler,
          &WorkspaceEditorActionHandler::OnEditAnyValueRequest);

  // make inserted item selected, and tree branch expanded
  auto on_select_variable_request = [this](auto item)
  {
    m_component_provider->SetSelectedItem(item);

    auto index_of_inserted = m_component_provider->GetViewIndexes(item);
    if (!index_of_inserted.empty())
    {
      m_tree_view->setExpanded(index_of_inserted.front(), true);
    }
  };
  connect(m_action_handler, &WorkspaceEditorActionHandler::SelectItemRequest, this,
          on_select_variable_request);
}

WorkspaceEditorContext WorkspaceEditor::CreateWorkspaceEditorContext()
{
  WorkspaceEditorContext result;

  auto selected_workspace_callback = [this]() { return m_workspace_item; };
  result.selected_workspace_callback = selected_workspace_callback;

  result.selected_item_callback = [this]() { return GetSelectedItem(); };

  auto send_message_callback = [](const auto &event) { sup::gui::SendWarningMessage(event); };
  result.send_message_callback = send_message_callback;

  result.edit_anyvalue_callback = CreateAnyValueDialogCallback(nullptr);

  result.get_mime_data = []() { return QGuiApplication::clipboard()->mimeData(); };

  result.set_mime_data = [](std::unique_ptr<QMimeData> data)
  { QGuiApplication::clipboard()->setMimeData(data.release()); };

  return result;
}

AttributeEditorContext WorkspaceEditor::CreateAttributeEditorContext()
{
  return {[this]() { return m_component_provider->GetSelectedItem(); }};
}

}  // namespace sequencergui
