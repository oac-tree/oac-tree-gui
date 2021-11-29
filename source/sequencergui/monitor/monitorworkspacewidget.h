/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_MONITORWORKSPACEWIDGET_H
#define SEQUENCERGUI_MONITOR_MONITORWORKSPACEWIDGET_H

#include <QWidget>

namespace ModelView
{
class AllItemsTreeView;
class PropertyTreeView;
}  // namespace ModelView

class QSplitter;

namespace sequi
{
class SequencerModel;
class MonitorWorkspaceToolBar;
class ProcedureItem;

//! Right panel on SequencerMonitorView.

class MonitorWorkspaceWidget : public QWidget
{
  Q_OBJECT

public:
  MonitorWorkspaceWidget(QWidget* parent = nullptr);
  ~MonitorWorkspaceWidget();

  void SetModel(SequencerModel* model, ProcedureItem *procedure_item);

private:
  MonitorWorkspaceToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};

  ModelView::AllItemsTreeView* m_instruction_tree{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_MONITORWORKSPACEWIDGET_H
