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
#include "sequencergui/widgets/collapsiblelistview.h"
#include "sequencergui/widgets/paneltoolbar.h"

#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorPanel::MonitorPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_collapsible_list(new CollapsibleListView)
    , m_job_list_widget(new JobListWidget)
    , m_job_property_widget(new JobPropertyWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_collapsible_list);

  m_collapsible_list->AddCollapsibleWidget(m_job_list_widget, m_job_list_widget->actions());
  m_collapsible_list->AddCollapsibleWidget(m_job_property_widget, m_job_property_widget->actions());

  connect(m_job_list_widget, &JobListWidget::JobSelected, this, &MonitorPanel::OnJobSelectedIntern);
}

MonitorPanel::~MonitorPanel() = default;

void MonitorPanel::SetApplicationModels(ApplicationModels *models)
{
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

void MonitorPanel::OnJobSelectedIntern(JobItem *item)
{
  m_job_property_widget->SetJob(item);
  emit JobSelected(item);
}

}  // namespace sequencergui
