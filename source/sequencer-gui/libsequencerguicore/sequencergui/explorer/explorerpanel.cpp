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

#include "sequencergui/mainwindow/filetreeview.h"
#include "sequencergui/mainwindow/paneltoolbar.h"
#include "sequencergui/mainwindow/procedurelistview.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/widgets/collapsiblelistview.h"

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
    , m_procedure_list_view(new ProcedureListView)
{
  m_tool_bar->setText("EXPLORER");

  m_collapsible_list->AddWidget(m_file_tree_view);

  m_procedure_list_view->setWindowTitle("OPEN PROCEDURES");
  m_collapsible_list->AddCollapsibleWidget(m_procedure_list_view);

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
  connect(m_procedure_list_view, &ProcedureListView::ProcedureSelected, this,
          &ExplorerPanel::ProcedureSelected);
  connect(m_procedure_list_view, &ProcedureListView::CreateNewProcedureRequest, this,
          &ExplorerPanel::CreateNewProcedureRequest);
  connect(m_procedure_list_view, &ProcedureListView::RemoveProcedureRequest, this,
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
