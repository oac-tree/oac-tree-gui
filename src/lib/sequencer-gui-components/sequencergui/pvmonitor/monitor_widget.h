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

#ifndef SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_
#define SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <QWidget>
#include <memory>

namespace mvvm
{
class AllItemsTreeView;
class ViewModel;
}  // namespace mvvm

namespace sequencergui
{

class MonitorModel;
class WorkspaceSynchronizer;
class MonitorWidgetToolBar;
class WorkspaceEditorActionHandler;
class WorkspaceEditorContext;
class WorkspaceItem;

class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWidget(MonitorModel* model, QWidget* parent = nullptr);
  ~MonitorWidget() override;

  void SetWorkspaceItem(WorkspaceItem* item);

private:
  mvvm::ViewModel* GetViewModel();

  void SetupConnections();
  void OnStartMonitoringRequest();
  void OnStopMonitoringRequest();

  WorkspaceEditorContext CreateContext();

  MonitorWidgetToolBar* m_tool_bar{nullptr};

  MonitorModel* m_model{nullptr};
  std::unique_ptr<workspace_t> m_workspace;
  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;
  WorkspaceEditorActionHandler* m_workspace_editor_action_handler{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_
