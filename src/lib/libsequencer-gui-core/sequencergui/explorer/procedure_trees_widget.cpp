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

#include "sequencergui/explorer/procedure_trees_widget.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/utils/style_utils.h>
#include <sequencergui/widgets/panel_toolbar.h>

#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{
ProcedureTreesWidget::ProcedureTreesWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_procedure_tree(new mvvm::TopItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  auto splitter = new QSplitter;
  splitter->setOrientation(Qt::Vertical);
  splitter->addWidget(m_procedure_tree);
  splitter->addWidget(m_property_tree);
  splitter->setSizes(QList<int>() << 400 << 200);

  layout->addWidget(m_tool_bar);
  layout->addWidget(splitter);

  connect(m_procedure_tree, &mvvm::TopItemsTreeView::SelectedItemChanged, m_property_tree,
          &mvvm::PropertyTreeView::SetItem);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_procedure_tree->GetTreeView());
}

void ProcedureTreesWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_tree->SetItem(procedure_item);
}

}  // namespace sequencergui
