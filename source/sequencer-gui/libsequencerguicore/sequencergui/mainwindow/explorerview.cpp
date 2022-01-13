/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/explorerview.h"

#include "sequencergui/mainwindow/explorertoolbar.h"
#include "sequencergui/mainwindow/filetreeview.h"
#include "sequencergui/mainwindow/opendocumentswidget.h"
#include "sequencergui/model/sequenceritems.h"

#include <QLabel>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{
ExplorerView::ExplorerView(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new ExplorerToolBar)
    , m_splitter(new QSplitter)
    , m_file_tree_view(new FileTreeView)
    , m_open_documents_widget(new OpenDocumentsWidget)
{
  m_tool_bar->setText("EXPLORER");

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_file_tree_view);

//  m_splitter->addWidget(m_open_documents_widget);
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
          &ExplorerView::procedureFileClicked);
  connect(m_file_tree_view, &FileTreeView::procedureFileDoubleClicked, this,
          &ExplorerView::procedureFileDoubleClicked);
  connect(m_open_documents_widget, &OpenDocumentsWidget::createNewProcedureRequest, this,
          &ExplorerView::createNewProcedureRequest);
  connect(m_open_documents_widget, &OpenDocumentsWidget::procedureSelected, this,
          &ExplorerView::sratchpadProcedureSelected);
}

void ExplorerView::SetModel(SequencerModel *model)
{
  m_open_documents_widget->SetModel(model);
}

ExplorerView::~ExplorerView() = default;

}  // namespace sequi
