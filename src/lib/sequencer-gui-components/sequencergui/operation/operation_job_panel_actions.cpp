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

#include "operation_job_panel_actions.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace sequencergui
{

OperationJobPanelToolActions::OperationJobPanelToolActions(QObject *parent)
    : QObject(parent)
    , m_import_action(new QWidgetAction(this))
    , m_submit_action(new QWidgetAction(this))
    , m_regenerate_action(new QWidgetAction(this))
    , m_remove_action(new QWidgetAction(this))
    , m_submit_procedure_menu(CreateSubmitProcedureMenu())
{
  auto import_button = new QToolButton;
  import_button->setText("New");
  import_button->setIcon(styleutils::GetIcon("file-plus-outline"));
  import_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  import_button->setToolTip("Open sequencer XML procedure from disk");
  connect(import_button, &QToolButton::clicked, this,
          &OperationJobPanelToolActions::ImportJobRequest);
  m_import_action->setDefaultWidget(import_button);

  auto submit_button = new QToolButton;
  submit_button->setText("Submit");
  submit_button->setIcon(styleutils::GetIcon("file-plus-outline"));
  submit_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  submit_button->setPopupMode(QToolButton::InstantPopup);
  submit_button->setMenu(m_submit_procedure_menu.get());
  submit_button->setToolTip("Submit existing sequencer procedure for execution");
  m_submit_action->setDefaultWidget(submit_button);

  auto regenerate_button = new QToolButton;
  regenerate_button->setText("Reload");
  regenerate_button->setIcon(styleutils::GetIcon("refresh"));
  regenerate_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  regenerate_button->setToolTip("Reload selected procedure");
  connect(regenerate_button, &QToolButton::clicked, this,
          &OperationJobPanelToolActions::RegenerateJobRequest);
  m_regenerate_action->setDefaultWidget(regenerate_button);

  auto remove_button = new QToolButton;
  remove_button->setText("Remove");
  remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  remove_button->setToolTip("Remove selected procedure from the list");
  connect(remove_button, &QToolButton::clicked, this,
          &OperationJobPanelToolActions::RemoveJobRequest);
  m_remove_action->setDefaultWidget(remove_button);
}

void OperationJobPanelToolActions::SetAvailableProcedures(callback_t available_procedures)
{
  m_available_procedures = available_procedures;
}

QList<QAction *> OperationJobPanelToolActions::GetSequencerMonitorViewActions()
{
  return QList<QAction *>({m_submit_action, m_regenerate_action, m_remove_action});
}

QList<QAction *> OperationJobPanelToolActions::GetOperationMonitorViewActions()
{
  return QList<QAction *>({m_import_action, m_regenerate_action, m_remove_action});
}

std::unique_ptr<QMenu> OperationJobPanelToolActions::CreateSubmitProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this,
          &OperationJobPanelToolActions::OnAboutToShowMenu);
  return result;
}

void OperationJobPanelToolActions::OnAboutToShowMenu()
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
