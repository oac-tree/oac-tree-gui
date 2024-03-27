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

#include "instruction_editor_action_handler.h"

#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/widgets/proxy_action.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

InstructionEditorActions::InstructionEditorActions(const InstructionEditorActionHandler *handler,
                                                   QObject *parent)
    : QObject(parent)
    , m_handler(handler)
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

void InstructionEditorActions::SetupMenu(QMenu &menu)
{
  menu.addAction(m_insert_after_action);
  menu.addAction(m_insert_into_action);
  menu.addAction(m_remove_action);
  menu.addSeparator();

  m_remove_action->setEnabled(m_handler->CanRemoveInstruction());

  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_after_action);
  menu.addAction(m_paste_into_action);
  menu.addSeparator();

  m_cut_action->setEnabled(m_handler->CanCut());
  m_copy_action->setEnabled(m_handler->CanCopy());
  m_paste_into_action->setEnabled(m_handler->CanPasteInto());
  m_paste_after_action->setEnabled(m_handler->CanPasteAfter());
}

void InstructionEditorActions::SetupInsertRemoveActions()
{
  // insert instruction after
  m_insert_after_action = new QAction(this);
  m_insert_after_action->setText("Add");
  m_insert_after_action->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  m_insert_after_action->setMenu(m_insert_after_menu.get());
  m_insert_after_action->setToolTip("Add instruction after current selection");

  // insert instruction after (toolbar version with instant pop-up capability)
  m_insert_after_toolbar_action = new QWidgetAction(this);
  auto insert_after_button = new QToolButton;
  insert_after_button->setText("Add");
  insert_after_button->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  insert_after_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_after_button->setPopupMode(QToolButton::InstantPopup);
  insert_after_button->setMenu(m_insert_after_menu.get());
  insert_after_button->setToolTip("Add instruction after current selection");
  m_insert_after_toolbar_action->setDefaultWidget(insert_after_button);
  m_actions[ActionKey::kInsertAfter] = m_insert_after_toolbar_action;

  // insert instruction into
  m_insert_into_action = new QWidgetAction(this);
  m_insert_into_action->setText("Insert");
  m_insert_into_action->setIcon(sup::gui::utils::GetIcon("plus-circle-multiple-outline.svg"));
  m_insert_into_action->setMenu(m_insert_into_menu.get());
  m_insert_into_action->setToolTip("Insert instruction into currently selected instruction");

  // insert instruction into (toolbar version with instant pop-up capability)
  m_insert_into_toolbar_action = new QWidgetAction(this);
  auto insert_into_button = new QToolButton;
  insert_into_button->setText("Insert");
  insert_into_button->setIcon(sup::gui::utils::GetIcon("plus-circle-multiple-outline.svg"));
  insert_into_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  insert_into_button->setPopupMode(QToolButton::InstantPopup);
  insert_into_button->setMenu(m_insert_into_menu.get());
  insert_into_button->setToolTip("Insert instruction into currently selected instruction");
  m_insert_into_toolbar_action->setDefaultWidget(insert_into_button);
  m_actions[ActionKey::kInsertInto] = m_insert_into_toolbar_action;

  // remove action
  m_remove_action = new QAction(this);
  m_remove_action->setText("Remove");
  m_remove_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_action->setToolTip("Remove currently selected instruction together with its children");
  connect(m_remove_action, &QAction::triggered, this,
          &InstructionEditorActions::RemoveSelectedRequest);

  // remove action (own toolbar version to avoid disabled status)
  m_remove_toolbar_action = new sup::gui::ProxyAction(this);
  m_remove_toolbar_action->SetTrackEnabled(false);
  m_remove_toolbar_action->SetAction(m_remove_action);
  m_actions[ActionKey::kRemoveSelected] = m_remove_toolbar_action;

  m_move_up_action = new QAction(this);
  m_move_up_action->setText("Move Up");
  m_move_up_action->setIcon(sup::gui::utils::GetIcon("arrow-up-thin-circle-outline.svg"));
  m_move_up_action->setToolTip(
      "Move currently selected instruction up, works within the same parent");
  connect(m_move_up_action, &QAction::triggered, this, &InstructionEditorActions::MoveUpRequest);
  m_actions[ActionKey::kMoveUp] = m_move_up_action;

  m_move_down_action = new QAction(this);
  m_move_down_action->setText("Move Down");
  m_move_down_action->setIcon(sup::gui::utils::GetIcon("arrow-down-thin-circle-outline.svg"));
  m_move_down_action->setToolTip(
      "Move currently selected instruction down, works within the same parent");
  connect(m_move_down_action, &QAction::triggered, this,
          &InstructionEditorActions::MoveDownRequest);
  m_actions[ActionKey::kMoveDown] = m_move_down_action;
}

void InstructionEditorActions::SetupCutCopyPasteActions()
{
  m_cut_action = new QAction(this);
  m_cut_action->setText("Cut");
  m_cut_action->setToolTip("Cuts selected instruction");
  m_cut_action->setShortcut(QKeySequence("Ctrl+X"));
  connect(m_cut_action, &QAction::triggered, this, &InstructionEditorActions::CutRequest);
  m_actions[ActionKey::kCut] = m_cut_action;

  m_copy_action = new QAction(this);
  m_copy_action->setText("Copy");
  m_copy_action->setToolTip("Copies selected instruction");
  m_copy_action->setShortcut(QKeySequence("Ctrl+C"));
  connect(m_copy_action, &QAction::triggered, this, &InstructionEditorActions::CopyRequest);
  m_actions[ActionKey::kCopy] = m_copy_action;

  m_paste_after_action = new QAction(this);
  m_paste_after_action->setText("Paste After");
  m_paste_after_action->setToolTip("Paste selected instruction after current selection");
  m_paste_after_action->setShortcut(QKeySequence("Ctrl+V"));
  connect(m_paste_after_action, &QAction::triggered, this,
          &InstructionEditorActions::PasteAfterRequest);
  m_actions[ActionKey::kPasteAfter] = m_paste_after_action;

  m_paste_into_action = new QAction(this);
  m_paste_into_action->setText("Paste Into");
  m_paste_into_action->setToolTip("Paste selected instruction into current selection");
  m_paste_into_action->setShortcut(QKeySequence("Ctrl+Shift+V"));
  connect(m_paste_into_action, &QAction::triggered, this,
          &InstructionEditorActions::PasteIntoRequest);
  m_actions[ActionKey::kPasteInto] = m_paste_into_action;
}

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertAfterMenu() const
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this,
          &InstructionEditorActions::OnAboutToShowInsertAfterMenu);
  return result;
}

void InstructionEditorActions::OnAboutToShowInsertAfterMenu()
{
  auto menu = m_insert_after_menu.get();
  menu->clear();

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = menu->addAction(name);
    action->setEnabled(m_handler->CanInsertAfter(name));
    auto on_action = [this, name]() { emit InsertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }
}

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertIntoMenu() const
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this,
          &InstructionEditorActions::OnAboutToShowInsertIntoMenu);
  return result;
}

void InstructionEditorActions::OnAboutToShowInsertIntoMenu()
{
  auto menu = m_insert_into_menu.get();
  menu->clear();

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = menu->addAction(name);
    action->setEnabled(m_handler->CanInsertInto(name));
    auto on_action = [this, name]() { emit InsertIntoRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }
}

}  // namespace sequencergui
