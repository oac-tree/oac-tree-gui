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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_

#include <QWidget>

class QToolBar;

namespace sup::gui
{
class CollapsibleListView;
}

namespace oac_tree_gui
{

class InstructionItem;
class JobItem;
class JobLog;
class MessagePanel;
class MonitorRealTimeActions;
class RealTimeInstructionTreeWidget;
class SequencerModel;

/**
 * @brief The OperationRealTimePanel class is a central panel of the OperationMonitorView.
 *
 * Contains a real-time instruction tree with start/step/stop control elements on the top, and log
 * panel at the bottom.
 */
class OperationRealTimePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationRealTimePanel(QWidget* parent_widget = nullptr);
  ~OperationRealTimePanel() override;

  void SetCurrentJob(JobItem* job_item);

  void SetSelectedInstructions(std::vector<InstructionItem*> items);

  void SetJobLog(JobLog* job_log);

  int GetCurrentTickTimeout();

signals:
  void RunRequest();
  void PauseRequest();
  void StepRequest();
  void StopRequest();
  void ResetRequest();
  void ChangeDelayRequest(int msec);
  void ToggleBreakpointRequest(oac_tree_gui::InstructionItem* instruction);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();

  MonitorRealTimeActions* m_actions{nullptr};

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  RealTimeInstructionTreeWidget* m_realtime_instruction_tree{nullptr};
  MessagePanel* m_message_panel{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_
