/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/sequencer_composer_view.h"

#include <sequencergui/composer/composer_panel.h>
#include <sequencergui/composer/composer_procedure_editor.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/xml_utils.h>
#include <sequencergui/nodeeditor/node_editor.h>
#include <sequencergui/utils/style_utils.h>
#include <sequencergui/widgets/item_stack_widget.h>
#include <sequencergui/widgets/xml_editor.h>

#include <mvvm/project/model_has_changed_controller.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/gui/components/message_handler_interface.h>

#include <QDebug>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{
SequencerComposerView::SequencerComposerView(QWidget *parent)
    : QWidget(parent)
    , m_composer_panel(new ComposerPanel)
    , m_node_editor(new NodeEditor)
    , m_central_panel(new ItemStackWidget)
    , m_composer_procedure_editor(
          new ComposerProcedureEditor(m_node_editor->CreateMessageHandler()))
    , m_xml_editor(new XMLEditor)
    , m_right_panel(new ItemStackWidget)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_central_panel->AddWidget(m_node_editor, m_node_editor->CreateToolBar());
  m_right_panel->AddWidget(m_composer_procedure_editor, m_composer_procedure_editor->actions());
  m_right_panel->AddWidget(m_xml_editor);

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_central_panel);
  m_splitter->addWidget(m_right_panel);
  m_splitter->setSizes(QList<int>() << styleutils::UnitSize(30) << styleutils::UnitSize(90)
                                    << styleutils::UnitSize(30));

  layout->addWidget(m_splitter);

  SetupConnections();
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_panel->SetModel(model);
  m_composer_procedure_editor->SetModel(model);
  m_model_changed_controller =
      std::make_unique<mvvm::ModelHasChangedController>(m_model, [this]() { UpdateXML(); });
}

void SequencerComposerView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);
  if (!m_composer_panel->GetSelectedProcedure())
  {
    m_composer_panel->SetSelectedProcedure(GetFirstProcedure());
  }
}

void SequencerComposerView::UpdateXML()
{
  if (auto selected = m_composer_panel->GetSelectedProcedure(); selected)
  {
    try
    {
      m_xml_editor->SetXMLContent(QString::fromStdString(ExportToXMLString(selected)));
    }
    catch (const std::exception &ex)
    {
      // Procedure is in inconsistent state. For example, variable items all have the same names
      // which makes domain's Workspace unhappy.
      m_xml_editor->ClearText();
    }
  }
}

SequencerComposerView::~SequencerComposerView()
{
  //  m_model_changed_controller.reset(); // to avoid signaling when
}

void SequencerComposerView::SetupConnections()
{
  auto on_scene_instruction_selected = [this](auto)
  {
    m_block_selection_to_scene = true;
    m_composer_procedure_editor->SetSelectedInstructions(m_node_editor->GetSelectedInstructions());
    m_block_selection_to_scene = false;
  };
  connect(m_node_editor, &NodeEditor::InstructionSelected, this, on_scene_instruction_selected);

  auto on_tree_instruction_selected = [this](auto)
  {
    if (!m_block_selection_to_scene)
    {
      m_node_editor->SetSelectedInstructions(
          m_composer_procedure_editor->GetSelectedInstructions());
    }
  };
  connect(m_composer_procedure_editor, &ComposerProcedureEditor::InstructionSelected, this,
          on_tree_instruction_selected);

  auto on_procedure_selected = [this](auto procedure_item)
  {
    qDebug() << "Show on_procedure_selected" << procedure_item;
    m_node_editor->SetProcedure(procedure_item);
    m_composer_procedure_editor->SetProcedure(procedure_item);
    UpdateXML();
  };
  connect(m_composer_panel, &ComposerPanel::ProcedureSelected, this, on_procedure_selected);

  auto on_create_procedure = [this]()
  { auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer()); };
  connect(m_composer_panel, &ComposerPanel::CreateNewProcedureRequest, this, on_create_procedure);

  auto on_remove_procedure = [this](auto procedure)
  {
    if (procedure)
    {
      m_model->RemoveItem(procedure);
    }
  };
  connect(m_composer_panel, &ComposerPanel::RemoveProcedureRequest, this, on_remove_procedure);
}

//! Returns first procedure from the procedure container, if exist.
ProcedureItem *SequencerComposerView::GetFirstProcedure()
{
  auto procedure_container = m_model->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequencergui
