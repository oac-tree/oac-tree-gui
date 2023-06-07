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

#include "instruction_editor_actions.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

InstructionEditorActions::InstructionEditorActions(QObject *parent)
    : QObject(parent)
    , m_insert_into_menu(CreateInsertIntoMenu())
    , m_insert_after_menu(CreateInsertAfterMenu())
{
  SetupActions();
}

InstructionEditorActions::~InstructionEditorActions() = default;

QList<QAction *> InstructionEditorActions::GetActions() const
{
  return {m_insert_after_action, m_insert_into_action, m_remove_action};
}

void InstructionEditorActions::SetupActions()
{
  // we are using QToolButon wrapped into QWidgetAction here because
  // 1. we want to pass around QList<QAction*>
  // 2. QAction with menu doesn't provide InstantPopup capabilities

  auto insert_after_button = new QToolButton;
  insert_after_button->setText("Add");
  insert_after_button->setIcon(styleutils::GetIcon("plus-circle-outline"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Add instruction after current selection");
  m_insert_after_action = new QWidgetAction(this);
  m_insert_after_action->setDefaultWidget(insert_after_button);

  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Insert");
  insert_into_button->setIcon(styleutils::GetIcon("plus-circle-multiple-outline"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  m_insert_into_action = new QWidgetAction(this);
  m_insert_into_action->setDefaultWidget(insert_into_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(remove_button, &QToolButton::clicked, this,
          &InstructionEditorActions::RemoveSelectedRequest);
  m_remove_action = new QWidgetAction(this);
  m_remove_action->setDefaultWidget(remove_button);
}

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

//! Creates menu to insert an instruction into currently selected instruction.
//! Code mostly coincides with the code above. However, this duplication is temporary and it
//! will diverge in the future (idea to disable some actions if an operation is not possible).

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertIntoMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit InsertIntoRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequencergui
