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

#include "operation_job_panel_actions.h"

#include <sequencergui/model/procedure_item.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace sequencergui
{

OperationJobPanelActions::OperationJobPanelActions(QObject *parent)
    : QObject(parent)
    , m_import_action(new QAction(this))
    , m_submit_action(new sup::gui::ActionMenu(this))
    , m_regenerate_action(new QAction(this))
    , m_remove_action(new QAction(this))
    , m_remove_and_cleanup_action(new QAction(this))
    , m_submit_procedure_menu(CreateSubmitProcedureMenu())
{
  m_import_action->setText("New");
  m_import_action->setIcon(sup::gui::utils::GetIcon("file-plus-outline.svg"));
  m_import_action->setToolTip("Open sequencer XML procedure from disk");
  connect(m_import_action, &QAction::triggered, this, &OperationJobPanelActions::ImportJobRequest);

  m_submit_action->setText("Submit");
  m_submit_action->setIcon(sup::gui::utils::GetIcon("file-plus-outline.svg"));
  m_submit_action->setMenu(m_submit_procedure_menu.get());
  m_submit_action->setToolTip("Submit existing sequencer procedure for execution");

  m_regenerate_action->setText("Reload");
  m_regenerate_action->setIcon(sup::gui::utils::GetIcon("refresh.svg"));
  connect(m_regenerate_action, &QAction::triggered, this,
          &OperationJobPanelActions::RegenerateJobRequest);

  m_remove_action->setText("Remove");
  m_remove_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  connect(m_remove_action, &QAction::triggered, this, &OperationJobPanelActions::RemoveJobRequest);

  m_remove_and_cleanup_action->setText("Remove");
  m_remove_and_cleanup_action->setIcon(sup::gui::utils::GetIcon("beaker-remove-outline.svg"));
  m_remove_and_cleanup_action->setToolTip("Remove selected procedure from the list");
  connect(m_remove_and_cleanup_action, &QAction::triggered, this,
          &OperationJobPanelActions::RemoveAndCleanupJobRequest);
}

OperationJobPanelActions::~OperationJobPanelActions() = default;

void OperationJobPanelActions::SetAvailableProcedures(callback_t available_procedures)
{
  m_available_procedures = available_procedures;
}

QList<QAction *> OperationJobPanelActions::GetSequencerMonitorViewActions()
{
  return QList<QAction *>({m_submit_action, m_regenerate_action, m_remove_action});
}

QList<QAction *> OperationJobPanelActions::GetOperationMonitorViewActions()
{
  return QList<QAction *>({m_import_action, m_regenerate_action, m_remove_and_cleanup_action});
}

std::unique_ptr<QMenu> OperationJobPanelActions::CreateSubmitProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &OperationJobPanelActions::OnAboutToShowMenu);
  return result;
}

void OperationJobPanelActions::OnAboutToShowMenu()
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

}  // namespace sequencergui
