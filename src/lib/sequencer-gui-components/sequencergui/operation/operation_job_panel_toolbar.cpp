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

#include "operation_job_panel_toolbar.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/widgets/style_utils.h>

#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace sequencergui
{

OperationJobPanelToolBar::OperationJobPanelToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_import_button(new QToolButton)
    , m_submit_button(new QToolButton)
    , m_regenerate_button(new QToolButton)
    , m_remove_button(new QToolButton)
    , m_submit_procedure_menu(CreateSubmitProcedureMenu())
{
  setIconSize(sup::gui::utils::ToolBarIconSize());

  m_import_button->setText("New");
  m_import_button->setIcon(styleutils::GetIcon("file-plus-outline"));
  m_import_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_import_button->setToolTip("Open sequencer XML procedure from disk");
  connect(m_import_button, &QToolButton::clicked, this,
          &OperationJobPanelToolBar::ImportJobRequest);
  addWidget(m_import_button);

  m_submit_button->setText("Submit");
  m_submit_button->setIcon(styleutils::GetIcon("file-plus-outline"));
  m_submit_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_submit_button->setPopupMode(QToolButton::InstantPopup);
  m_submit_button->setMenu(m_submit_procedure_menu.get());
  m_submit_button->setToolTip("Submit existing sequencer procedure for execution");
  addWidget(m_submit_button);

  m_regenerate_button->setText("Reload");
  m_regenerate_button->setIcon(styleutils::GetIcon("refresh"));
  m_regenerate_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_regenerate_button->setToolTip("Reload selected procedure");
  connect(m_regenerate_button, &QToolButton::clicked, this,
          &OperationJobPanelToolBar::RegenerateJobRequest);
  addWidget(m_regenerate_button);

  m_remove_button->setText("Remove");
  m_remove_button->setIcon(styleutils::GetIcon("beaker-remove-outline"));
  m_remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_remove_button->setToolTip("Remove selected procedure from the list");
  connect(m_remove_button, &QToolButton::clicked, this,
          &OperationJobPanelToolBar::RemoveJobRequest);
  addWidget(m_remove_button);
}

void OperationJobPanelToolBar::SetAvailableProcedures(callback_t available_procedures)
{
  m_available_procedures = available_procedures;
}

std::unique_ptr<QMenu> OperationJobPanelToolBar::CreateSubmitProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &OperationJobPanelToolBar::OnAboutToShowMenu);
  return result;
}

void OperationJobPanelToolBar::OnAboutToShowMenu()
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
