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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_WORKSPACE_PANEL_H_
#define SEQUENCERGUI_OPERATION_OPERATION_WORKSPACE_PANEL_H_

#include <QWidget>

class QToolBar;
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
class SequencerModel;
class ProcedureItem;
class InstructionItem;

//! Right panel on SequencerMonitorView.

class OperationWorkspacePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationWorkspacePanel(QWidget* parent = nullptr);
  ~OperationWorkspacePanel() override;

  void SetProcedure(ProcedureItem* procedure_item);

  QToolBar* GetToolBar() const;

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  QToolBar* m_tool_bar{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_WORKSPACE_PANEL_H_
