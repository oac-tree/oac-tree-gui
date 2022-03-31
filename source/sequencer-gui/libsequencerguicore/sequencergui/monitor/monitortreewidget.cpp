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

#include "sequencergui/monitor/monitortreewidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/instructionviewmodel.h"
#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/monitor/monitortreetoolbar.h"
#include "sequencergui/nodeeditor/nodeeditor.h"

#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QStackedWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorRealTimeWidget::MonitorRealTimeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorTreeToolBar)
    , m_splitter(new QSplitter)
    , m_stacked_widget(new QStackedWidget)
    , m_instruction_tree(new mvvm::ItemsTreeView)
    , m_node_editor(new NodeEditor(Qt::RightToolBarArea))
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_stacked_widget->addWidget(m_instruction_tree);

  m_splitter->addWidget(m_stacked_widget);
  m_stacked_widget->addWidget(m_node_editor);

  m_message_panel->AddToSplitter(m_splitter);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  SetupConnections();
}

MonitorRealTimeWidget::~MonitorRealTimeWidget() = default;

void MonitorRealTimeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  if (procedure_item)
  {
    auto model = dynamic_cast<SequencerModel*>(procedure_item->GetModel());
    m_instruction_tree->SetViewModel(std::make_unique<InstructionViewModel>(model));
    m_instruction_tree->SetRootSessionItem(procedure_item->GetInstructionContainer());
    m_node_editor->SetModel(model);
    m_node_editor->SetProcedure(procedure_item);
  }
  else
  {
    m_instruction_tree->Reset();
  }
}

void MonitorRealTimeWidget::SetSelectedInstruction(const InstructionItem *item)
{
  auto instruction = const_cast<InstructionItem *>(item);
  m_instruction_tree->SetSelected(instruction);
  m_node_editor->SetSelectedInstructions({instruction});
}

MessagePanel *MonitorRealTimeWidget::GetMessagePanel()
{
  return m_message_panel;
}

void MonitorRealTimeWidget::onAppChangeRequest(int id)
{
  m_stacked_widget->setCurrentIndex(id);
}

void MonitorRealTimeWidget::SetupConnections()
{
  connect(m_tool_bar, &MonitorTreeToolBar::runRequest, this, &MonitorRealTimeWidget::runRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::pauseRequest, this, &MonitorRealTimeWidget::pauseRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::stepRequest, this, &MonitorRealTimeWidget::stepRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::stopRequest, this, &MonitorRealTimeWidget::stopRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::changeDelayRequest, this,
          &MonitorRealTimeWidget::changeDelayRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::appChangeRequest, this,
          &MonitorRealTimeWidget::onAppChangeRequest);
}

}  // namespace sequencergui
