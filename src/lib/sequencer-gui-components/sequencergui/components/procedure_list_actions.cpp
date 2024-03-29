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

#include "procedure_list_action_handler.h"

#include <sup/gui/widgets/style_utils.h>

#include <QAction>
#include <QMenu>

namespace sequencergui
{

ProcedureListActions::ProcedureListActions(QObject *parent)
    : QObject(parent)
    , m_new_procedure_action(new QAction(this))
    , m_remove_selected_action(new QAction(this))
    , m_cut_action(new QAction(this))
    , m_copy_action(new QAction(this))
    , m_paste_action(new QAction(this))
{
  m_new_procedure_action->setText("New Procedure");
  m_new_procedure_action->setToolTip("Creates new empty procedure");
  m_new_procedure_action->setIcon(sup::gui::utils::GetIcon("file-plus-outline.svg"));
  connect(m_new_procedure_action, &QAction::triggered, this,
          &ProcedureListActions::CreateNewProcedureRequest);
  m_action_map[ActionKey::kCreateNew] = m_new_procedure_action;

  m_remove_selected_action->setText("Remove Procedure");
  m_remove_selected_action->setToolTip("Removes selected procedure");
  m_remove_selected_action->setIcon(sup::gui::utils::GetIcon("file-remove-outline.svg"));
  connect(m_remove_selected_action, &QAction::triggered, this,
          &ProcedureListActions::RemoveProcedureRequest);
  m_action_map[ActionKey::kRemoveSelected] = m_remove_selected_action;

  m_cut_action->setText("Cut");
  m_cut_action->setToolTip("Cuts selected procedure");
  connect(m_cut_action, &QAction::triggered, this, &ProcedureListActions::CutRequest);
  m_action_map[ActionKey::kCut] = m_cut_action;

  m_copy_action->setText("Copy");
  m_copy_action->setToolTip("Copies selected procedure");
  connect(m_copy_action, &QAction::triggered, this, &ProcedureListActions::CopyRequest);
  m_action_map[ActionKey::kCopy] = m_copy_action;

  m_paste_action->setText("Paste");
  m_paste_action->setToolTip("Paste selected procedure");
  connect(m_paste_action, &QAction::triggered, this, &ProcedureListActions::PasteRequest);
  m_action_map[ActionKey::kPaste] = m_paste_action;
}

QList<QAction *> ProcedureListActions::GetActions(const std::vector<ActionKey> &action_keys)
{
  QList<QAction *> result;
  std::transform(action_keys.begin(), action_keys.end(), std::back_inserter(result),
                 [this](auto element) { return GetAction(element); });

  return result;
}

void ProcedureListActions::SetupMenu(QMenu &menu, const ProcedureListActionHandler *handler)
{
  menu.setToolTipsVisible(true);
  menu.addAction(m_new_procedure_action);
  menu.addSeparator();
  menu.addAction(m_cut_action);
  menu.addAction(m_copy_action);
  menu.addAction(m_paste_action);
  menu.addSeparator();
  menu.addAction(m_remove_selected_action);

  m_cut_action->setEnabled(handler->CanCut());
  m_copy_action->setEnabled(handler->CanCopy());
  m_paste_action->setEnabled(handler->CanPaste());
  m_remove_selected_action->setEnabled(handler->CanRemove());
}

QAction *ProcedureListActions::GetAction(ActionKey key) const
{
  auto iter = m_action_map.find(key);
  return iter == m_action_map.end() ? nullptr : iter->second;
}

}  // namespace sequencergui
