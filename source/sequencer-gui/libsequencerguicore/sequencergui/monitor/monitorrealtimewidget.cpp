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

#include "sequencergui/monitor/monitorrealtimewidget.h"

#include "sequencergui/model/procedureitem.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/monitor/instructionviewmodel.h"
#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/monitor/monitorrealtimetoolbar.h"
#include "sequencergui/nodeeditor/nodeeditor.h"
#include "sequencergui/widgets/collapsiblelistview.h"

#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QStackedWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

class MVVM_VIEW_EXPORT JobTreeView : public mvvm::AbstractItemView
{
public:
  explicit JobTreeView(mvvm::ApplicationModel *model = nullptr, QWidget *parent = nullptr)
      : AbstractItemView(mvvm::CreateViewModel<InstructionViewModel>, new QTreeView, model, parent)
  {
  }
};

MonitorRealTimeWidget::MonitorRealTimeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorRealTimeToolBar)
    , m_splitter(new CollapsibleListView)
    , m_stacked_widget(new QStackedWidget)
    , m_instruction_tree(new JobTreeView)
    , m_node_editor(new NodeEditor(Qt::RightToolBarArea))
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);

  m_stacked_widget->addWidget(m_instruction_tree);

  m_splitter->AddWidget(m_stacked_widget);
  m_stacked_widget->addWidget(m_node_editor);

  m_splitter->AddCollapsibleWidget(m_message_panel);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  SetupConnections();
}

MonitorRealTimeWidget::~MonitorRealTimeWidget() = default;

void MonitorRealTimeWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_instruction_tree->SetItem(procedure_item);
}

void MonitorRealTimeWidget::SetSelectedInstruction(InstructionItem *item)
{
  m_instruction_tree->SetSelectedItem(item);
  m_node_editor->SetSelectedInstructions({item});
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
  connect(m_tool_bar, &MonitorRealTimeToolBar::appChangeRequest, this,
          &MonitorRealTimeWidget::onAppChangeRequest);
}

}  // namespace sequencergui
