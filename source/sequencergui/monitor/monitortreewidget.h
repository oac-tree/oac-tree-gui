/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H
#define SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H

#include <QWidget>

class QSplitter;
class QTreeView;
class QStackedWidget;

namespace ModelView
{
class ItemsTreeView;
}

namespace sequi
{
class SequencerModel;
class MonitorTreeToolBar;
class InstructionItem;
class ProcedureItem;
class InstructionViewModel;
class MessagePanel;
class NodeEditor;

//! Central panel on SequencerMonitorView.

class MonitorTreeWidget : public QWidget
{
  Q_OBJECT

public:
  MonitorTreeWidget(QWidget* parent = nullptr);
  ~MonitorTreeWidget();

  void SetModel(SequencerModel* model, ProcedureItem* procedure_item);

  void SetSelected(const InstructionItem* item);

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

  MonitorTreeToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};

  QStackedWidget* m_stacked_widget{nullptr};
  ModelView::ItemsTreeView* m_instruction_tree{nullptr};
  NodeEditor* m_node_editor{nullptr};

  MessagePanel* m_message_panel{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H
