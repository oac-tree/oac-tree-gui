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

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H
#define SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H

#include "sequencergui/model/sequencer_types.h"

#include <QWidget>

class QSplitter;
class QTreeView;
class QTabWidget;

namespace mvvm
{
class TopItemsTreeView;
class PropertyTreeView;
class AllItemsTreeView;
}  // namespace mvvm

namespace sequencergui
{
class SequencerModel;
class InstructionItem;
class ComposerTreeToolBar;
class ProcedureItem;

//! The panel with two trees occupying right part of SequencerComposerView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! instruction.

class ComposerTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerTreeWidget(QWidget* parent = nullptr);
  virtual ~ComposerTreeWidget() override;

  void SetModel(SequencerModel* model, ProcedureItem* procedure);

  void SetSelected(InstructionItem* instruction);
  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

signals:
  void InstructionSelected(InstructionItem* instruction);

private:
  void SetupConnections();

  ComposerTreeToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  mvvm::TopItemsTreeView* m_instruction_tree{nullptr};
  mvvm::AllItemsTreeView* m_workspace_tree{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};

  SequencerModel* m_model{nullptr};
  ProcedureItem* m_procedure{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H
