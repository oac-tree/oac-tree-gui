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

#include "monitor_widget.h"

#include "monitor_model.h"
#include "monitor_widget_actions.h"
#include "workspace_editor_context.h"
#include "workspace_monitor_helper.h"
#include "workspace_synchronizer.h"

#include <sequencergui/components/anyvalue_editor_dialog_factory.h>
#include <sequencergui/composer/workspace_editor_widget.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/core/message_helper.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <sup/sequencer/workspace.h>

#include <QVBoxLayout>

namespace sequencergui
{

MonitorWidget::MonitorWidget(MonitorModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_actions(new MonitorWidgetActions(this))
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_stack_widget);

  auto control_actions = m_actions->GetActions({MonitorWidgetActions::ActionKey::kStartWorkspace,
                                                MonitorWidgetActions::ActionKey::kStopWorkspace});
  m_stack_widget->AddWidget(m_workspace_editor, m_workspace_editor->actions() + control_actions);

  SetupConnections();  // should be after tree view got its model
}

MonitorWidget::~MonitorWidget() = default;

void MonitorWidget::SetWorkspaceItem(WorkspaceItem *item)
{
  m_workspace_editor->SetWorkspaceItem(item);
}

void MonitorWidget::SetupConnections()
{
  connect(m_actions, &MonitorWidgetActions::StartMonitoringRequest, this,
          &MonitorWidget::OnStartMonitoringRequest);

  connect(m_actions, &MonitorWidgetActions::StopMonitoringRequest, this,
          &MonitorWidget::OnStopMonitoringRequest);
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
  }
  catch (std::exception &ex)
  {
    sup::gui::SendWarningMessage({"Setup failed", "Can't setup workspace", ex.what()});
    UpdateVariableEditableProperty(false, *m_model->GetWorkspaceItem());
  }
}

void MonitorWidget::OnStopMonitoringRequest()
{
  m_workspace_synchronizer->Shutdown();
}

WorkspaceEditorContext MonitorWidget::CreateContext()
{
  auto selected_workspace_callback = [this]() { return m_model->GetWorkspaceItem(); };

  auto selected_item_callback = [this]() { return m_workspace_editor->GetSelectedItem(); };

  auto send_message_callback = [](const auto &event)
  { return sup::gui::SendWarningMessage(event); };

  return {selected_workspace_callback, selected_item_callback, send_message_callback,
          CreateAnyValueDialogCallback(this)};
}

}  // namespace sequencergui
