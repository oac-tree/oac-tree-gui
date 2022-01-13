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

#include "sequencergui/composer/composerpanel.h"

#include "sequencergui/composer/instructionpanel.h"
#include "sequencergui/composer/workspacepanel.h"
#include "sequencergui/mainwindow/explorertoolbar.h"
#include "sequencergui/mainwindow/opendocumentswidget.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/nodeeditor/instructionlistwidget.h"

#include <QLabel>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

ComposerPanel::ComposerPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new ExplorerToolBar)
    , m_splitter(new QSplitter)
    , m_open_documents_widget(new OpenDocumentsWidget)
    , m_instruction_panel(new InstructionPanel)
    , m_workspace_panel(new WorkspacePanel)

{
  m_splitter->setOrientation(Qt::Vertical);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  m_open_documents_widget->AddToSplitter(m_splitter);
  m_instruction_panel->AddToSplitter(m_splitter);
  m_workspace_panel->AddToSplitter(m_splitter);

  //  m_splitter->setSizes(QList<int>() << 400 << 200);

  connect(m_open_documents_widget, &OpenDocumentsWidget::procedureSelected, this,
          &ComposerPanel::procedureSelected);
}

ComposerPanel::~ComposerPanel() = default;

void ComposerPanel::SetModel(SequencerModel *model)
{
  m_open_documents_widget->SetModel(model);
}

}  // namespace sequi
