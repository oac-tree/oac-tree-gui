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

#include "monitor_realtime_widget.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/message_panel.h>
#include <sequencergui/monitor/monitor_realtime_toolbar.h>
#include <sequencergui/monitor/realtime_instruction_tree_widget.h>
#include <sequencergui/nodeeditor/node_editor.h>
#include <sequencergui/viewmodel/instruction_viewmodel.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <mvvm/widgets/collapsible_list_view.h>

#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

MonitorRealTimeWidget::MonitorRealTimeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorRealTimeToolBar)
    , m_tool_bar_action(new QWidgetAction(this))
    , m_collapsible_list_view(new mvvm::CollapsibleListView)
    , m_stacked_widget(new ItemStackWidget)
    , m_realtime_instruction_tree(new RealTimeInstructionTreeWidget)
    , m_node_editor(new NodeEditor)
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupToolBar();

  m_stacked_widget->AddWidget(m_realtime_instruction_tree, {m_tool_bar_action},
                              /*toolbar_always_visible*/ true);
  m_stacked_widget->AddWidget(m_node_editor, m_node_editor->actions());

  m_collapsible_list_view->AddWidget(m_stacked_widget);

  m_collapsible_list_view->AddCollapsibleWidget(m_message_panel, m_message_panel->actions());

  layout->addWidget(m_collapsible_list_view);

  SetupConnections();
}

MonitorRealTimeWidget::~MonitorRealTimeWidget() = default;

void MonitorRealTimeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_realtime_instruction_tree->SetProcedure(procedure_item);
  m_node_editor->SetProcedure(procedure_item);
}

void MonitorRealTimeWidget::SetSelectedInstruction(InstructionItem *item)
{
  m_node_editor->SetSelectedInstructions({item});
  m_realtime_instruction_tree->SetSelectedInstruction(item);
}

MessagePanel *MonitorRealTimeWidget::GetMessagePanel()
{
  return m_message_panel;
}

void MonitorRealTimeWidget::onAppChangeRequest(int id)
{
  m_stacked_widget->SetCurrentIndex(id);
}

void MonitorRealTimeWidget::SetupConnections()
{
  // forward signals from a toolbar further up
  connect(m_tool_bar, &MonitorRealTimeToolBar::runRequest, this,
          &MonitorRealTimeWidget::runRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::pauseRequest, this,
          &MonitorRealTimeWidget::pauseRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::stepRequest, this,
          &MonitorRealTimeWidget::stepRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::stopRequest, this,
          &MonitorRealTimeWidget::stopRequest);
  connect(m_tool_bar, &MonitorRealTimeToolBar::changeDelayRequest, this,
          &MonitorRealTimeWidget::changeDelayRequest);

  connect(m_realtime_instruction_tree, &RealTimeInstructionTreeWidget::InstructionClicked, this,
          &MonitorRealTimeWidget::InstructionClicked);
}

//! Setup a toolbar so it can be used via widget's action mechanism.

void MonitorRealTimeWidget::SetupToolBar()
{
  // remove extra spacing so it can be embedded into another toolbar
  m_tool_bar->layout()->setContentsMargins(0, 0, 0, 0);
  m_tool_bar->layout()->setSpacing(0);

  // add toolbar to the list of widgert's action
  m_tool_bar_action->setDefaultWidget(m_tool_bar);
  addAction(m_tool_bar_action);
}

}  // namespace sequencergui
