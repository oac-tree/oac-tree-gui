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
