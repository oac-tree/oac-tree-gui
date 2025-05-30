/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_REALTIME_INSTRUCTION_TREE_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_REALTIME_INSTRUCTION_TREE_WIDGET_H_

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

namespace oac_tree_gui
{

class ProcedureItem;
class InstructionItem;
class BreakpointModelDelegate;
class InstructionTreeExpandController;

//! Widget with expanded instruction tree for realtime job execution.
//! Located at the central panel of SequencerMonitorView.

class RealTimeInstructionTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit RealTimeInstructionTreeWidget(QWidget* parent_widget = nullptr);
  ~RealTimeInstructionTreeWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstructions(std::vector<InstructionItem*> items);

  /**
   * @brief Makes tree viewport follow currently selected instruction.
   */
  void SetViewportFollowsSelectionFlag(bool value);

signals:
  void ToggleBreakpointRequest(oac_tree_gui::InstructionItem* instruction);

protected:
  bool event(QEvent* event) override;

private:
  void OnTreeDoubleClick(const QModelIndex& index);
  void OnCustomContextMenuRequested(const QPoint& pos);

  /**
   * @brief UpdateTreeViewport
   */
  void ScrollViewportToSelection();

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<BreakpointModelDelegate> m_delegate;
  std::unique_ptr<InstructionTreeExpandController> m_expand_controller;
  ProcedureItem* m_procedure{nullptr};

  bool m_viewport_follows_selection{true};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_REALTIME_INSTRUCTION_TREE_WIDGET_H_
