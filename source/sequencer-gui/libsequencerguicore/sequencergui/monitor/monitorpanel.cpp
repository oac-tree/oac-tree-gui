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

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/monitor/joblistwidget.h"
#include "sequencergui/widgets/collapsiblelistview.h"
#include "sequencergui/widgets/paneltoolbar.h"

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
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_collapsible_list);

  m_collapsible_list->AddCollapsibleWidget(m_job_list_widget);

  connect(m_job_list_widget, &JobListWidget::procedureSelected, this,
          &MonitorPanel::procedureSelected);
}

MonitorPanel::~MonitorPanel() = default;

void MonitorPanel::SetModel(SequencerModel *model)
{
  m_job_list_widget->SetModel(model);
}

ProcedureItem *MonitorPanel::GetSelectedProcedure() const
{
  return m_job_list_widget->GetSelectedProcedure();
}

void MonitorPanel::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_job_list_widget->SetSelectedProcedure(procedure);
}

}  // namespace sequencergui
