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

#include "workspace_editor.h"

#include "attribute_editor_actions.h"

#include <oac_tree_gui/composer/attribute_editor_action_handler.h>
#include <oac_tree_gui/composer/workspace_editor_action_handler.h>
#include <oac_tree_gui/composer/workspace_editor_context.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/views/composer/workspace_editor_actions.h>
#include <oac_tree_gui/views/editors/anyvalue_editor_dialog_factory.h>

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/mainwindow/clipboard_helper.h>
#include <sup/gui/widgets/message_helper.h>

#include <QMenu>

namespace oac_tree_gui
{

WorkspaceEditor::WorkspaceEditor(
    const std::function<std::vector<mvvm::SessionItem *>()> &selected_items, QWidget *parent_widget)
    : QObject(parent_widget)
    , m_get_selected_items(selected_items)
    , m_action_handler(
          std::make_unique<WorkspaceEditorActionHandler>(CreateWorkspaceEditorContext()))
    , m_editor_actions(new WorkspaceEditorActions(m_action_handler.get(), this))
    , m_attribute_action_handler(
          std::make_unique<AttributeEditorActionHandler>(CreateAttributeEditorContext()))
    , m_attribute_actions(new AttributeEditorActions(m_attribute_action_handler.get(), this))
{
  SetupConnections();

  m_editor_actions->RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(parent_widget));
}

WorkspaceEditor::~WorkspaceEditor() = default;

void WorkspaceEditor::SetWorkspaceItem(WorkspaceItem *workspace)
{
  m_workspace_item = workspace;
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

void WorkspaceEditor::SetupContextMenu(QMenu &menu)
{
  // populate cut/copy/paste menu
  m_editor_actions->SetupMenu(menu);

  // populate attribute menu
  menu.addSeparator();
  auto attribute_menu = menu.addMenu("Modify attribute");
  m_attribute_actions->SetupMenu(*attribute_menu);
}

void WorkspaceEditor::SetupConnections()
{
  connect(m_attribute_actions, &AttributeEditorActions::EditAnyvalueRequest, this,
          [this]() { m_action_handler->EditAnyValue(); });
}

WorkspaceEditorContext WorkspaceEditor::CreateWorkspaceEditorContext()
{
  WorkspaceEditorContext result;

  auto selected_workspace_callback = [this]() { return m_workspace_item; };
  result.selected_workspace = selected_workspace_callback;
  result.selected_items_callback = m_get_selected_items;
  result.notify_request = [this](auto item) { emit ItemSelectRequest(item); };

  auto send_message_callback = [](const auto &event) { sup::gui::SendWarningMessage(event); };
  result.send_message = send_message_callback;

  result.edit_anyvalue = CreateAnyValueDialogCallback(nullptr);
  result.get_mime_data = sup::gui::DefaultClipboardGetFunc();
  result.set_mime_data = sup::gui::DefaultClipboardSetFunc();

  return result;
}

AttributeEditorContext WorkspaceEditor::CreateAttributeEditorContext()
{
  return {m_get_selected_items};
}

}  // namespace oac_tree_gui
