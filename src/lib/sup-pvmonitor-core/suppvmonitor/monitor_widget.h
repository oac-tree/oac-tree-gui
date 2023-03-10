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

#ifndef LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_H_
#define LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_H_

#include <QWidget>
#include <memory>

namespace sup::sequencer
{
class Workspace;
}  // namespace sup::sequencer

namespace mvvm
{
class AllItemsTreeView;
}

namespace sequencergui
{
class VariableItem;
}

namespace suppvmonitor
{

class MonitorModel;
class WorkspaceSynchronizer;
class MonitorWidgetToolBar;
class MonitorWidgetActions;
class MonitorWidgetContext;

class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWidget(QWidget* parent = nullptr);
  ~MonitorWidget() override;

  sequencergui::VariableItem* GetSelectedVariable();

private:
  void PopulateModel();
  void SetupConnections();
  void OnStartMonitoringRequest();

  MonitorWidgetContext CreateContext();

  MonitorWidgetToolBar* m_tool_bar{nullptr};

  std::unique_ptr<MonitorModel> m_model;
  std::unique_ptr<sup::sequencer::Workspace> m_workspace;
  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;
  MonitorWidgetActions* m_actions{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
};

}  // namespace suppvmonitor

#endif  // LIBSUP_PVMONITOR_CORE_SUPPVMONITOR_MONITOR_WIDGET_H_
