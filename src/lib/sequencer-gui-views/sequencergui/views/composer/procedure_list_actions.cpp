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

#include "procedure_list_actions.h"

#include <sequencergui/composer/procedure_list_action_handler.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_context.h>
#include <sup/gui/widgets/style_utils.h>

#include <QAction>
#include <QMenu>

namespace sequencergui
{

ProcedureListActions::ProcedureListActions(QObject *parent) : QObject(parent)
{
  m_new_procedure_action = new QAction("New Procedure", this);
  m_new_procedure_action->setToolTip("Creates new empty procedure");
  m_new_procedure_action->setIcon(sup::gui::utils::GetIcon("file-plus-outline.svg"));
  connect(m_new_procedure_action, &QAction::triggered, this,
          &ProcedureListActions::CreateNewProcedureRequest);
  m_action_map.Add(ActionKey::kCreateNew, m_new_procedure_action);

  m_remove_selected_action = new QAction("Remove Procedure", this);
  m_remove_selected_action->setToolTip("Removes selected procedure");
  m_remove_selected_action->setIcon(sup::gui::utils::GetIcon("file-remove-outline.svg"));
  connect(m_remove_selected_action, &QAction::triggered, this,
          &ProcedureListActions::RemoveProcedureRequest);
  m_action_map.Add(ActionKey::kRemoveSelected, m_remove_selected_action);

  m_cut_action = new QAction("Cut", this);
  m_cut_action->setToolTip("Cuts selected procedure");
  m_cut_action->setShortcut(QKeySequence::Cut);
  connect(m_cut_action, &QAction::triggered, this, &ProcedureListActions::CutRequest);
  m_action_map.Add(ActionKey::kCut, m_cut_action);

  m_copy_action = new QAction("Copy", this);
  m_copy_action->setToolTip("Copies selected procedure");
  m_copy_action->setShortcut(QKeySequence::Copy);
  connect(m_copy_action, &QAction::triggered, this, &ProcedureListActions::CopyRequest);
  m_action_map.Add(ActionKey::kCopy, m_copy_action);

  m_paste_action = new QAction("Paste", this);
  m_paste_action->setToolTip("Paste selected procedure");
  m_paste_action->setShortcut(QKeySequence::Paste);
  connect(m_paste_action, &QAction::triggered, this, &ProcedureListActions::PasteRequest);
  m_action_map.Add(ActionKey::kPaste, m_paste_action);
}

QList<QAction *> ProcedureListActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void ProcedureListActions::SetupMenu(QMenu &menu, const ProcedureListActionHandler *handler)
{
  menu.setToolTipsVisible(true);
  menu.addAction(m_new_procedure_action);
  menu.addAction(m_remove_selected_action);
  m_remove_selected_action->setEnabled(handler->CanRemove());

  menu.addSeparator();
  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_action);

  m_cut_action->setEnabled(handler->CanCut());
  m_copy_action->setEnabled(handler->CanCopy());
  m_paste_action->setEnabled(handler->CanPaste());
}

void ProcedureListActions::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  sup::gui::AppAddActionToCommand(m_cut_action, sup::gui::constants::kCutCommandId, context);
  sup::gui::AppAddActionToCommand(m_copy_action, sup::gui::constants::kCopyCommandId, context);
  sup::gui::AppAddActionToCommand(m_paste_action, sup::gui::constants::kPasteCommandId, context);
}

}  // namespace sequencergui
