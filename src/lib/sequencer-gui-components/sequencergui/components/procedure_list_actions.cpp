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

#include <sup/gui/widgets/style_utils.h>

#include <QAction>

namespace sequencergui
{

ProcedureListActions::ProcedureListActions(QObject *parent)
    : QObject(parent)
    , m_new_procedure_action(new QAction(this))
    , m_remove_selected_action(new QAction(this))
{
  m_new_procedure_action->setText("New Procedure");
  m_new_procedure_action->setToolTip("Creates new empty procedure");
  m_new_procedure_action->setIcon(sup::gui::utils::GetIcon("file-plus-outline.svg"));
  connect(m_new_procedure_action, &QAction::triggered, this,
          &ProcedureListActions::CreateNewProcedureRequest);
  m_actions[ActionKey::kCreateNew] = m_new_procedure_action;

  m_remove_selected_action->setText("Remove Procedure");
  m_remove_selected_action->setToolTip("Removes selected procedure");
  m_remove_selected_action->setIcon(sup::gui::utils::GetIcon("file-remove-outline.svg"));
  connect(m_remove_selected_action, &QAction::triggered, this,
          &ProcedureListActions::RemoveProcedureRequest);
  m_actions[ActionKey::kRemoveSelected] = m_remove_selected_action;
}

QList<QAction *> ProcedureListActions::GetActions(const std::vector<ActionKey> &action_keys)
{
  QList<QAction *> result;
  for (auto key : action_keys)
  {
    auto iter = m_actions.find(key);
    if (iter != m_actions.end())
    {
      result.push_back(iter->second);
    }
  }

  return result;
}

}  // namespace sequencergui
