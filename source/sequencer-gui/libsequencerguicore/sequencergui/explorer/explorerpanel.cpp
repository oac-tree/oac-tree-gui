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

#include "sequencergui/explorer/explorerpanel.h"

#include "sequencergui/components/procedurelistwidget.h"
#include "sequencergui/model/procedureitem.h"
#include "sequencergui/widgets/collapsiblelistview.h"
#include "sequencergui/widgets/filetreeview.h"
#include "sequencergui/widgets/paneltoolbar.h"

#include <QLabel>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{
ExplorerPanel::ExplorerPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_collapsible_list(new CollapsibleListView)
    , m_file_tree_view(new FileTreeView)
    , m_procedure_list_view(new ProcedureListWidget)
{
  m_tool_bar->setText("EXPLORER");

  m_collapsible_list->AddWidget(m_file_tree_view);

  m_procedure_list_view->SetupActions(ProcedureListWidget::kRemoveSelected);

  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view, m_procedure_list_view->actions());

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_collapsible_list);

  connect(m_file_tree_view, &FileTreeView::ProcedureFileClicked, this,
          &ExplorerPanel::ProcedureFileClicked);
  connect(m_file_tree_view, &FileTreeView::ProcedureFileDoubleClicked, this,
          &ExplorerPanel::ProcedureFileDoubleClicked);
  connect(m_procedure_list_view, &ProcedureListWidget::ProcedureSelected, this,
          &ExplorerPanel::ProcedureSelected);
  connect(m_procedure_list_view, &ProcedureListWidget::CreateNewProcedureRequest, this,
          &ExplorerPanel::CreateNewProcedureRequest);
  connect(m_procedure_list_view, &ProcedureListWidget::RemoveProcedureRequest, this,
          &ExplorerPanel::RemoveProcedureRequest);
}

void ExplorerPanel::SetModel(SequencerModel *model)
{
  m_procedure_list_view->SetModel(model);
}

ProcedureItem *ExplorerPanel::GetSelectedProcedure()
{
  return m_procedure_list_view->GetSelectedProcedure();
}

ExplorerPanel::~ExplorerPanel() = default;

}  // namespace sequencergui
