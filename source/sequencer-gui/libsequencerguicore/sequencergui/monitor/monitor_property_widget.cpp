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

#include "sequencergui/monitor/monitor_property_widget.h"

#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/property_tree_view.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/widgets/panel_toolbar.h>

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorPropertyWidget::MonitorPropertyWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_workspace_tree(new mvvm::AllItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)

{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_splitter->addWidget(m_workspace_tree);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes(QList<int>() << 300 << 200);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
}

MonitorPropertyWidget::~MonitorPropertyWidget() = default;

void MonitorPropertyWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_workspace_tree->SetItem(procedure_item ? procedure_item->GetWorkspace() : nullptr);
}

void MonitorPropertyWidget::SetSelectedInstruction(InstructionItem *instruction)
{
  m_property_tree->SetItem(instruction);
  m_property_tree->GetTreeView()->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

}  // namespace sequencergui
