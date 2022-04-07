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

#ifndef SEQUENCERGUI_MONITOR_MONITORREALTIMEWIDGET_H
#define SEQUENCERGUI_MONITOR_MONITORREALTIMEWIDGET_H

#include <QWidget>

class QSplitter;
class QTreeView;
class QStackedWidget;

namespace mvvm
{
class ItemsTreeView;
}

namespace sequencergui
{
class SequencerModel;
class MonitorRealTimeToolBar;
class InstructionItem;
class ProcedureItem;
class MessagePanel;
class NodeEditor;
class CollapsibleWidget;

//! Central panel on SequencerMonitorView.

class MonitorRealTimeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorRealTimeWidget(QWidget* parent = nullptr);
  ~MonitorRealTimeWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstruction(const InstructionItem* item);

  MessagePanel* GetMessagePanel();

  void onAppChangeRequest(int id);

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);

private:
  void SetupConnections();

  MonitorRealTimeToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};

  QStackedWidget* m_stacked_widget{nullptr};
  mvvm::ItemsTreeView* m_instruction_tree{nullptr};
  NodeEditor* m_node_editor{nullptr};

  MessagePanel* m_message_panel{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITORREALTIMEWIDGET_H
