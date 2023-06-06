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

#include <QList>
#include <QMenu>
#include <QWidget>
#include <memory>

class QAction;
class QWidgetAction;
class QSplitter;

namespace mvvm
{
class TopItemsTreeView;
class PropertyTreeView;
}  // namespace mvvm

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;
class InstructionEditorContext;
class InstructionEditorController;

//! Tree of instruction items at the center of SequencerComposerView.
//! Defines set of actions to add/remove instructions.

class InstructionEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionEditorWidget(QWidget* parent = nullptr);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);
  void InsertIntoRequest(const QString& name);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();

private:
  void SetupActions();
  InstructionEditorContext CreateInstructionEditorContext();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();
  std::unique_ptr<QMenu> CreateInsertIntoMenu();

  std::unique_ptr<QMenu> m_insert_into_menu;
  std::unique_ptr<QMenu> m_insert_after_menu;

  QWidgetAction* m_insert_after_action{nullptr};
  QWidgetAction* m_insert_into_action{nullptr};
  QWidgetAction* m_remove_action{nullptr};

  mvvm::TopItemsTreeView* m_tree_view{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};
  ProcedureItem* m_procedure{nullptr};
  std::unique_ptr<InstructionEditorController> m_instruction_editor_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_WIDGET_H_
