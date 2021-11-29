/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/monitorpanel.h"

#include "sequencergui/mainwindow/explorertoolbar.h"
#include "sequencergui/mainwindow/opendocumentswidget.h"
#include "sequencergui/model/sequenceritems.h"

#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequi
{

MonitorPanel::MonitorPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new ExplorerToolBar)
    , m_splitter(new QSplitter)
    , m_open_documents_widget(new OpenDocumentsWidget)
{
  m_splitter->setOrientation(Qt::Vertical);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  m_open_documents_widget->AddToSplitter(m_splitter);

  connect(m_open_documents_widget, &OpenDocumentsWidget::procedureSelected, this,
          &MonitorPanel::procedureSelected);
}

MonitorPanel::~MonitorPanel() = default;

void MonitorPanel::SetModel(SequencerModel *model)
{
  m_open_documents_widget->SetModel(model);
}

ProcedureItem *MonitorPanel::GetSelectedProcedure() const
{
  return m_open_documents_widget->GetSelectedProcedure();
}

void MonitorPanel::SetSelectedProcedure(ProcedureItem *procedure)
{
  m_open_documents_widget->SetSelectedProcedure(procedure);
}

}  // namespace sequi
