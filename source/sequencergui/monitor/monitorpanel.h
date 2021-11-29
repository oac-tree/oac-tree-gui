/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_MONITORPANEL_H
#define SEQUENCERGUI_MONITOR_MONITORPANEL_H

#include <QWidget>

class QSplitter;

namespace sequi
{
class ExplorerToolBar;
class OpenDocumentsWidget;
class ProcedureItem;
class SequencerModel;

//! Left panel on SequencerMonitorView.
//! Contains widget with open procedures and running jobs.

class MonitorPanel : public QWidget
{
  Q_OBJECT

public:
  MonitorPanel(QWidget* parent = nullptr);
  ~MonitorPanel();

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure() const;

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void procedureSelected(ProcedureItem* procedureItem);

private:
  ExplorerToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};
  OpenDocumentsWidget* m_open_documents_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_MONITORPANEL_H
