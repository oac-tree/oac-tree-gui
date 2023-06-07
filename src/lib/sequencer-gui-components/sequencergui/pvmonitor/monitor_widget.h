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

#ifndef SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_
#define SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QAbstractItemModel;

namespace sup::sequencer
{
class Workspace;
}  // namespace sup::sequencer

namespace mvvm
{
class AllItemsTreeView;
class SessionItem;
template <typename T>
class ModelListener;
class SessionModelInterface;
struct ItemInsertedEvent;
class ViewModel;
}  // namespace mvvm

namespace sequencergui
{
class VariableItem;
class MonitorModel;
class WorkspaceSynchronizer;
class MonitorWidgetToolBar;
class WorkspaceEditorActionHandler;
class WorkspaceEditorContext;

class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  using listener_t = mvvm::ModelListener<mvvm::SessionModelInterface>;

  explicit MonitorWidget(MonitorModel* model, QWidget* parent = nullptr);
  ~MonitorWidget() override;

private:
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event);
  mvvm::ViewModel* GetViewModel();

  void PopulateModel();
  void SetupConnections();
  void OnStartMonitoringRequest();
  void OnStopMonitoringRequest();

  WorkspaceEditorContext CreateContext();

  MonitorWidgetToolBar* m_tool_bar{nullptr};

  MonitorModel* m_model{nullptr};
  std::unique_ptr<sup::sequencer::Workspace> m_workspace;
  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;
  WorkspaceEditorActionHandler* m_actions{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  std::unique_ptr<listener_t> m_listener;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_H_
