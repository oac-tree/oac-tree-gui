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
#include "sequencergui/mainwindow/opendocumentswidget.h"
#include "sequencergui/mainwindow/paneltoolbar.h"
#include "sequencergui/model/sequenceritems.h"

#include <QLabel>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{
ExplorerPanel::ExplorerPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_splitter(new QSplitter)
    , m_file_tree_view(new FileTreeView)
    , m_open_documents_widget(new OpenDocumentsWidget)
{
  m_tool_bar->setText("EXPLORER");

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_file_tree_view);

  m_open_documents_widget->SetText("OPEN PROCEDURES");
  m_open_documents_widget->AddToSplitter(m_splitter);

  m_splitter->setCollapsible(m_splitter->indexOf(m_open_documents_widget), false);

  m_splitter->setSizes(QList<int>() << 400 << 200);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  connect(m_file_tree_view, &FileTreeView::procedureFileClicked, this,
          &ExplorerPanel::procedureFileClicked);
  connect(m_file_tree_view, &FileTreeView::procedureFileDoubleClicked, this,
          &ExplorerPanel::procedureFileDoubleClicked);
  connect(m_open_documents_widget, &OpenDocumentsWidget::createNewProcedureRequest, this,
          &ExplorerPanel::createNewProcedureRequest);
  connect(m_open_documents_widget, &OpenDocumentsWidget::procedureSelected, this,
          &ExplorerPanel::procedureSelected);
}

void ExplorerPanel::SetModel(SequencerModel *model)
{
  m_open_documents_widget->SetModel(model);
}

ProcedureItem *ExplorerPanel::GetSelectedProcedure()
{
  return m_open_documents_widget->GetSelectedProcedure();
}

ExplorerPanel::~ExplorerPanel() = default;

}  // namespace sequencergui
