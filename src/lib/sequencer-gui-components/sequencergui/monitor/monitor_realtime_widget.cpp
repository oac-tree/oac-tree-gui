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

#include "sequencergui/monitor/monitor_realtime_widget.h"

#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/message_panel.h>
#include <sequencergui/monitor/monitor_realtime_toolbar.h>
#include <sequencergui/monitor/monitor_realtime_tree_widget.h>
#include <sequencergui/nodeeditor/node_editor.h>
#include <sequencergui/viewmodel/instruction_viewmodel.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <mvvm/widgets/collapsible_list_view.h>

#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorRealTimeWidget::MonitorRealTimeWidget(QWidget *parent)
    : QWidget(parent)
    , m_collapsible_list_view(new mvvm::CollapsibleListView)
    , m_stacked_widget(new ItemStackWidget)
    , m_monitor_realtime_widget(new MonitorRealTimeTreeWidget)
    , m_node_editor(new NodeEditor)
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_stacked_widget->AddWidget(m_monitor_realtime_widget, CreateRealTimeToolBar(),
                              /*toolbar_always_visible*/ true);
  m_stacked_widget->AddWidget(m_node_editor, m_node_editor->CreateToolBar());

  m_collapsible_list_view->AddWidget(m_stacked_widget);

  m_collapsible_list_view->AddCollapsibleWidget(m_message_panel, m_message_panel->actions());

  layout->addWidget(m_collapsible_list_view);

  connect(m_monitor_realtime_widget, &MonitorRealTimeTreeWidget::InstructionClicked, this,
          &MonitorRealTimeWidget::InstructionClicked);
}

MonitorRealTimeWidget::~MonitorRealTimeWidget() = default;

void MonitorRealTimeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_monitor_realtime_widget->SetProcedure(procedure_item);
  m_node_editor->SetProcedure(procedure_item);
}

void MonitorRealTimeWidget::SetSelectedInstruction(InstructionItem *item)
{
  m_node_editor->SetSelectedInstructions({item});
  m_monitor_realtime_widget->SetSelectedInstruction(item);
}

MessagePanel *MonitorRealTimeWidget::GetMessagePanel()
{
  return m_message_panel;
}

void MonitorRealTimeWidget::onAppChangeRequest(int id)
{
  m_stacked_widget->SetCurrentIndex(id);
}

std::unique_ptr<MonitorRealTimeToolBar> MonitorRealTimeWidget::CreateRealTimeToolBar()
{
  auto result = std::make_unique<MonitorRealTimeToolBar>();

  // forward signals from a toolbar further up
  connect(result.get(), &MonitorRealTimeToolBar::runRequest, this,
          &MonitorRealTimeWidget::runRequest);
  connect(result.get(), &MonitorRealTimeToolBar::pauseRequest, this,
          &MonitorRealTimeWidget::pauseRequest);
  connect(result.get(), &MonitorRealTimeToolBar::stepRequest, this,
          &MonitorRealTimeWidget::stepRequest);
  connect(result.get(), &MonitorRealTimeToolBar::stopRequest, this,
          &MonitorRealTimeWidget::stopRequest);
  connect(result.get(), &MonitorRealTimeToolBar::changeDelayRequest, this,
          &MonitorRealTimeWidget::changeDelayRequest);

  return result;
}

}  // namespace sequencergui
