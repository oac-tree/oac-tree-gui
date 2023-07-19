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

#include "workspace_editor_actions.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

WorkspaceEditorActions::WorkspaceEditorActions(QObject *parent)
    : QObject(parent), m_add_variable_menu(CreateInsertAfterMenu())
{
  SetupActions();
}

QList<QAction *> WorkspaceEditorActions::GetActions() const
{
  return {m_add_variable_action, m_edit_anyvalue_action, m_remove_variable_action};
}

WorkspaceEditorActions::~WorkspaceEditorActions() = default;

void WorkspaceEditorActions::SetupActions()
{
  // We are using QToolButon wrapped into QWidgetAction here because:
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities, so instead we create
  // QToolButton with the menu and wrap it into QWidgetAction

  auto add_variable_button = new QToolButton;
  add_variable_button->setText("Add");
  add_variable_button->setIcon(styleutils::GetIcon("plus-circle-outline"));
  add_variable_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  add_variable_button->setPopupMode(QToolButton::InstantPopup);
  add_variable_button->setMenu(m_add_variable_menu.get());
  add_variable_button->setToolTip(
      "Add sequencer variable to the workspace\n\n"
      "- If existing variable is selected, new variable\n"
      "  will be inserted after");
  m_add_variable_action = new QWidgetAction(this);
  m_add_variable_action->setDefaultWidget(add_variable_button);

  auto edit_anyvalue_button = new QToolButton;
  edit_anyvalue_button->setText("Edit");
  edit_anyvalue_button->setIcon(styleutils::GetIcon("file-tree-outline"));
  edit_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  edit_anyvalue_button->setToolTip("Edit value of currently selected variable");
  connect(edit_anyvalue_button, &QToolButton::clicked, this,
          &WorkspaceEditorActions::EditAnyvalueRequest);
  m_edit_anyvalue_action = new QWidgetAction(this);
  m_edit_anyvalue_action->setDefaultWidget(edit_anyvalue_button);

  auto remove_variable_button = new QToolButton;
  remove_variable_button->setText("Remove");
  remove_variable_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_variable_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_variable_button->setToolTip("Remove currently selected variable");
  connect(remove_variable_button, &QToolButton::clicked, this,
          &WorkspaceEditorActions::RemoveVariableRequest);
  m_remove_variable_action = new QWidgetAction(this);
  m_remove_variable_action->setDefaultWidget(remove_variable_button);
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
