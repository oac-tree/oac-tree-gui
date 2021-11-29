/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H
#define SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H

#include <QWidget>

class QSplitter;
class QTreeView;

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

  ModelView::ItemsTreeView* m_instruction_tree{nullptr};
  MessagePanel* m_message_panel{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_MONITORTREEWIDGET_H
