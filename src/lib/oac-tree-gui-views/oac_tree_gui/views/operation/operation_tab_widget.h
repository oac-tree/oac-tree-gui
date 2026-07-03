/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_

#include <QWidget>
#include <sup/gui/mainwindow/session_item_widget.h>

#include <vector>

class QTabWidget;
class QToolBar;

namespace oac_tree_gui
{

class InstructionItem;
class JobItem;
class JobLog;
class MonitorRealTimeActions;
class OperationRealTimeWidget;

/**
 * @brief The OperationTabWidget class shows running jobs with the possibility to select what to see
 * in a tab.
 *
 * Contains following views:
 * - main panel with real time tree
 * - workspace with variable tree
 * - workspace with variable table
 * - task monitor
 * - node editor
 */
class OperationTabWidget : public sup::gui::SessionItemWidget
{
  Q_OBJECT

public:
  explicit OperationTabWidget(QWidget* parent_widget = nullptr);

  void SetItem(mvvm::SessionItem* job_item) override;
  void SetCurrentJob(JobItem* job_item);

  void SetSelectedInstructions(const std::vector<const InstructionItem*>& items);

  void SetJobLog(JobLog* job_log);

signals:
  void RunRequest();
  void PauseRequest();
  void StepRequest();
  void StopRequest();
  void ResetRequest();
  void ChangeDelayRequest(int msec);
  void ToggleBreakpointRequest(const oac_tree_gui::InstructionItem* instruction);

private:
  void SetupConnections();
  void AddTab(QWidget* widget, const QString& label, const QString& tooltip);

  MonitorRealTimeActions* m_actions{nullptr};
  QToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  OperationRealTimeWidget* m_realtime_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_
