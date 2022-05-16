/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/monitorpanel.h"

#include "sequencergui/model/applicationmodels.h"
#include "sequencergui/model/jobitem.h"
#include "sequencergui/monitor/joblistwidget.h"
#include "sequencergui/monitor/jobpropertywidget.h"
#include "sequencergui/utils/styleutils.h"
#include "sequencergui/widgets/collapsiblelistview.h"
#include "sequencergui/widgets/collapsibletoolbar.h"
#include "sequencergui/widgets/paneltoolbar.h"

#include <QDebug>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorPanel::MonitorPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_collapsible_list(new CollapsibleListView)
    , m_job_list_widget(new JobListWidget)
    , m_job_property_widget(new JobPropertyWidget)
    , m_submit_procedure_menu(CreateSubmitProcedureMenu())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_collapsible_list);

  auto toolbar = m_collapsible_list->AddCollapsibleWidget(m_job_list_widget, {});
  toolbar->AddWidgets(GetToolBarWidgets());
  m_collapsible_list->AddCollapsibleWidget(m_job_property_widget, m_job_property_widget->actions());

  connect(m_job_list_widget, &JobListWidget::JobSelected, this, &MonitorPanel::OnJobSelectedIntern);
}

MonitorPanel::~MonitorPanel() = default;

void MonitorPanel::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  m_job_list_widget->SetJobModel(models->GetJobModel());
}

JobItem *MonitorPanel::GetSelectedJob() const
{
  return m_job_list_widget->GetSelectedJob();
}

void MonitorPanel::SetSelectedJob(JobItem *job_item)
{
  m_job_list_widget->SetSelectedJob(job_item);
}

QList<QWidget *> MonitorPanel::GetToolBarWidgets()
{
  QList<QWidget *> result;

  auto submit_button = new QToolButton;
  submit_button->setText("Submit");
  submit_button->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
  submit_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  submit_button->setPopupMode(QToolButton::InstantPopup);
  submit_button->setMenu(m_submit_procedure_menu.get());
  submit_button->setToolTip("Submit sequencer procedure for execution");
  result.push_back(submit_button);

  return result;
}

void MonitorPanel::OnJobSelectedIntern(JobItem *item)
{
  m_job_property_widget->SetJob(item);
  emit JobSelected(item);
}

//! Will be called when insert_af
void MonitorPanel::OnAboutToShowMenu()
{
  qDebug() << "About to show";
}

std::unique_ptr<QMenu> MonitorPanel::CreateSubmitProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  connect(result.get(), &QMenu::aboutToShow, this, &MonitorPanel::OnAboutToShowMenu);
  return result;
}

}  // namespace sequencergui
