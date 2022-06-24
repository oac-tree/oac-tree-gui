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

#ifndef SEQUENCERGUI_MONITOR_MONITOR_WORKSPACE_WIDGET_H_
#define SEQUENCERGUI_MONITOR_MONITOR_WORKSPACE_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class AllItemsTreeView;
class PropertyTreeView;
}  // namespace mvvm

class QSplitter;

namespace sequencergui
{
class SequencerModel;
class MonitorWorkspaceToolBar;
class ProcedureItem;

//! Right panel on SequencerMonitorView.

class MonitorWorkspaceWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWorkspaceWidget(QWidget* parent = nullptr);
  ~MonitorWorkspaceWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

private:
  MonitorWorkspaceToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};

  mvvm::AllItemsTreeView* m_workspace_tree{nullptr};
  mvvm::PropertyTreeView* m_property_tree{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITOR_WORKSPACE_WIDGET_H_
