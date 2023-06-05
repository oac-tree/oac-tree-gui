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

#include <sequencergui/composer/instruction_editor_widget.h>
#include <sequencergui/composer/workspace_editor_widget.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace sequencergui
{
ComposerWidgetPanel::ComposerWidgetPanel(QWidget* parent)
    : QWidget(parent)
    , m_instruction_editor_widget(new InstructionEditorWidget)
    , m_workspace_editor_widget(new WorkspaceEditorWidget)
    , m_stack_widget(new ItemStackWidget)
{
  setWindowTitle("Composer");

  auto layout = new QVBoxLayout(this);

  m_stack_widget->AddWidget(m_instruction_editor_widget, m_instruction_editor_widget->actions());
  m_stack_widget->AddWidget(m_workspace_editor_widget, m_workspace_editor_widget->actions());

  layout->addWidget(m_stack_widget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupConnections();
}

ComposerWidgetPanel::~ComposerWidgetPanel() = default;

void ComposerWidgetPanel::SetModel(SequencerModel* model)
{
  m_model = model;
}

void ComposerWidgetPanel::SetProcedure(ProcedureItem* procedure)
{
  m_procedure = procedure;
  m_instruction_editor_widget->SetProcedure(m_procedure);
  m_workspace_editor_widget->SetProcedure(m_procedure);
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
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InstructionSelected, this,
          &ComposerWidgetPanel::InstructionSelected);
}

}  // namespace sequencergui
