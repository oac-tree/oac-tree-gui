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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSER_PROCEDURE_EDITOR_H_
#define SEQUENCERGUI_COMPOSER_COMPOSER_PROCEDURE_EDITOR_H_

#include <QWidget>

class QTabWidget;
class QToolBar;
class QWidgetAction;

namespace sup::gui
{
class MessageHandlerInterface;
}

namespace sequencergui
{

class InstructionEditorActions;
class InstructionEditorContext;
class InstructionEditorWidget;
class InstructionItem;
class ProcedureItem;
class SequencerModel;
class WorkspaceEditorActions;
class WorkspaceEditorContext;
class WorkspaceEditorWidget;

//! The panel with two trees occupying right part of SequencerComposerView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! instruction.

class ComposerProcedureEditor : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerProcedureEditor(
      std::unique_ptr<sup::gui::MessageHandlerInterface> message_handler,
      QWidget* parent = nullptr);
  ~ComposerProcedureEditor() override;

  void SetModel(SequencerModel* model);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);

private:
  void SetupToolBar();
  void SetupConnections();
  InstructionEditorContext CreateInstructionEditorContext();
  WorkspaceEditorContext CreateWorkspaceEditorContext();

  QToolBar* m_tool_bar{nullptr};
  QWidgetAction* m_tool_bar_action{nullptr};  //!< wrapper to send our tool bar outside

  QTabWidget* m_tab_widget{nullptr};
  InstructionEditorWidget* m_instruction_editor_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_editor_widget{nullptr};

  SequencerModel* m_model{nullptr};
  ProcedureItem* m_procedure{nullptr};

  std::unique_ptr<sup::gui::MessageHandlerInterface> m_message_handler;
  std::unique_ptr<InstructionEditorActions> m_instruction_editor_actions;
  std::unique_ptr<WorkspaceEditorActions> m_workspace_editor_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSER_PROCEDURE_EDITOR_H_
