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

#include <sequencergui/composer/instruction_editor_actions.h>
#include <sequencergui/composer/instruction_editor_widget.h>
#include <sequencergui/composer/workspace_editor_widget.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/pvmonitor/anyvalue_editor_dialog.h>
#include <sequencergui/pvmonitor/workspace_editor_actions.h>
#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/components/message_handler_interface.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/collapsible_list_view.h>
#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMessageBox>
#include <QSplitter>
#include <QToolBar>
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
    , m_instruction_editor_actions(
          std::make_unique<InstructionEditorActions>(CreateInstructionEditorContext()))
    , m_workspace_editor_actions(
          std::make_unique<WorkspaceEditorActions>(CreateWorkspaceEditorContext()))
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

  // propagate instruction related requests from InstructionTreeWidget to InstructionEditorActions
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InsertAfterRequest,
          m_instruction_editor_actions.get(),
          &InstructionEditorActions::OnInsertInstructionAfterRequest);
  connect(m_instruction_editor_widget, &InstructionEditorWidget::InsertIntoRequest,
          m_instruction_editor_actions.get(),
          &InstructionEditorActions::OnInsertInstructionIntoRequest);
  connect(m_instruction_editor_widget, &InstructionEditorWidget::RemoveSelectedRequest,
          m_instruction_editor_actions.get(),
          &InstructionEditorActions::OnRemoveInstructionRequest);

  // propagate instruction related requests from WorkspaceEditorWidget to WorkspaceEditorActions
  connect(m_workspace_editor_widget, &WorkspaceEditorWidget::InsertAfterRequest,
          m_workspace_editor_actions.get(), &WorkspaceEditorActions::OnAddVariableRequest);
  connect(m_workspace_editor_widget, &WorkspaceEditorWidget::RemoveSelectedRequest,
          m_workspace_editor_actions.get(), &WorkspaceEditorActions::OnRemoveVariableRequest);
  connect(m_workspace_editor_widget, &WorkspaceEditorWidget::EditAnyvalueRequest,
          m_workspace_editor_actions.get(), &WorkspaceEditorActions::OnEditAnyvalueRequest);
}

//! Create context necessary for InstructionEditorActions to function.
InstructionEditorContext ComposerProcedureEditor::CreateInstructionEditorContext()
{
  InstructionEditorContext result;
  result.selected_procedure = [this]() { return m_procedure; };
  result.selected_instruction = [this]()
  { return m_instruction_editor_widget->GetSelectedInstruction(); };

  auto send_message_callback = [this](const sup::gui::MessageEvent& event)
  { m_message_handler->SendMessage(event.text + event.informative + event.detailed); };
  result.send_message_callback = send_message_callback;

  return result;
}

WorkspaceEditorContext ComposerProcedureEditor::CreateWorkspaceEditorContext()
{
  WorkspaceEditorContext result;

  auto selected_workspace_callback = [this]()
  { return m_procedure ? m_procedure->GetWorkspace() : nullptr; };
  result.selected_workspace_callback = selected_workspace_callback;

  result.selected_item_callback = [this]() { return m_workspace_editor_widget->GetSelectedItem(); };

  auto send_message_callback = [this](const sup::gui::MessageEvent& event)
  {
    QMessageBox msg_box;
    msg_box.setText(QString::fromStdString(event.text));
    msg_box.setInformativeText(QString::fromStdString(event.informative));
    msg_box.setDetailedText(QString::fromStdString(event.detailed));
    msg_box.setIcon(msg_box.Warning);
    msg_box.exec();
  };
  result.send_message_callback = send_message_callback;

  auto edit_anyvalue_callback =
      [this](const sup::gui::AnyValueItem* item) -> std::unique_ptr<sup::gui::AnyValueItem>
  {
    AnyValueEditorDialog dialog(this);
    dialog.SetInitialValue(item);
    if (dialog.exec() == QDialog::Accepted)
    {
      return dialog.GetResult();
    }
    return {};
  };
  result.edit_anyvalue_callback = edit_anyvalue_callback;

  return result;
}

}  // namespace sequencergui
