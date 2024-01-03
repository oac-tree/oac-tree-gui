/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_explorer_view.h"

#include <sequencergui/explorer/explorer_panel.h>
#include <sequencergui/explorer/procedure_trees_widget.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/operation/procedure_action_handler.h>
#include <sup/gui/codeeditor/code_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/container_item.h>

#include <QSettings>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace
{

const QString kGroupName = "SequencerExplorerView";
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

}  // namespace

namespace sequencergui
{
SequencerExplorerView::SequencerExplorerView(QWidget *parent)
    : QWidget(parent)
    , m_explorer_panel(new ExplorerPanel)
    , m_trees_widget(new ProcedureTreesWidget)
    , m_xml_view(new sup::gui::CodeView)
    , m_right_panel(new sup::gui::ItemStackWidget)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_explorer_panel);
  m_splitter->addWidget(m_trees_widget);

  m_right_panel->AddWidget(m_xml_view);
  m_splitter->addWidget(m_right_panel);

  layout->addWidget(m_splitter);

  SetupConnections();

  ReadSettings();
}

SequencerExplorerView::~SequencerExplorerView()
{
  WriteSettings();
}

//! Sets the model.
void SequencerExplorerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_explorer_panel->SetModel(model);
}

void SequencerExplorerView::ImportProcedure(const QString &file_name)
{
  ProcedureActionHandler handler;

  if (auto procedure_item = handler.LoadProcedureFromFile(file_name); procedure_item)
  {
    m_model->InsertItem(std::move(procedure_item), m_model->GetProcedureContainer(),
                        mvvm::TagIndex::Append());
  }
}

//! Show content of XML file.
void SequencerExplorerView::ShowXMLFile(const QString &file_name)
{
  // show content in XML editor
  m_xml_view->SetFile(file_name);

  // Generates temporary Procedure from XML and show object tree.
  ProcedureActionHandler handler;
  auto procedure_item = handler.LoadProcedureFromFile(file_name);

  if (procedure_item)
  {
    m_temp_model = std::make_unique<SequencerModel>();
    auto procedure_ptr = procedure_item.get();
    auto procedure = m_temp_model->InsertItem(
        std::move(procedure_item), m_temp_model->GetRootItem(), mvvm::TagIndex::Append());
    m_trees_widget->SetProcedure(procedure_ptr);
  }
  else
  {
    m_trees_widget->SetProcedure(nullptr);
  }
}

void SequencerExplorerView::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }
}

void SequencerExplorerView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
}

void SequencerExplorerView::SetupConnections()
{
  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileClicked, this,
          &SequencerExplorerView::ShowXMLFile);

  connect(m_explorer_panel, &ExplorerPanel::ProcedureFileDoubleClicked, this,
          &SequencerExplorerView::ImportProcedure);

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
