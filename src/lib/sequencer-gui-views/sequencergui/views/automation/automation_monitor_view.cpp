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

#include "automation_monitor_view.h"

#include "automation_monitor_tool_bar.h"

#include <sequencergui/automation/automation_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/views/operation/job_list_widget.h>
#include <sequencergui/views/operation/operation_realtime_panel.h>

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{

AutomationMonitorView::AutomationMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new AutomationMonitorToolBar)
    , m_splitter(new QSplitter)
    , m_job_list(new JobListWidget)
    , m_realtime_panel(new OperationRealTimePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_job_list);
  m_splitter->addWidget(m_realtime_panel);

  // connect(m_line_edit, &QLineEdit::editingFinished, this,
  //         [this]() { qDebug() << m_line_edit->text(); });

  // connect(m_connect_button, &QPushButton::clicked, this, [this](auto) { OnConnect(); });
}

void AutomationMonitorView::SetApplicationModels(ApplicationModels *models)
{
  m_job_list->SetJobModel(models->GetJobModel());
}

AutomationMonitorView::~AutomationMonitorView() = default;

void AutomationMonitorView::OnConnect()
{
  qDebug() << "RemoteMonitorView::OnConnect()";

  // m_automation_manager = std::make_unique<AutomationManager>(m_line_edit->text().toStdString());
}

}  // namespace sequencergui
