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

#include "anyvalue_editor_dialog.h"
#include "monitor_model.h"
#include "monitor_widget_actions.h"
#include "monitor_widget_toolbar.h"
#include "workspace_monitor_helper.h"
#include "workspace_synchronizer.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <sequencergui/widgets/widget_utils.h>

#include <mvvm/widgets/all_items_tree_view.h>

#include <sup/sequencer/workspace.h>

#include <QDebug>
#include <QMessageBox>
#include <QTreeView>
#include <QVBoxLayout>

namespace suppvmonitor
{

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWidgetToolBar)
    , m_model(std::make_unique<MonitorModel>())
    , m_actions(new MonitorWidgetActions(CreateContext(), m_model.get(), this))
    , m_tree_view(new mvvm::AllItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tree_view);

  PopulateModel();
  SetupConnections();

  m_tree_view->SetItem(m_model->GetWorkspaceItem());
}

sequencergui::VariableItem *MonitorWidget::GetSelectedVariable()
{
  return m_tree_view->GetSelected<sequencergui::VariableItem>();
}

MonitorWidget::~MonitorWidget() = default;

void MonitorWidget::PopulateModel()
{
  auto workspace = m_model->InsertItem<sequencergui::WorkspaceItem>();
}

void MonitorWidget::SetupConnections()
{
  auto on_add_variable = [this](const QString &name)
  {
    m_actions->OnAddVariableRequest(name);
    // provide tree adjustment
    if (m_model->GetWorkspaceItem()->GetVariableCount() > 0)
    {
      m_tree_view->GetTreeView()->expandToDepth(2);
      m_tree_view->GetTreeView()->resizeColumnToContents(0);
    }
  };
  connect(m_tool_bar, &MonitorWidgetToolBar::AddVariableRequest, this, on_add_variable);

  connect(m_tool_bar, &MonitorWidgetToolBar::RemoveVariableRequest, this,
          &MonitorWidget::OnRemoveVariableRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::StartMonitoringRequest, this,
          &MonitorWidget::OnStartMonitoringRequest);

  connect(m_tool_bar, &MonitorWidgetToolBar::EditAnyvalueRequest, this,
          &MonitorWidget::OnEditAnyvalueRequest);
}

void MonitorWidget::OnEditAnyvalueRequest()
{
  //  auto selected = GetSelectedVariable();
  //  if(!selected)
  //  {
  //    QMessageBox::warning(this, "Select item", "Please select AnyValue you want to edit");
  //    return;
  //  }

  AnyValueEditorDialog dialog(this);

  if (dialog.exec() == QDialog::Accepted)
  {
  }
}

void MonitorWidget::OnRemoveVariableRequest()
{
  if (auto selected = m_tree_view->GetSelected<sequencergui::VariableItem>(); selected)
  {
    m_model->RemoveItem(selected);
  }
}

void MonitorWidget::OnStartMonitoringRequest()
{
  auto on_start = [this]()
  {
    m_workspace = std::make_unique<sup::sequencer::Workspace>();

    PopulateDomainWorkspace(*m_model->GetWorkspaceItem(), *m_workspace);
    m_workspace->Setup();

    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(m_model->GetWorkspaceItem(), m_workspace.get());
    m_workspace_synchronizer->Start();
  };
  sequencergui::InvokeAndCatch(on_start, "Can't setup workspace");
}

MonitorWidgetContext MonitorWidget::CreateContext()
{
  auto get_selected_callback = [this]()
  { return m_tree_view->GetSelected<sequencergui::VariableItem>(); };

  auto notify_warning_callback = [this](const sup::gui::MessageEvent &event)
  {
    QMessageBox msg_box;
    msg_box.setText(QString::fromStdString(event.text));
    msg_box.setInformativeText(QString::fromStdString(event.informative));
    msg_box.setDetailedText(QString::fromStdString(event.detailed));
    msg_box.setIcon(msg_box.Warning);
    msg_box.exec();
  };

  return {get_selected_callback, notify_warning_callback};
}

}  // namespace suppvmonitor
