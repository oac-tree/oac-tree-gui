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

#include "sequencergui/composer/composerpanel.h"

#include "sequencergui/composer/instructionlistwidget.h"
#include "sequencergui/composer/instructionpanel.h"
#include "sequencergui/composer/workspacepanel.h"
#include "sequencergui/mainwindow/paneltoolbar.h"
#include "sequencergui/mainwindow/procedurelistview.h"
#include "sequencergui/model/sequenceritems.h"

#include "sequencergui/widgets/collapsiblewidget_v2.h"

#include <QLabel>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{

ComposerPanel::ComposerPanel(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_splitter(new QSplitter)
    , m_procedure_list_widget(new ProcedureListView)
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

  AddToSplitter(m_splitter, m_procedure_list_widget);
  AddToSplitter(m_splitter, m_instruction_panel);
  m_workspace_panel->AddToSplitter(m_splitter);

  //  m_splitter->setSizes(QList<int>() << 400 << 200);

  connect(m_procedure_list_widget, &ProcedureListView::ProcedureSelected, this,
          &ComposerPanel::ProcedureSelected);
  connect(m_procedure_list_widget, &ProcedureListView::CreateNewProcedureRequest, this,
          &ComposerPanel::CreateNewProcedureRequest);
}

ComposerPanel::~ComposerPanel() = default;

void ComposerPanel::SetModel(SequencerModel *model)
{
  m_procedure_list_widget->SetModel(model);
}

ProcedureItem *ComposerPanel::GetSelectedProcedure()
{
  return m_procedure_list_widget->GetSelectedProcedure();
}

void ComposerPanel::SetSelectedProcedure(ProcedureItem *procedure)
{
  return m_procedure_list_widget->SetSelectedProcedure(procedure);
}

}  // namespace sequencergui
