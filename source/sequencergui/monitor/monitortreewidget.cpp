/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/monitortreewidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/instructionviewmodel.h"
#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/monitor/monitortreetoolbar.h"

#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequi
{

MonitorTreeWidget::MonitorTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorTreeToolBar)
    , m_splitter(new QSplitter)
    , m_instruction_tree(new ModelView::ItemsTreeView)
    , m_message_panel(new MessagePanel)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_splitter->addWidget(m_instruction_tree);
  m_message_panel->AddToSplitter(m_splitter);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  SetupConnections();
}

MonitorTreeWidget::~MonitorTreeWidget() = default;

void MonitorTreeWidget::SetModel(SequencerModel *model, ProcedureItem *procedure_item)
{
  m_instruction_tree->SetViewModel(std::make_unique<InstructionViewModel>(model));
  if (procedure_item)
  {
    m_instruction_tree->SetRootSessionItem(procedure_item->GetInstructionContainer());
  }
}

void MonitorTreeWidget::SetSelected(const InstructionItem *item)
{
  m_instruction_tree->SetSelected(const_cast<InstructionItem *>(item));
}

MessagePanel *MonitorTreeWidget::GetMessagePanel()
{
  return m_message_panel;
}

void MonitorTreeWidget::SetupConnections()
{
  connect(m_tool_bar, &MonitorTreeToolBar::runRequest, this, &MonitorTreeWidget::runRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::pauseRequest, this, &MonitorTreeWidget::pauseRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::stepRequest, this, &MonitorTreeWidget::stepRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::stopRequest, this, &MonitorTreeWidget::stopRequest);
  connect(m_tool_bar, &MonitorTreeToolBar::changeDelayRequest, this,
          &MonitorTreeWidget::changeDelayRequest);
}

}  // namespace sequi
