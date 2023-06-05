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

#include "composer_procedure_editor.h"

#include <sequencergui/composer/instruction_editor_widget.h>
#include <sequencergui/composer/workspace_editor_widget.h>
#include <sequencergui/model/instruction_item.h>
#include <sup/gui/components/message_handler_interface.h>

#include <mvvm/widgets/collapsible_list_view.h>

#include <QVBoxLayout>

namespace sequencergui
{
ComposerProcedureEditor::ComposerProcedureEditor(
    std::unique_ptr<sup::gui::MessageHandlerInterface> message_handler, QWidget* parent)
    : QWidget(parent)
    , m_collapsible_list(new mvvm::CollapsibleListView)
    , m_instruction_editor_widget(new InstructionEditorWidget)
    , m_workspace_editor_widget(new WorkspaceEditorWidget)
    , m_message_handler(std::move(message_handler))
{
  setWindowTitle("Composer");

  auto layout = new QVBoxLayout(this);

  m_collapsible_list->AddCollapsibleWidget(m_instruction_editor_widget,
                                           m_instruction_editor_widget->actions());
  m_collapsible_list->AddCollapsibleWidget(m_workspace_editor_widget,
                                           m_workspace_editor_widget->actions());

  layout->addWidget(m_collapsible_list);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupConnections();
}

ComposerProcedureEditor::~ComposerProcedureEditor() = default;

void ComposerProcedureEditor::SetModel(SequencerModel* model)
{
  m_model = model;
}

void ComposerProcedureEditor::SetProcedure(ProcedureItem* procedure)
{
  m_procedure = procedure;
  m_instruction_editor_widget->SetProcedure(m_procedure);
  m_workspace_editor_widget->SetProcedure(m_procedure);
}

void ComposerProcedureEditor::SetSelectedInstructions(
    const std::vector<InstructionItem*>& instructions)
{
  m_instruction_editor_widget->SetSelectedInstructions(instructions);
}

std::vector<InstructionItem*> ComposerProcedureEditor::GetSelectedInstructions() const
{
  return m_instruction_editor_widget->GetSelectedInstructions();
}

InstructionItem* ComposerProcedureEditor::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void ComposerProcedureEditor::SetupConnections()
{
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InstructionSelected, this,
          &ComposerProcedureEditor::InstructionSelected);
}

}  // namespace sequencergui
