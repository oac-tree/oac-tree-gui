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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_REALTIME_PANEL_H_
#define SEQUENCERGUI_OPERATION_OPERATION_REALTIME_PANEL_H_

#include <QWidget>

class QToolBar;

namespace mvvm
{
class CollapsibleListView;
}

namespace sequencergui
{
class SequencerModel;
class MonitorRealTimeToolBar;
class InstructionItem;
class ProcedureItem;
class MessagePanel;
class RealTimeInstructionTreeWidget;

//! Central panel on SequencerMonitorView.

class OperationRealTimePanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationRealTimePanel(QWidget* parent = nullptr);
  ~OperationRealTimePanel() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstruction(InstructionItem* item);

  void SetSelectedInstructions(std::vector<InstructionItem*> items);

  MessagePanel* GetMessagePanel();

  QToolBar* GetToolBar() const;

  int GetCurrentTickTimeout();

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);
  void ToggleBreakpointRequest(sequencergui::InstructionItem* instruction);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();

  MonitorRealTimeToolBar* m_tool_bar{nullptr};

  mvvm::CollapsibleListView* m_collapsible_list{nullptr};
  RealTimeInstructionTreeWidget* m_realtime_instruction_tree{nullptr};
  MessagePanel* m_message_panel{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_REALTIME_PANEL_H_
