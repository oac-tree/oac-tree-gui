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

#include "suppvmonitor/monitor_widget.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/monitor_widget_toolbar.h>
#include <suppvmonitor/workspace_monitor_helper.h>
#include <suppvmonitor/workspace_synchronizer.h>
#include <sequencergui/transform/variable_item_transform_utils.h>

#include <mvvm/widgets/all_items_tree_view.h>

#include <sup/sequencer/workspace.h>

#include <QVBoxLayout>

namespace suppvmonitor
{

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWidgetToolBar)
    , m_model(std::make_unique<MonitorModel>())
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
  channel_access_variable->SetJsonType(R"RAW({"type":"uint32"})RAW");
  SetAnyValueFromJsonType(R"RAW({"type":"uint32"})RAW", *channel_access_variable);
}

void MonitorWidget::SetupConnections()
{
  auto on_setup_workspace = [this]()
  {
    m_workspace = std::make_unique<sup::sequencer::Workspace>();

    PopulateDomainWorkspace(*m_model->GetWorkspaceItem(), *m_workspace);
    m_workspace->Setup();

    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(m_model->GetWorkspaceItem(), m_workspace.get());
    m_workspace_synchronizer->Start();
  };
  connect(m_tool_bar, &MonitorWidgetToolBar::SetupWorkspaceRequest, this, on_setup_workspace);
}

}  // namespace suppvmonitor
