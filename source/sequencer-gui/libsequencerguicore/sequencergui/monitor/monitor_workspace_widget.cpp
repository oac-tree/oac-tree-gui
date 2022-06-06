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

#include "sequencergui/monitor/monitor_workspace_widget.h"

#include "sequencergui/model/instruction_container_item.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"
#include "sequencergui/model/workspace_item.h"
#include "sequencergui/monitor/monitor_workspace_toolbar.h"

#include "mvvm/widgets/all_items_tree_view.h"

#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

MonitorWorkspaceWidget::MonitorWorkspaceWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new MonitorWorkspaceToolBar)
    , m_instruction_tree(new mvvm::AllItemsTreeView)
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

void MonitorWorkspaceWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_instruction_tree->SetItem(procedure_item->GetWorkspace());
}

}  // namespace sequencergui
