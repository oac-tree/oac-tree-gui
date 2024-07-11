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

#include <sequencergui/domain/domain_object_group_helper.h>
#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/components/proxy_action.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>

namespace sequencergui
{

InstructionEditorActions::InstructionEditorActions(InstructionEditorActionHandler *handler,
                                                   QObject *parent)
    : QObject(parent)
    , m_action_handler(handler)
    , m_insert_after_menu(CreateInsertMenu())
    , m_insert_into_menu(CreateInsertMenu())
{
  SetupInsertRemoveActions();
  SetupCutCopyPasteActions();
}

InstructionEditorActions::~InstructionEditorActions() = default;

QList<QAction *> InstructionEditorActions::GetActions(
    const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void InstructionEditorActions::SetupMenu(QMenu &menu)
{
  menu.addAction(m_insert_after_action);
  menu.addAction(m_insert_into_action);
  menu.addAction(m_remove_action);
  menu.addSeparator();

  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_after_action);
  menu.addAction(m_paste_into_action);
  menu.addSeparator();

  UpdateEnabledStatus();
}

void InstructionEditorActions::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  sup::gui::AppAddActionToCommand(m_cut_action, sup::gui::constants::kCutCommandId, context);
  sup::gui::AppAddActionToCommand(m_copy_action, sup::gui::constants::kCopyCommandId, context);
  sup::gui::AppAddActionToCommand(m_paste_after_action, sup::gui::constants::kPasteCommandId,
                                  context);
  sup::gui::AppAddActionToCommand(m_paste_into_action, sup::gui::constants::kPasteSpecialCommandId,
                                  context);
}

void InstructionEditorActions::UpdateEnabledStatus()
{
  m_remove_action->setEnabled(m_action_handler->CanRemoveInstruction());
  m_cut_action->setEnabled(m_action_handler->CanCut());
  m_copy_action->setEnabled(m_action_handler->CanCopy());
  m_paste_into_action->setEnabled(m_action_handler->CanPasteInto());
  m_paste_after_action->setEnabled(m_action_handler->CanPasteAfter());

  // no need to update insert into/after action status since it is checked at the moment of menu
  // pop-up
}

void InstructionEditorActions::SetupInsertRemoveActions()
{
  // insert instruction after (special version whick looks nice in a toolbar and in pop-up menu)
  m_insert_after_action = new sup::gui::ActionMenu(this);
  m_insert_after_action->setText("Add");
  m_insert_after_action->setIcon(sup::gui::utils::GetIcon("plus-circle-outline.svg"));
  m_insert_after_action->setMenu(m_insert_after_menu.get());
  m_insert_after_action->setToolTip("Add instruction after current selection");
  m_action_map.Add(ActionKey::kInsertAfter, m_insert_after_action);

  // insert instruction into
  m_insert_into_action = new sup::gui::ActionMenu(this);
  m_insert_into_action->setText("Insert");
  m_insert_into_action->setIcon(sup::gui::utils::GetIcon("plus-circle-multiple-outline.svg"));
  m_insert_into_action->setMenu(m_insert_into_menu.get());
  m_insert_into_action->setToolTip("Insert instruction into currently selected instruction");
  m_action_map.Add(ActionKey::kInsertInto, m_insert_into_action);

  // remove action
  m_remove_action = new QAction(this);
  m_remove_action->setText("Remove");
  m_remove_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_action->setToolTip("Remove currently selected instruction together with its children");
  connect(m_remove_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnRemoveInstructionRequest(); });

  // remove action (own toolbar version to avoid disabled status)
  m_remove_toolbar_action = new sup::gui::ProxyAction(this);
  m_remove_toolbar_action->SetAction(m_remove_action);
  m_action_map.Add(ActionKey::kRemoveSelected, m_remove_toolbar_action);

  m_move_up_action = new QAction(this);
  m_move_up_action->setText("Move Up");
  m_move_up_action->setIcon(sup::gui::utils::GetIcon("arrow-up-thin-circle-outline.svg"));
  m_move_up_action->setToolTip(
      "Move currently selected instruction up, works within the same parent");
  m_action_map.Add(ActionKey::kMoveUp, m_move_up_action);
  connect(m_move_up_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnMoveUpRequest(); });

  m_move_down_action = new QAction(this);
  m_move_down_action->setText("Move Down");
  m_move_down_action->setIcon(sup::gui::utils::GetIcon("arrow-down-thin-circle-outline.svg"));
  m_move_down_action->setToolTip(
      "Move currently selected instruction down, works within the same parent");
  m_action_map.Add(ActionKey::kMoveDown, m_move_down_action);
  connect(m_move_down_action, &QAction::triggered, this,
          [this]() { m_action_handler->OnMoveDownRequest(); });
}

void InstructionEditorActions::SetupCutCopyPasteActions()
{
  m_cut_action = new QAction(this);
  m_cut_action->setText("Cut");
  m_cut_action->setToolTip("Cuts selected instruction");
  m_cut_action->setShortcut(QKeySequence("Ctrl+X"));
  m_action_map.Add(ActionKey::kCut, m_cut_action);
  auto on_cut = [this]()
  {
    m_action_handler->Cut();
    UpdateEnabledStatus();  // to update availability of paste operation
  };
  connect(m_cut_action, &QAction::triggered, this, on_cut);

  m_copy_action = new QAction(this);
  m_copy_action->setText("Copy");
  m_copy_action->setToolTip("Copies selected instruction");
  m_copy_action->setShortcut(QKeySequence("Ctrl+C"));
  m_action_map.Add(ActionKey::kCopy, m_copy_action);
  auto on_copy_action = [this]()
  {
    m_action_handler->Copy();
    UpdateEnabledStatus();  // to update availability of paste operation
  };
  connect(m_copy_action, &QAction::triggered, this, on_copy_action);

  m_paste_after_action = new QAction(this);
  m_paste_after_action->setText("Paste After");
  m_paste_after_action->setToolTip("Paste selected instruction after current selection");
  m_paste_after_action->setShortcut(QKeySequence("Ctrl+V"));
  m_action_map.Add(ActionKey::kPasteAfter, m_paste_after_action);
  connect(m_paste_after_action, &QAction::triggered, this,
          [this]() { m_action_handler->PasteAfter(); });

  m_paste_into_action = new QAction(this);
  m_paste_into_action->setText("Paste Into");
  m_paste_into_action->setToolTip("Paste selected instruction into current selection");
  m_paste_into_action->setShortcut(QKeySequence("Ctrl+Shift+V"));
  m_action_map.Add(ActionKey::kPasteInto, m_paste_into_action);
  connect(m_paste_into_action, &QAction::triggered, this,
          [this]() { m_action_handler->PasteInto(); });
}

std::unique_ptr<QMenu> InstructionEditorActions::CreateInsertMenu()
{
  auto result = std::make_unique<QMenu>();
  connect(result.get(), &QMenu::aboutToShow, this,
          &InstructionEditorActions::AboutToShowInsertMenu);
  return result;
}

void InstructionEditorActions::AboutToShowInsertMenu()
{
  auto menu = qobject_cast<QMenu *>(sender());
  const bool insert_into = (menu == m_insert_into_menu.get());

  menu->clear();
  menu->setToolTipsVisible(true);

  auto group_info = CreateInstructionTypeGroups();
  for (const auto &group_info : group_info)
  {
    auto group_menu = menu->addMenu(QString::fromStdString(group_info.group_name));
    int enabled_actions_count{0};
    for (const auto &name : group_info.object_names)
    {
      auto action = group_menu->addAction(QString::fromStdString(name));
      if (insert_into ? m_action_handler->CanInsertInto(name)
                      : m_action_handler->CanInsertAfter(name))
      {
        action->setEnabled(true);
        ++enabled_actions_count;
      }
      if (insert_into)
      {
        connect(action, &QAction::triggered, m_action_handler,
                [this, name]() { m_action_handler->OnInsertInstructionIntoRequest(name); });
      }
      else
      {
        connect(action, &QAction::triggered, m_action_handler,
                [this, name]() { m_action_handler->OnInsertInstructionAfterRequest(name); });
      }
    };
    group_menu->setEnabled(enabled_actions_count > 0);
  }
}

}  // namespace sequencergui
