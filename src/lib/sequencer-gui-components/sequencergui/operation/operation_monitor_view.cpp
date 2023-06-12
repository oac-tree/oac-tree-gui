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

#include "operation_monitor_view.h"

#include "operation_job_panel.h"
#include "operation_realtime_panel.h"

#include <sequencergui/monitor/monitor_property_widget.h>
#include <sequencergui/monitor/monitor_realtime_widget.h>

#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{

OperationMonitorView::OperationMonitorView(QMainWindow *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_job_panel(new OperationJobPanel)
    , m_realtime_widget(new OperationRealTimePanel)
    , m_property_widget(new MonitorPropertyWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter->addWidget(m_job_panel);
  m_splitter->addWidget(m_realtime_widget);
  m_splitter->addWidget(m_property_widget);
  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(30) << mvvm::utils::UnitSize(90)
                                    << mvvm::utils::UnitSize(30));

  parent->addToolBar(m_job_panel->GetToolBar());
  parent->addToolBar(m_realtime_widget->GetToolBar());

  layout->addWidget(m_splitter);
}

OperationMonitorView::~OperationMonitorView() = default;

}  // namespace sequencergui
