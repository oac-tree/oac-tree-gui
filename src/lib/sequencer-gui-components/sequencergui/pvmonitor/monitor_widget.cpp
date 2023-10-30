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

#include "monitor_widget.h"

#include "monitor_model.h"
#include "monitor_widget_toolbar.h"
#include "workspace_editor_action_handler.h"
#include "workspace_monitor_helper.h"
#include "workspace_synchronizer.h"

#include <sequencergui/components/message_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/widgets/all_items_tree_view.h>

#include <sup/sequencer/workspace.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorWidget::MonitorWidget(MonitorModel *model, QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWidgetToolBar)
    , m_model(model)
    , m_workspace_editor_action_handler(new WorkspaceEditorActionHandler(CreateContext(), this))
    , m_tree_view(new mvvm::AllItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tree_view);

  m_tree_view->SetItem(m_model->GetWorkspaceItem());
  SetupConnections();  // should be after tree view got its model
}

MonitorWidget::~MonitorWidget() = default;

//! Returns underlying view model to which QTreeView is pointing.

mvvm::ViewModel *MonitorWidget::GetViewModel()
{
  return dynamic_cast<mvvm::ViewModel *>(m_tree_view->GetTreeView()->model());
}

void MonitorWidget::SetupConnections()
{
  connect(m_tool_bar, &MonitorWidgetToolBar::AddVariableRequest, m_workspace_editor_action_handler,
          &WorkspaceEditorActionHandler::OnAddVariableRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::EditAnyvalueRequest, m_workspace_editor_action_handler,
          &WorkspaceEditorActionHandler::OnEditAnyvalueRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::RemoveVariableRequest,
          m_workspace_editor_action_handler,
          &WorkspaceEditorActionHandler::OnRemoveVariableRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::StartMonitoringRequest, this,
          &MonitorWidget::OnStartMonitoringRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::StopMonitoringRequest, this,
          &MonitorWidget::OnStopMonitoringRequest);

  // make inserted item selected, and tree branch expanded
  auto on_select_variable_request = [this](auto item)
  {
    m_tree_view->SetSelectedItem(item);

    auto index_of_inserted = GetViewModel()->GetIndexOfSessionItem(item);
    if (!index_of_inserted.empty())
    {
      m_tree_view->GetTreeView()->setExpanded(index_of_inserted.front(), true);
    }
  };
  connect(m_workspace_editor_action_handler, &WorkspaceEditorActionHandler::SelectItemRequest, this,
          on_select_variable_request);
}

void MonitorWidget::OnStartMonitoringRequest()
{
  try
  {
    m_workspace = std::make_unique<workspace_t>();

    PopulateDomainWorkspace(*m_model->GetWorkspaceItem(), *m_workspace);

    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(m_model->GetWorkspaceItem(), m_workspace.get());
    m_workspace_synchronizer->Start();
    m_tool_bar->UpdateActionsState(true);
  }
  catch (std::exception &ex)
  {
    SendWarningMessage({"Setup failed", "Can't setup workspace", ex.what()});
    UpdateVariableEditableProperty(false, *m_model->GetWorkspaceItem());
  }
}

void MonitorWidget::OnStopMonitoringRequest()
{
  m_workspace_synchronizer->Shutdown();
  m_tool_bar->UpdateActionsState(false);
}

WorkspaceEditorContext MonitorWidget::CreateContext()
{
  auto selected_workspace_callback = [this]() { return m_model->GetWorkspaceItem(); };

  auto selected_item_callback = [this]() { return m_tree_view->GetSelectedItem(); };

  auto send_message_callback = [](const auto &event) { return SendWarningMessage(event); };

  return {selected_workspace_callback, selected_item_callback, send_message_callback,
          CreateAnyValueDialogCallback()};
}

}  // namespace sequencergui
