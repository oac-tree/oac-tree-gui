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
#include "sequencergui/widgets/widgetutils.h"

#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <QApplication>
#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
bool PopulateProcedureFromXmlFile(const QString &file_name,
                                  sequencergui::ProcedureItem *procedure_item)
{
  auto on_import = [file_name, procedure_item]()
  { ImportFromFile(file_name.toStdString(), procedure_item); };
  return sequencergui::InvokeAndCatch(on_import);
}

}  // namespace

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
void SequencerExplorerView::ShowXMLFile(const QString &file_name)
{
  // show content in XML editor
  m_xml_editor->SetXMLFile(file_name);

  // Generates temporary Procedure from XML and show object tree.
  m_temp_model = std::make_unique<SequencerModel>();
  auto procedure_item = m_temp_model->InsertItem<ProcedureItem>();
  if (PopulateProcedureFromXmlFile(file_name, procedure_item))
  {
    m_trees_widget->SetProcedure(procedure_item);
  }
  else
  {
    m_trees_widget->SetProcedure(nullptr);
  }
}

//! Show selected procedure in widgets.
//! - Generates XML representing a procedure and show it in editor.
//! - Show object tree in widgets.
void SequencerExplorerView::ShowSelectedProcedure(ProcedureItem *procedure_item)
{
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

void SequencerExplorerView::SetupConnections()
{
  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileClicked, this,
          &SequencerExplorerView::ShowXMLFile);

  auto import_procedure_from_file = [this](auto file_name)
  {
    auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    PopulateProcedureFromXmlFile(file_name, procedure_item);
  };
  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileDoubleClicked, this,
          import_procedure_from_file);

  connect(m_explorer_panel, &ExplorerPanel::ProcedureSelected, this,
          &SequencerExplorerView::ShowSelectedProcedure);

  // FIXME duplication with SequencerComposerView
  auto on_remove_procedure = [this](auto procedure)
  {
    if (procedure)
    {
      m_model->RemoveItem(procedure);
    }
  };

  connect(m_explorer_panel, &ExplorerPanel::RemoveProcedureRequest, this, on_remove_procedure);
}

}  // namespace sequencergui
