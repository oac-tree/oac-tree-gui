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

#include "sequencergui/composer/sequencer_composer_view.h"

#include <sequencergui/composer/composer_panel.h>
#include <sequencergui/composer/composer_procedure_editor.h>
#include <sequencergui/core/message_handler_interface.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/node_editor.h>
#include <sequencergui/utils/style_utils.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/container_item.h>

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>
#include <QToolBar>

namespace sequencergui
{
SequencerComposerView::SequencerComposerView(QWidget *parent)
    : QWidget(parent)
    , m_composer_panel(new ComposerPanel)
    , m_node_editor(new NodeEditor)
    , m_stack_widget(new ItemStackWidget)
    , m_composer_tree_widget(new ComposerProcedureEditor(m_node_editor->CreateMessageHandler()))
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_stack_widget->AddWidget(m_node_editor, m_node_editor->CreateToolBar());

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_stack_widget);
  m_splitter->addWidget(m_composer_tree_widget);
  m_splitter->setSizes(QList<int>() << styleutils::UnitSize(30) << styleutils::UnitSize(90)
                                    << styleutils::UnitSize(30));

  layout->addWidget(m_splitter);

  SetupConnections();
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_panel->SetModel(model);
  m_composer_tree_widget->SetModel(model);
}

void SequencerComposerView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);
  if (!m_composer_panel->GetSelectedProcedure())
  {
    m_composer_panel->SetSelectedProcedure(GetFirstProcedure());
  }
}

SequencerComposerView::~SequencerComposerView() = default;

void SequencerComposerView::SetupConnections()
{
  auto on_scene_instruction_selected = [this](auto)
  {
    m_block_selection_to_scene = true;
    m_composer_tree_widget->SetSelectedInstructions(m_node_editor->GetSelectedInstructions());
    m_block_selection_to_scene = false;
  };
  connect(m_node_editor, &NodeEditor::InstructionSelected, this, on_scene_instruction_selected);

  auto on_tree_instruction_selected = [this](auto)
  {
    if (!m_block_selection_to_scene)
    {
      m_node_editor->SetSelectedInstructions(m_composer_tree_widget->GetSelectedInstructions());
    }
  };
  connect(m_composer_tree_widget, &ComposerProcedureEditor::InstructionSelected, this,
          on_tree_instruction_selected);

  auto on_procedure_selected = [this](auto procedure_item)
  {
    qDebug() << "Show on_procedure_selected" << procedure_item;
    m_node_editor->SetProcedure(procedure_item);
    m_composer_tree_widget->SetProcedure(procedure_item);
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
