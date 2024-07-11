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

#include "operation_realtime_panel.h"

#include "message_panel.h"
#include "monitor_realtime_actions.h"
#include "realtime_instruction_tree_widget.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("OperationRealTimePanel");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

QList<QAction *> GetToolBarActions(sequencergui::MonitorRealTimeActions *actions)
{
  using ActionKey = sequencergui::MonitorRealTimeActions::ActionKey;
  return actions->GetActions({ActionKey::kRun, ActionKey::kPause, ActionKey::kStep,
                              ActionKey::kStop, ActionKey::kDelay, ActionKey::kSettings});
}

}  // namespace

namespace sequencergui
{

OperationRealTimePanel::OperationRealTimePanel(QWidget *parent)
    : QWidget(parent)
    , m_actions(new MonitorRealTimeActions(this))
    , m_collapsible_list(new sup::gui::CollapsibleListView)
    , m_realtime_instruction_tree(new RealTimeInstructionTreeWidget)
    , m_message_panel(new MessagePanel)
{
  setWindowTitle("Operations");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_collapsible_list->AddWidget(m_realtime_instruction_tree);

  m_collapsible_list->AddCollapsibleWidget(m_message_panel, m_message_panel->actions());

  layout->addWidget(m_collapsible_list);

  SetupConnections();
  ReadSettings();

  addActions(GetToolBarActions(m_actions));
}

OperationRealTimePanel::~OperationRealTimePanel()
{
  WriteSettings();
}

void OperationRealTimePanel::SetCurrentJob(JobItem *job_item)
{
  if (job_item)
  {
    m_realtime_instruction_tree->SetProcedure(job_item->GetExpandedProcedure());
    m_actions->SetCurrentTickTimeout(job_item->GetTickTimeout());
  }
  else
  {
    m_realtime_instruction_tree->SetProcedure(nullptr);
  }
}

void OperationRealTimePanel::SetSelectedInstructions(std::vector<InstructionItem *> items)
{
  m_realtime_instruction_tree->SetSelectedInstructions(items);
}

MessagePanel *OperationRealTimePanel::GetMessagePanel()
{
  return m_message_panel;
}

int OperationRealTimePanel::GetCurrentTickTimeout()
{
  return m_actions->GetCurrentTickTimeout();
}

void OperationRealTimePanel::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_collapsible_list->GetSplitter()->restoreState(
        settings.value(kSplitterSettingName).toByteArray());
  }
}

void OperationRealTimePanel::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_collapsible_list->GetSplitter()->saveState());
}

void OperationRealTimePanel::SetupConnections()
{
  // forward signals from a toolbar further up
  connect(m_actions, &MonitorRealTimeActions::RunRequest, this,
          &OperationRealTimePanel::runRequest);
  connect(m_actions, &MonitorRealTimeActions::PauseRequest, this,
          &OperationRealTimePanel::pauseRequest);
  connect(m_actions, &MonitorRealTimeActions::StepRequest, this,
          &OperationRealTimePanel::stepRequest);
  connect(m_actions, &MonitorRealTimeActions::StopRequest, this,
          &OperationRealTimePanel::stopRequest);
  connect(m_actions, &MonitorRealTimeActions::ChangeDelayRequest, this,
          &OperationRealTimePanel::changeDelayRequest);

  connect(m_realtime_instruction_tree, &RealTimeInstructionTreeWidget::ToggleBreakpointRequest,
          this, &OperationRealTimePanel::ToggleBreakpointRequest);
  connect(m_actions, &MonitorRealTimeActions::ScrollToSelectionRequest, m_realtime_instruction_tree,
          &RealTimeInstructionTreeWidget::SetViewportFollowsSelectionFlag);
}

}  // namespace sequencergui
