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

#include "composer_widget_panel.h"

#include "xml_panel.h"

#include <sequencergui/composer/instruction_editor_widget.h>
#include <sequencergui/composer/workspace_editor_widget.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/node_editor.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace sequencergui
{
ComposerWidgetPanel::ComposerWidgetPanel(QWidget* parent)
    : QWidget(parent)
    , m_instruction_editor_widget(new InstructionEditorWidget)
    , m_workspace_editor_widget(new WorkspaceEditorWidget)
    , m_node_editor(new NodeEditor)
    , m_xml_panel(new XmlPanel)
    , m_stack_widget(new ItemStackWidget)
{
  setWindowTitle("Composer");

  auto layout = new QVBoxLayout(this);

  m_stack_widget->AddWidget(m_instruction_editor_widget, m_instruction_editor_widget->actions());
  m_stack_widget->AddWidget(m_workspace_editor_widget, m_workspace_editor_widget->actions());
  m_stack_widget->AddWidget(m_node_editor, m_node_editor->actions());
  m_stack_widget->AddWidget(m_xml_panel, m_xml_panel->actions());

  layout->addWidget(m_stack_widget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupConnections();
}

ComposerWidgetPanel::~ComposerWidgetPanel() = default;

void ComposerWidgetPanel::SetModel(SequencerModel* model)
{
  m_model = model;
  m_xml_panel->SetModel(model);
}

void ComposerWidgetPanel::SetProcedure(ProcedureItem* procedure)
{
  m_procedure = procedure;
  m_instruction_editor_widget->SetProcedure(m_procedure);
  m_workspace_editor_widget->SetProcedure(m_procedure);
  m_node_editor->SetProcedure(m_procedure);
  m_xml_panel->SetProcedure(m_procedure);
}

void ComposerWidgetPanel::SetSelectedInstructions(const std::vector<InstructionItem*>& instructions)
{
  m_instruction_editor_widget->SetSelectedInstructions(instructions);
}

std::vector<InstructionItem*> ComposerWidgetPanel::GetSelectedInstructions() const
{
  return m_instruction_editor_widget->GetSelectedInstructions();
}

InstructionItem* ComposerWidgetPanel::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void ComposerWidgetPanel::SetupConnections()
{
  auto on_scene_instruction_selected = [this](auto)
  {
    m_block_selection_to_scene = true;
    SetSelectedInstructions(m_node_editor->GetSelectedInstructions());
    m_block_selection_to_scene = false;
  };
  connect(m_node_editor, &NodeEditor::InstructionSelected, this, on_scene_instruction_selected);

  auto on_tree_instruction_selected = [this](auto)
  {
    if (!m_block_selection_to_scene)
    {
      m_node_editor->SetSelectedInstructions(GetSelectedInstructions());
    }
  };
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InstructionSelected, this,
          on_tree_instruction_selected);
}

}  // namespace sequencergui
