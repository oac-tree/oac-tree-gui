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

#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

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

WorkspaceEditorActions::~WorkspaceEditorActions() = default;

void WorkspaceEditorActions::SetupActions()
{
  m_add_variable_action = new sup::gui::ActionMenu(this);
  m_add_variable_action->setText("Add variable");
  m_add_variable_action->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  m_add_variable_action->setMenu(m_add_variable_menu.get());
  m_add_variable_action->setToolTip(
      "Add sequencer variable to the workspace\n\n"
      "- If existing variable is selected, new variable\n"
      "  will be inserted after");
  m_action_map.Add(ActionKey::kAdd, m_add_variable_action);

  m_remove_variable_action = new QAction(this);
  m_remove_variable_action->setText("Remove variable");
  m_remove_variable_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_variable_action->setToolTip("Remove currently selected variable");
  connect(m_remove_variable_action, &QAction::triggered, this,
          &WorkspaceEditorActions::RemoveVariableRequest);
  m_action_map.Add(ActionKey::kRemove, m_remove_variable_action);
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
