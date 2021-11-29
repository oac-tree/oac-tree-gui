/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/monitorworkspacewidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/monitorworkspacetoolbar.h"

#include "mvvm/widgets/allitemstreeview.h"

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequi
{

MonitorWorkspaceWidget::MonitorWorkspaceWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWorkspaceToolBar)
    , m_instruction_tree(new ModelView::AllItemsTreeView)
    , m_splitter(new QSplitter)

{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_splitter->addWidget(m_instruction_tree);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
}

MonitorWorkspaceWidget::~MonitorWorkspaceWidget() = default;

void MonitorWorkspaceWidget::SetModel(SequencerModel *model, ProcedureItem *procedure_item)
{
  m_instruction_tree->SetApplicationModel(model);
  if (procedure_item)
  {
    m_instruction_tree->SetRootSessionItem(procedure_item->GetWorkspace());
  }
}

}  // namespace sequi
