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

#ifndef SEQUENCERGUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_
#define SEQUENCERGUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_

#include <QWidget>

class QToolBar;

namespace sup::gui
{
class CollapsibleListView;
}

namespace sequencergui
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
  explicit OperationRealTimePanel(QWidget* parent = nullptr);
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
  void ToggleBreakpointRequest(sequencergui::InstructionItem* instruction);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();

  MonitorRealTimeActions* m_actions{nullptr};

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  RealTimeInstructionTreeWidget* m_realtime_instruction_tree{nullptr};
  MessagePanel* m_message_panel{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_OPERATION_OPERATION_REALTIME_PANEL_H_
