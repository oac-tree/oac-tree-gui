/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

namespace sup::gui
{
class ItemStackWidget;
}

namespace sequencergui
{

class OperationWorkspaceWidget;
class ProcedureItem;

/**
 * @brief The OperationWorkspacePanel class represents vertical panel on the right part of
 * OperationMonitorView with Workspace variables.
 *
 * @details It is a stack of two widgets with possibility to choose either a workspace tree or
 * workspace table.
 */

class OperationWorkspacePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationWorkspacePanel(QWidget* parent = nullptr);
  ~OperationWorkspacePanel() override;

  void SetProcedure(ProcedureItem* item);

private:
  void ReadSettings();
  void WriteSettings();

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  OperationWorkspaceWidget* m_workspace_tree_widget{nullptr};
  OperationWorkspaceWidget* m_workspace_table_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_WORKSPACE_PANEL_H_
