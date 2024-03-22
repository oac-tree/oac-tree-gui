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

#include "instruction_editor_actions.h"

#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

InstructionEditorActions::InstructionEditorActions(QObject *parent)
    : QObject(parent)
    , m_insert_after_menu(CreateInsertAfterMenu())
    , m_insert_into_menu(CreateInsertIntoMenu())
{
  SetupInsertRemoveActions();
  SetupCutCopyPasteActions();
}

InstructionEditorActions::~InstructionEditorActions() = default;

QList<QAction *> InstructionEditorActions::GetActions(const std::vector<ActionKey> &action_keys)
{
  QList<QAction *> result;
  std::transform(action_keys.begin(), action_keys.end(), std::back_inserter(result),
                 [this](auto element) { return GetAction(element); });

  return result;
}

QAction *InstructionEditorActions::GetAction(ActionKey key) const
{
  auto iter = m_actions.find(key);
  return iter == m_actions.end() ? nullptr : iter->second;
}

void InstructionEditorActions::SetupInsertRemoveActions()
{
  // We wrap QToolButton into QWidgetAction to have a menu with instant popup capabilties (which is
  // a QToolButton feature) and still be able to pass actions around.

  m_insert_after_action = new QWidgetAction(this);
  auto insert_after_button = new QToolButton;
  insert_after_button->setText("Add");
  insert_after_button->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Add instruction after current selection");
  m_insert_after_action->setDefaultWidget(insert_after_button);
  m_actions[ActionKey::kInsertAfter] = m_insert_after_action;

  m_insert_into_action = new QWidgetAction(this);
  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Insert");
  insert_into_button->setIcon(sup::gui::utils::GetIcon("plus-circle-multiple-outline.svg"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  m_insert_into_action->setDefaultWidget(insert_into_button);
  m_actions[ActionKey::kInsertInto] = m_insert_into_action;

  m_remove_action = new QWidgetAction(this);
  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(remove_button, &QToolButton::clicked, this,
          &InstructionEditorActions::RemoveSelectedRequest);
  m_remove_action->setDefaultWidget(remove_button);
  m_actions[ActionKey::kRemoveSelected] = m_remove_action;

  m_move_up_action = new QWidgetAction(this);
  auto move_up_button = new QToolButton;
  move_up_button->setText("Move Up");
  move_up_button->setIcon(sup::gui::utils::GetIcon("arrow-up-thin-circle-outline.svg"));
  move_up_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  move_up_button->setToolTip(
      "Move currently selected instruction up, works within the same parent");
  connect(move_up_button, &QToolButton::clicked, this, &InstructionEditorActions::MoveUpRequest);
  m_move_up_action->setDefaultWidget(move_up_button);
  m_actions[ActionKey::kMoveUp] = m_move_up_action;

  m_move_down_action = new QWidgetAction(this);
  auto move_down_button = new QToolButton;
  move_down_button->setText("Move Down");
  move_down_button->setIcon(sup::gui::utils::GetIcon("arrow-down-thin-circle-outline.svg"));
  move_down_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  move_down_button->setToolTip(
      "Move currently selected instruction down, works within the same parent");
  connect(move_down_button, &QToolButton::clicked, this,
          &InstructionEditorActions::MoveDownRequest);
  m_move_down_action->setDefaultWidget(move_down_button);
  m_actions[ActionKey::kMoveDown] = m_move_down_action;
}

void InstructionEditorActions::SetupCutCopyPasteActions()
{
  m_cut_action = new QAction(this);
  m_cut_action->setText("Cut");
  m_cut_action->setToolTip("Cuts selected procedure");
  connect(m_cut_action, &QAction::triggered, this, &InstructionEditorActions::CutRequest);
  m_actions[ActionKey::kCut] = m_cut_action;

  m_copy_action = new QAction(this);
  m_copy_action->setText("Copy");
  m_copy_action->setToolTip("Copies selected procedure");
  connect(m_copy_action, &QAction::triggered, this, &InstructionEditorActions::CopyRequest);
  m_actions[ActionKey::kCopy] = m_copy_action;

  m_paste_action = new QAction(this);
  m_paste_action->setText("Paste");
  m_paste_action->setToolTip("Paste selected procedure");
  connect(m_paste_action, &QAction::triggered, this, &InstructionEditorActions::PasteRequest);
  m_actions[ActionKey::kPaste] = m_paste_action;
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

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertIntoMenu()
{
  // Code mostly coincides with the code above. However, this duplication is temporary and it
  // will diverge in the future (idea to disable some actions if an operation is not possible).

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
