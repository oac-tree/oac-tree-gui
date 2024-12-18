/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "workspace_editor_actions.h"

#include <sequencergui/composer/workspace_editor_action_handler.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/style/style_helper.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/components/proxy_action.h>
#include <sup/gui/widgets/action_menu.h>

#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QToolButton>

namespace sequencergui
{

WorkspaceEditorActions::WorkspaceEditorActions(QObject *parent)
    : QObject(parent), m_add_variable_menu(CreateInsertAfterMenu())
{
  SetupActions();
}

QList<QAction *> WorkspaceEditorActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void WorkspaceEditorActions::SetupMenu(QMenu &menu, WorkspaceEditorActionHandler *handler)
{
  menu.addAction(m_add_variable_action);
  menu.addAction(m_remove_variable_action);
  m_remove_variable_action->setEnabled(handler->CanRemoveVariable());

  menu.addSeparator();
  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_action);

  m_cut_action->setEnabled(handler->CanCut());
  m_copy_action->setEnabled(handler->CanCopy());
  m_paste_action->setEnabled(handler->CanPaste());
}

void WorkspaceEditorActions::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  sup::gui::AppAddActionToCommand(m_cut_action, sup::gui::constants::kCutCommandId, context);
  sup::gui::AppAddActionToCommand(m_copy_action, sup::gui::constants::kCopyCommandId, context);
  sup::gui::AppAddActionToCommand(m_paste_action, sup::gui::constants::kPasteCommandId, context);
}

WorkspaceEditorActions::~WorkspaceEditorActions() = default;

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
  connect(m_remove_variable_action, &QAction::triggered, this,
          &WorkspaceEditorActions::RemoveVariableRequest);

  // remove action (own toolbar version to avoid disabled status)
  m_remove_variable_toolbar_action = new sup::gui::ProxyAction(this);
  m_remove_variable_toolbar_action->SetAction(m_remove_variable_action);
  m_action_map.Add(ActionKey::kRemove, m_remove_variable_toolbar_action);

  m_cut_action = new QAction("Cut", this);
  m_cut_action->setShortcut(QKeySequence::Cut);
  m_cut_action->setToolTip("Cuts selected variable");
  connect(m_cut_action, &QAction::triggered, this, &WorkspaceEditorActions::CutRequest);
  m_action_map.Add(ActionKey::kCut, m_cut_action);

  m_copy_action = new QAction("Copy", this);
  m_copy_action->setShortcut(QKeySequence::Copy);
  m_copy_action->setToolTip("Copies selected variable");
  connect(m_copy_action, &QAction::triggered, this, &WorkspaceEditorActions::CopyRequest);
  m_action_map.Add(ActionKey::kCopy, m_copy_action);

  m_paste_action = new QAction("Paste", this);
  m_paste_action->setShortcut(QKeySequence::Paste);
  m_paste_action->setToolTip("Paste selected variable after current variable");
  connect(m_paste_action, &QAction::triggered, this, &WorkspaceEditorActions::PasteRequest);
  m_action_map.Add(ActionKey::kPaste, m_paste_action);
}

//! Creates menu to insert Variables in a workspace.
std::unique_ptr<QMenu> WorkspaceEditorActions::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainVariableNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit AddVariableRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
