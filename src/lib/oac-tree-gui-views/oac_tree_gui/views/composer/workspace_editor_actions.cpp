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

#include "workspace_editor_actions.h"

#include <oac_tree_gui/composer/i_workspace_editor_action_handler.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/component_objects/proxy_action.h>
#include <sup/gui/widgets/action_menu.h>

#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QToolButton>

namespace oac_tree_gui
{

WorkspaceEditorActions::WorkspaceEditorActions(IWorkspaceEditorActionHandler* handler,
                                               QObject* parent_object)
    : QObject(parent_object)
    , m_action_handler(handler)
    , m_add_variable_menu(CreateInsertAfterMenu())
{
  SetupActions();
}

WorkspaceEditorActions::~WorkspaceEditorActions() = default;

QList<QAction*> WorkspaceEditorActions::GetActions(const std::vector<ActionKey>& action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void WorkspaceEditorActions::SetupMenu(QMenu& menu)
{
  menu.addAction(m_add_variable_action);
  menu.addAction(m_remove_variable_action);
  m_remove_variable_action->setEnabled(m_action_handler->CanRemoveVariable());

  menu.addSeparator();
  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_action);

  m_cut_action->setEnabled(m_action_handler->CanCut());
  m_copy_action->setEnabled(m_action_handler->CanCopy());
  m_paste_action->setEnabled(m_action_handler->CanPaste());
}

void WorkspaceEditorActions::RegisterActionsForContext(
    const sup::gui::AppCommandContext& context, sup::gui::IAppCommandService& command_service)
{
  command_service.AddActionToCommand(m_cut_action, sup::gui::constants::kCutCommandId, context);
  command_service.AddActionToCommand(m_copy_action, sup::gui::constants::kCopyCommandId, context);
  command_service.AddActionToCommand(m_paste_action, sup::gui::constants::kPasteCommandId, context);
  command_service.AddActionToCommand(m_remove_variable_action,
                                     sup::gui::constants::kRemoveSelectedCommandId, context);
}

void WorkspaceEditorActions::SetupActions()
{
  m_add_variable_action = new sup::gui::ActionMenu("Add variable", this);
  m_add_variable_action->setIcon(FindIcon("plus-circle-outline"));
  m_add_variable_action->setMenu(m_add_variable_menu.get());
  m_add_variable_action->setToolTip(
      "Add sequencer variable to the workspace\n\n"
      "- If existing variable is selected, new variable\n"
      "  will be inserted after");
  m_action_map.Add(ActionKey::kAdd, m_add_variable_action);

  m_remove_variable_action = new sup::gui::ActionMenu("Remove variable", this);
  m_remove_variable_action->setIcon(FindIcon("beaker-remove-outline"));
  m_remove_variable_action->setToolTip("Remove currently selected variable");
  // Shortcut are here only to provide visual helper in the context menu. They are not triggered
  // since m_remove_action is not attached to any particular widget. Real shortcuts are handled by
  // global proxy actions.
  m_remove_variable_action->setShortcuts({Qt::Key_Delete, Qt::Key_Backspace});
  connect(m_remove_variable_action, &QAction::triggered, this,
          [this]() { m_action_handler->RemoveVariable(); });

  // remove action (own toolbar version to avoid disabled status)
  m_remove_variable_toolbar_action = new sup::gui::ProxyAction(this);
  m_remove_variable_toolbar_action->SetAction(m_remove_variable_action);
  m_action_map.Add(ActionKey::kRemove, m_remove_variable_toolbar_action);

  m_cut_action = new QAction("Cut", this);
  m_cut_action->setShortcut(QKeySequence::Cut);
  m_cut_action->setToolTip("Cuts selected variable");
  connect(m_cut_action, &QAction::triggered, this, [this]() { m_action_handler->Cut(); });
  m_action_map.Add(ActionKey::kCut, m_cut_action);

  m_copy_action = new QAction("Copy", this);
  m_copy_action->setShortcut(QKeySequence::Copy);
  m_copy_action->setToolTip("Copies selected variable");
  connect(m_copy_action, &QAction::triggered, this, [this]() { m_action_handler->Copy(); });
  m_action_map.Add(ActionKey::kCopy, m_copy_action);

  m_paste_action = new QAction("Paste", this);
  m_paste_action->setShortcut(QKeySequence::Paste);
  m_paste_action->setToolTip("Paste selected variable after current variable");
  connect(m_paste_action, &QAction::triggered, this, [this]() { m_action_handler->Paste(); });
  m_action_map.Add(ActionKey::kPaste, m_paste_action);
}

//! Creates menu to insert Variables in a workspace.
std::unique_ptr<QMenu> WorkspaceEditorActions::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  for (const auto& name : oac_tree_gui::GetDomainVariableNames())
  {
    auto action = result->addAction(QString::fromStdString(name));
    auto on_action = [this, name]() { m_action_handler->AddVariable(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace oac_tree_gui
