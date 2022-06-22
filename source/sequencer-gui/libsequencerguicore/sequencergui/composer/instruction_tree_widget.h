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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_TREE_WIDGET_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_TREE_WIDGET_H_

#include <QList>
#include <QMenu>
#include <QWidget>
#include <memory>

class QAction;

namespace mvvm
{
class TopItemsTreeView;
}  // namespace mvvm

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;

//! Tree of instruction items at the right of SequencerComposerView.
//! Defines set of actions to add/remove instructions. Part of ComposerProcedureEditor.

class InstructionTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionTreeWidget(QWidget* parent = nullptr);

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstruction(InstructionItem* instruction);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  InstructionItem* GetSelectedInstruction() const;

  QList<QWidget*> GetToolBarWidgets();

signals:
  void InstructionSelected(sequencergui::InstructionItem* instruction);
  void InsertIntoRequest(const QString& name);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();

private:
  void SetupActions();
  std::unique_ptr<QMenu> CreateInsertAfterMenu();
  std::unique_ptr<QMenu> CreateInsertIntoMenu();

  std::unique_ptr<QMenu> m_insert_into_menu;
  std::unique_ptr<QMenu> m_insert_after_menu;

  mvvm::TopItemsTreeView* m_tree_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_TREE_WIDGET_H_
