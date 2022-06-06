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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERPROCEDUREEDITOR_H
#define SEQUENCERGUI_COMPOSER_COMPOSERPROCEDUREEDITOR_H

#include "sequencergui/model/sequencer_types.h"

#include <QWidget>

class QSplitter;
class QTabWidget;

namespace mvvm
{
class PropertyTreeView;
}  // namespace mvvm

namespace sequencergui
{
class SequencerModel;
class InstructionItem;
class DotsToolBar;
class ProcedureItem;
class ComposerActions;
class InstructionTreeWidget;
class WorkspaceListWidget;
class ComposerContext;
class MessageHandlerInterface;

//! The panel with two trees occupying right part of SequencerComposerView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! instruction.

class ComposerProcedureEditor : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerProcedureEditor(std::unique_ptr<MessageHandlerInterface> message_handler,
                                   QWidget* parent = nullptr);
  ~ComposerProcedureEditor() override;

  void SetModel(SequencerModel* model);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstruction(InstructionItem* instruction);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);

private:
  void SetupConnections();
  ComposerContext CreateComposerContext();

  DotsToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  InstructionTreeWidget* m_instruction_tree{nullptr};
  WorkspaceListWidget* m_workspace_tree{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};

  SequencerModel* m_model{nullptr};
  ProcedureItem* m_procedure{nullptr};

  std::unique_ptr<ComposerActions> m_composer_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERPROCEDUREEDITOR_H
