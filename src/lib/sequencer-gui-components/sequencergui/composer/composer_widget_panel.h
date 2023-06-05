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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSER_WIDGET_PANEL_H_
#define SEQUENCERGUI_COMPOSER_COMPOSER_WIDGET_PANEL_H_

#include <QWidget>

namespace sequencergui
{

class InstructionEditorWidget;
class InstructionItem;
class ProcedureItem;
class SequencerModel;
class WorkspaceEditorWidget;
class ItemStackWidget;

//! The panel with stack of widgets for procedure editing: instruction tree editor, workspace
//! editor, node editor and code view.

class ComposerWidgetPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerWidgetPanel(QWidget* parent = nullptr);
  ~ComposerWidgetPanel() override;

  void SetModel(SequencerModel* model);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);

private:
  void SetupConnections();

  InstructionEditorWidget* m_instruction_editor_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_editor_widget{nullptr};
  ItemStackWidget* m_stack_widget{nullptr};

  ProcedureItem* m_procedure{nullptr};
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSER_WIDGET_PANEL_H_
