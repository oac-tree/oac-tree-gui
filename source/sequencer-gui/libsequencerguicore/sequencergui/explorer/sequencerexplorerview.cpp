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

#include "sequencergui/explorer/sequencerexplorerview.h"

#include "sequencergui/explorer/explorerpanel.h"
#include "sequencergui/explorer/proceduretreeswidget.h"
#include "sequencergui/explorer/xmleditor.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/xmlutils.h"
#include "sequencergui/utils/widgetutils.h"

#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{
SequencerExplorerView::SequencerExplorerView(QWidget *parent)
    : QWidget(parent)
    , m_explorer_panel(new ExplorerPanel)
    , m_trees_widget(new ProcedureTreesWidget)
    , m_xml_editor(new XMLEditor)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_splitter->addWidget(m_explorer_panel);
  m_splitter->addWidget(m_trees_widget);
  m_splitter->addWidget(m_xml_editor);

  layout->addWidget(m_splitter);

  SetupConnections();
}

SequencerExplorerView::~SequencerExplorerView() = default;

//! Sets the model.
void SequencerExplorerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_explorer_panel->SetModel(model);

  // Provide regeneration of XML text corresponding to the currently opened procedure, on every
  // change in the model.
  auto on_data_change = [this](auto, auto)
  {
    if (auto procedure_item = m_explorer_panel->GetSelectedProcedure(); procedure_item)
    {
      m_xml_editor->SetXMLContent(QString::fromStdString(ExportToXMLString(procedure_item)));
    }
  };
  m_model->GetSubscriber()->SetOnDataChanged(on_data_change);
}

//! Show content of XML file.
//! - Show XML in corresponding editor.
//! - Generates temporary Procedure from XML and show object tree.
void SequencerExplorerView::ShowXMLFile(const QString &file_name)
{
  m_xml_editor->SetXMLFile(file_name);

  m_temp_model = std::make_unique<SequencerModel>();

  auto procedure_item = m_temp_model->InsertItem<ProcedureItem>();

  auto on_import = [file_name, procedure_item]()
  { ImportFromFile(file_name.toStdString(), procedure_item); };

  if (auto result = InvokeAndCatch(on_import); result)
  {
    m_trees_widget->SetProcedure(procedure_item);
  }
  else
  {
    m_trees_widget->SetProcedure(nullptr);
  }
  //  on_import();
  //  m_trees_widget->SetModel(m_model.get(), procedure_item);
}

//! Show selected procedure in widgets.
//! - Generates XML representing a procedure and show it in editor.
//! - Show object tree in widgets.
void SequencerExplorerView::ShowSelectedProcedure(ProcedureItem *procedure_item)
{
  qDebug() << "onSratchpadProcedureSelected" << procedure_item;
  if (procedure_item)
  {
    m_xml_editor->SetXMLContent(QString::fromStdString(ExportToXMLString(procedure_item)));
    m_trees_widget->SetProcedure(procedure_item);
  }
  else
  {
    m_xml_editor->ClearText();
    m_trees_widget->SetProcedure(nullptr);
  }
}

//! Import procedure from file and add it to the container.

void SequencerExplorerView::ImportProcedureFromFile(const QString &file_name)
{
  auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
  qDebug() << "inserting new procedure" << procedure_item;

  auto on_import = [file_name, procedure_item]()
  { ImportFromFile(file_name.toStdString(), procedure_item); };
  InvokeAndCatch(on_import);
}

void SequencerExplorerView::CreateNewProcedure()
{
  auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
}

void SequencerExplorerView::SetupConnections()
{
  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileClicked, this,
          &SequencerExplorerView::ShowXMLFile);

  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileDoubleClicked, this,
          &SequencerExplorerView::ImportProcedureFromFile);

  connect(m_explorer_panel, &ExplorerPanel::CreateNewProcedureRequest, this,
          &SequencerExplorerView::CreateNewProcedure);

  connect(m_explorer_panel, &ExplorerPanel::ProcedureSelected, this,
          &SequencerExplorerView::ShowSelectedProcedure);
}

}  // namespace sequencergui
