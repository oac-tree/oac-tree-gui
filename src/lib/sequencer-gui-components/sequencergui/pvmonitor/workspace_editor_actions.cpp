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
    : QObject(parent), m_insert_after_menu(CreateInsertAfterMenu())
{
  SetupActions();
}

QList<QAction *> WorkspaceEditorActions::GetActions() const
{
  return {m_insert_after_action, m_edit_anyvalue_action, m_remove_action};
}

WorkspaceEditorActions::~WorkspaceEditorActions() = default;

void WorkspaceEditorActions::SetupActions()
{
  // We are using QToolButon wrapped into QWidgetAction here because:
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities, so instead we create
  // QToolButton with the menu and wrap it into QWidgetAction

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("Add");
  insert_after_button->setIcon(styleutils::GetIcon("plus-circle-outline"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Insert variable after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);

  auto edit_anyvalue_button = new QToolButton;
  edit_anyvalue_button->setText("Edit");
  edit_anyvalue_button->setIcon(styleutils::GetIcon("file-tree-outline"));
  edit_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  edit_anyvalue_button->setToolTip("Edit value of currently selected variable");
  connect(edit_anyvalue_button, &QToolButton::clicked, this,
          &WorkspaceEditorActions::EditAnyvalueRequest);
  m_edit_anyvalue_action = new QWidgetAction(this);
  m_edit_anyvalue_action->setDefaultWidget(edit_anyvalue_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_button->setToolTip("Remove currently selected variable");
  connect(remove_button, &QToolButton::clicked, this,
          &WorkspaceEditorActions::RemoveSelectedRequest);
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(remove_button);
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
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
