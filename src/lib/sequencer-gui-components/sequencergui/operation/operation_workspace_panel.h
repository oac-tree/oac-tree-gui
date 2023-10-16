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

/**
 * @brief The OperationWorkspacePanel class is a right panel on OperationMonitorView representing
 * workspace variables.
 *
 * @details Contains a panel with possibility to switch between full tree, and table-like view.
 */

class OperationWorkspacePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationWorkspacePanel(QWidget* parent = nullptr);
  ~OperationWorkspacePanel() override;

  void SetProcedure(ProcedureItem* procedure_item);

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_WORKSPACE_PANEL_H_
