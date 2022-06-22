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

#ifndef SEQUENCERGUI_MONITOR_MONITOR_REALTIME_WIDGET_H_
#define SEQUENCERGUI_MONITOR_MONITOR_REALTIME_WIDGET_H_

#include <QWidget>
#include <array>
#include <memory>

#include <QHeaderView>

namespace mvvm
{
class ItemViewComponentProvider;
}

class QTreeView;
class QStackedWidget;

namespace sequencergui
{
class SequencerModel;
class MonitorRealTimeToolBar;
class InstructionItem;
class ProcedureItem;
class MessagePanel;
class NodeEditor;
class CollapsibleListView;
class CustomHeaderView;

//! Central panel on SequencerMonitorView.

class MonitorRealTimeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorRealTimeWidget(QWidget* parent = nullptr);
  ~MonitorRealTimeWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstruction(InstructionItem* item);

  MessagePanel* GetMessagePanel();

  void onAppChangeRequest(int id);

signals:
  void runRequest();
  void pauseRequest();
  void stepRequest();
  void stopRequest();
  void changeDelayRequest(int msec);

private:
  void AdjustColumnWidth();

  void SetupConnections();

  MonitorRealTimeToolBar* m_tool_bar{nullptr};
  CollapsibleListView* m_splitter{nullptr};

  QStackedWidget* m_stacked_widget{nullptr};
  QTreeView* m_instruction_tree{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;

  NodeEditor* m_node_editor{nullptr};

  MessagePanel* m_message_panel{nullptr};
  CustomHeaderView* m_custom_header{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITOR_REALTIME_WIDGET_H_
