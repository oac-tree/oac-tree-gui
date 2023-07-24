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

#include "operation_realtime_panel.h"

#include "message_panel.h"
#include "monitor_realtime_toolbar.h"
#include "realtime_instruction_tree_widget.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <mvvm/widgets/collapsible_list_view.h>

#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("OperationRealTimePanel");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
}  // namespace

namespace sequencergui
{

OperationRealTimePanel::OperationRealTimePanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorRealTimeToolBar)
    , m_collapsible_list(new mvvm::CollapsibleListView)
    , m_realtime_instruction_tree(new RealTimeInstructionTreeWidget)
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_collapsible_list->AddWidget(m_realtime_instruction_tree);

  m_collapsible_list->AddCollapsibleWidget(m_message_panel, m_message_panel->actions());

  layout->addWidget(m_collapsible_list);

  SetupConnections();
  ReadSettings();
}

OperationRealTimePanel::~OperationRealTimePanel()
{
  WriteSettings();
}

void OperationRealTimePanel::SetProcedure(ProcedureItem *procedure_item)
{
  m_realtime_instruction_tree->SetProcedure(procedure_item);
}

void OperationRealTimePanel::SetSelectedInstruction(InstructionItem *item)
{
  m_realtime_instruction_tree->SetSelectedInstruction(item);
}

void OperationRealTimePanel::SetSelectedInstructions(std::vector<InstructionItem *> items)
{
  m_realtime_instruction_tree->SetSelectedInstructions(items);
}

MessagePanel *OperationRealTimePanel::GetMessagePanel()
{
  return m_message_panel;
}

QToolBar *OperationRealTimePanel::GetToolBar() const
{
  return m_tool_bar;
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
  connect(m_tool_bar, &MonitorRealTimeToolBar::runRequest, this,
          &OperationRealTimePanel::runRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::pauseRequest, this,
          &OperationRealTimePanel::pauseRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::stepRequest, this,
          &OperationRealTimePanel::stepRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::stopRequest, this,
          &OperationRealTimePanel::stopRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::changeDelayRequest, this,
          &OperationRealTimePanel::changeDelayRequest);

  connect(m_realtime_instruction_tree, &RealTimeInstructionTreeWidget::InstructionClicked, this,
          &OperationRealTimePanel::InstructionClicked);
  connect(m_realtime_instruction_tree, &RealTimeInstructionTreeWidget::ToggleBreakpointRequest,
          this, &OperationRealTimePanel::ToggleBreakpointRequest);
}

}  // namespace sequencergui
