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
#include "sequencergui/explorer/xmleditor.h"
#include "sequencergui/mainwindow/proceduretreeswidget.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/xmlutils.h"

#include "mvvm/interfaces/modeleventsubscriberinterface.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
//! Wraps user method in try/catch and invoke it.
//! Provides busy-sign while executing, and warning dialog on exception catch.
template <typename T>
bool invoke_and_catch(T method)
{
  bool is_success{false};
  QApplication::setOverrideCursor(Qt::WaitCursor);
  try
  {
    std::invoke(method);
    QApplication::restoreOverrideCursor();
    is_success = true;
  }
  catch (const std::exception &ex)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox msgBox;

    QString message =
        QString("Exception was thrown while trying to load file \n\n%1").arg(ex.what());
    msgBox.setText(message);
    msgBox.setIcon(msgBox.Critical);
    msgBox.exec();
  }
  return is_success;
}
}  // namespace

namespace sequencergui
{
SequencerExplorerView::SequencerExplorerView(QWidget *parent)
    : QWidget(parent)
    , m_explorer_view(new ExplorerPanel)
    , m_trees_widget(new ProcedureTreesWidget)
    , m_xml_editor(new XMLEditor)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_splitter->addWidget(m_explorer_view);
  m_splitter->addWidget(m_trees_widget);
  m_splitter->addWidget(m_xml_editor);

  layout->addWidget(m_splitter);

  SetupConnections();
}

SequencerExplorerView::~SequencerExplorerView() = default;

void SequencerExplorerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_explorer_view->SetModel(model);

  auto on_data_change = [this](auto, auto)
  {
    if (auto procedure_item = m_explorer_view->GetCurrentScratchpadProcedure(); procedure_item)
    {
      m_xml_editor->SetXMLContent(QString::fromStdString(ExportToXMLString(procedure_item)));
    }
  };
  m_model->GetSubscriber()->SetOnDataChanged(on_data_change);
}

void SequencerExplorerView::SetXMLFile(const QString &file_name)
{
  m_xml_editor->SetXMLFile(file_name);

  m_trees_widget->SetModel(nullptr);
  m_temp_model = std::make_unique<SequencerModel>();

  auto procedure_item = m_temp_model->InsertItem<ProcedureItem>();

  auto on_import = [file_name, procedure_item]()
  { ImportFromFile(file_name.toStdString(), procedure_item); };

  if (auto result = invoke_and_catch(on_import); result)
  {
    m_trees_widget->SetModel(m_temp_model.get(), procedure_item);
  }
  //  on_import();
  //  m_trees_widget->SetModel(m_model.get(), procedure_item);
}

//! The procedure with given name with be added to procedure container.

void SequencerExplorerView::OnAddToScratchpad(const QString &file_name)
{
  auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
  qDebug() << "inserting new procedure" << procedure_item;

  auto on_import = [file_name, procedure_item]()
  { ImportFromFile(file_name.toStdString(), procedure_item); };
  invoke_and_catch(on_import);
}

void SequencerExplorerView::onCreateNewProcedure()
{
  auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
  qDebug() << "inserting new procedure" << procedure_item;
}

void SequencerExplorerView::onSratchpadProcedureSelected(ProcedureItem *procedure_item)
{
  qDebug() << "onSratchpadProcedureSelected" << procedure_item;
  if (procedure_item)
  {
    m_trees_widget->SetModel(m_model, procedure_item);
    m_xml_editor->SetXMLContent(QString::fromStdString(ExportToXMLString(procedure_item)));
  }
  else
  {
    m_xml_editor->ClearText();
    m_trees_widget->SetModel(nullptr, nullptr);
  }
}

void SequencerExplorerView::SetupConnections()
{
  connect(m_explorer_view, &ExplorerPanel::procedureFileClicked, this,
          &SequencerExplorerView::SetXMLFile);

  connect(m_explorer_view, &ExplorerPanel::procedureFileDoubleClicked, this,
          &SequencerExplorerView::OnAddToScratchpad);

  connect(m_explorer_view, &ExplorerPanel::createNewProcedureRequest, this,
          &SequencerExplorerView::onCreateNewProcedure);

  connect(m_explorer_view, &ExplorerPanel::sratchpadProcedureSelected, this,
          &SequencerExplorerView::onSratchpadProcedureSelected);
}

}  // namespace sequencergui
