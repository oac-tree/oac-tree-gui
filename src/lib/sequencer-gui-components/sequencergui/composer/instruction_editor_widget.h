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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QSplitter;
class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
}

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;
class InstructionEditorContext;
class InstructionEditorActionHandler;
class InstructionEditorActions;
class VisibilityAgentBase;
class UniversalPropertyEditor;
class CustomTreeViewStyle;

//! InstructionEditor to compose instruction tree (add,remove, move instructions). Occupies the
//! center (and right side if necessary) os SequencerComposerView.

class InstructionEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionEditorWidget(QWidget* parent = nullptr);
  ~InstructionEditorWidget() override;

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupTree();
  void AdjustTreeAppearance();
  void SetProcedureIntern(ProcedureItem* procedure);

  void SetupConnections();
  InstructionEditorContext CreateInstructionEditorContext();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  UniversalPropertyEditor* m_property_tree{nullptr};

  QSplitter* m_splitter{nullptr};
  ProcedureItem* m_procedure{nullptr};

  InstructionEditorActions* m_editor_actions{nullptr};
  std::unique_ptr<InstructionEditorActionHandler> m_action_handler;

  VisibilityAgentBase* m_visibility_agent{nullptr};
  CustomTreeViewStyle* m_tree_view_style{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_
