/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "operation_job_actions.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/widgets/action_menu.h>

#include <QMenu>
#include <QToolButton>

namespace oac_tree_gui
{

OperationJobActions::OperationJobActions(QObject *parent_object)
    : QObject(parent_object)
    , m_submit_procedure_menu(CreateSubmitProcedureMenu())
    , m_import_action(new QAction(this))
    , m_submit_action(new sup::gui::ActionMenu(this))
    , m_regenerate_action(new QAction(this))
    , m_remove_action(new QAction(this))
    , m_connect_to_remote_action(new QAction(this))
{
  m_import_action->setText("New");
  m_import_action->setIcon(FindIcon("file-plus-outline"));
  m_import_action->setToolTip("Open sequencer XML procedure from disk");
  connect(m_import_action, &QAction::triggered, this, &OperationJobActions::ImportJobRequest);
  m_action_map.Add(ActionKey::kImportProcedure, m_import_action);

  m_submit_action->setText("Submit");
  m_submit_action->setIcon(FindIcon("file-plus-outline"));
  m_submit_action->setMenu(m_submit_procedure_menu.get());
  m_submit_action->setToolTip("Submit existing sequencer procedure for execution");
  m_action_map.Add(ActionKey::kSubmitJob, m_submit_action);

  m_regenerate_action->setText("Reload");
  m_regenerate_action->setIcon(FindIcon("refresh"));
  m_regenerate_action->setToolTip("Full rebuild of sequencer job from the original source");
  connect(m_regenerate_action, &QAction::triggered, this,
          &OperationJobActions::RegenerateJobRequest);
  m_action_map.Add(ActionKey::kRegenerateJob, m_regenerate_action);

  m_remove_action->setText("Remove");
  m_remove_action->setToolTip("Remove selected procedure from the list");
  m_remove_action->setIcon(FindIcon("beaker-remove-outline"));
  connect(m_remove_action, &QAction::triggered, this, &OperationJobActions::RemoveJobRequest);
  m_action_map.Add(ActionKey::kRemoveJob, m_remove_action);

  m_connect_to_remote_action->setText("Connect");
  m_connect_to_remote_action->setToolTip("Connect to remote server and import jobs");
  m_connect_to_remote_action->setIcon(FindIcon("lan-pending"));
  connect(m_connect_to_remote_action, &QAction::triggered, this,
          &OperationJobActions::ConnectRequest);
  m_action_map.Add(ActionKey::kConnect, m_connect_to_remote_action);
}

QList<QAction *> OperationJobActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

OperationJobActions::~OperationJobActions() = default;

void OperationJobActions::SetAvailableProcedures(callback_t available_procedures)
{
  m_available_procedures = available_procedures;
}

std::unique_ptr<QMenu> OperationJobActions::CreateSubmitProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &OperationJobActions::OnAboutToShowMenu);
  return result;
}

void OperationJobActions::OnAboutToShowMenu()
{
  if (!m_available_procedures)
  {
    return;
  }

  auto menu = m_submit_procedure_menu.get();
  menu->clear();
  menu->addSection("Available procedures:");

  // build a menu containing list of procedures for later job submission
  for (auto procedure : m_available_procedures())
  {
    auto action = menu->addAction(QString::fromStdString(procedure->GetDisplayName()));
    action->setToolTip("Submit given procedure for execution");
    auto on_action = [this, procedure]() { emit SubmitProcedureRequest(procedure); };
    connect(action, &QAction::triggered, this, on_action);
  }
}

}  // namespace oac_tree_gui
