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

#ifndef SEQUENCERGUI_OPERATION_REALTTIME_INSTRUCTION_TREE_WIDGET_H_
#define SEQUENCERGUI_OPERATION_REALTTIME_INSTRUCTION_TREE_WIDGET_H_

#include <QWidget>
#include <memory>

class QTreeView;
class QAction;

namespace mvvm
{
class ItemViewComponentProvider;
}

namespace sup::gui
{
class CustomHeaderView;
}

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;
class BreakpointModelDelegate;
class InstructionTreeSelectionController;

//! Widget with expanded instruction tree for realtime job execution.
//! Located at the central panel of SequencerMonitorView.

class RealTimeInstructionTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit RealTimeInstructionTreeWidget(QWidget* parent = nullptr);
  ~RealTimeInstructionTreeWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstruction(InstructionItem* item);

  void SetSelectedInstructions(std::vector<InstructionItem*> items);

  /**
   * @brief Makes tree viewport follow currently selected instruction.
   */
  void SetViewportFollowsSelectionFlag(bool value);

signals:
  void ToggleBreakpointRequest(sequencergui::InstructionItem* instruction);

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();
  void OnTreeDoubleClick(const QModelIndex& index);
  void OnCustomContextMenuRequested(const QPoint &pos);

  /**
   * @brief UpdateTreeViewport
   */
  void ScrollViewportToSelection();

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<BreakpointModelDelegate> m_delegate;
  std::unique_ptr<InstructionTreeSelectionController> m_selection_controller;
  ProcedureItem* m_procedure{nullptr};

  bool m_viewport_follows_selection{true};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_REALTTIME_INSTRUCTION_TREE_WIDGET_H_
