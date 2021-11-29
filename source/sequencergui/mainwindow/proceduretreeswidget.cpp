/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/proceduretreeswidget.h"

#include "sequencergui/mainwindow/explorertoolbar.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/widgets/propertytreeview.h"
#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequi
{
ProcedureTreesWidget::ProcedureTreesWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new ExplorerToolBar)
    , m_procedure_tree(new ModelView::TopItemsTreeView)
    , m_property_tree(new ModelView::PropertyTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);

  auto splitter = new QSplitter;
  splitter->setOrientation(Qt::Vertical);
  splitter->addWidget(m_procedure_tree);
  splitter->addWidget(m_property_tree);
  splitter->setSizes(QList<int>() << 400 << 200);

  layout->addWidget(m_tool_bar);
  layout->addWidget(splitter);

  connect(m_procedure_tree, &ModelView::TopItemsTreeView::itemSelected,
          [this](auto item) { m_property_tree->SetItem(item); });
}

void ProcedureTreesWidget::SetModel(SequencerModel *model, ProcedureItem *procedure_item)
{
  m_procedure_tree->SetApplicationModel(model);
  if (procedure_item)
  {
    m_procedure_tree->SetRootSessionItem(procedure_item);
  }
  m_procedure_tree->GetTreeView()->setAlternatingRowColors(true);

  m_property_tree->SetItem(procedure_item);
  m_property_tree->GetTreeView()->expandAll();
  m_property_tree->GetTreeView()->setAlternatingRowColors(true);
}

}  // namespace sequi
