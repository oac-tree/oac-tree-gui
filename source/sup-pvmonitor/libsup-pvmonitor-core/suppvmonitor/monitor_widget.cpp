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

#include "suppvmonitor/monitor_widget.h"

#include <mvvm/widgets/all_items_tree_view.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/monitor_widget_toolbar.h>
#include <suppvmonitor/workspace_controller.h>

#include <QVBoxLayout>

namespace suppvmonitor
{

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWidgetToolBar)
    , m_model(std::make_unique<MonitorModel>())
    , m_workspace_controller(std::make_unique<WorkspaceSyncronizer>(m_model.get()))
    , m_tree_view(new mvvm::AllItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tree_view);

  PopulateModel();
  SetupConnections();

  m_tree_view->SetApplicationModel(m_model.get());
}

MonitorWidget::~MonitorWidget() = default;

void MonitorWidget::PopulateModel()
{
  auto workspace = m_model->InsertItem<sequencergui::WorkspaceItem>();

  auto channel_access_variable =
      workspace->InsertItem<sequencergui::ChannelAccessVariableItem>(mvvm::TagIndex::Append());
  channel_access_variable->SetName("var1");
  channel_access_variable->SetChannel("IS_RUNNING");
  channel_access_variable->SetDataType(R"RAW({"type":"uint32"})RAW");
}

void MonitorWidget::SetupConnections()
{
  connect(m_tool_bar, &MonitorWidgetToolBar::SetupWorkspaceRequest, m_workspace_controller.get(),
          &WorkspaceSyncronizer::OnSetupWorkspaceRequest);
}

}  // namespace suppvmonitor
