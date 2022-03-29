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

#include "sequencergui/composer/sequencercomposerview.h"

#include "sequencergui/composer/composerpanel.h"
#include "sequencergui/composer/composertreewidget.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/nodeeditor.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{
SequencerComposerView::SequencerComposerView(QWidget *parent)
    : QWidget(parent)
    , m_composer_panel(new ComposerPanel)
    , m_node_editor(new NodeEditor)
    , m_composer_tree_widget(new ComposerTreeWidget)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_node_editor);
  m_splitter->addWidget(m_composer_tree_widget);
  m_splitter->setSizes(QList<int>() << 50 << 300 << 120);

  layout->addWidget(m_splitter);

  SetupConnections();
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_panel->SetModel(model);
  m_node_editor->SetModel(model, GetFirstProcedure());
  m_composer_tree_widget->SetModel(model, GetFirstProcedure());
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
  connect(m_composer_tree_widget, &ComposerTreeWidget::InstructionSelected, this,
          on_tree_instruction_selected);

  auto on_procedure_selected = [this](auto procedure_item)
  {
    m_node_editor->SetModel(m_model, procedure_item);
    m_composer_tree_widget->SetModel(m_model, procedure_item);
  };
  connect(m_composer_panel, &ComposerPanel::procedureSelected, this, on_procedure_selected);

  auto on_create_procedure = [this]()
  { auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer()); };
  connect(m_composer_panel, &ComposerPanel::createNewProcedureRequest, this, on_create_procedure);
}

//! Returns first procedure from the procedure container, if exist.
ProcedureItem *SequencerComposerView::GetFirstProcedure()
{
  auto procedure_container = m_model->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequencergui
