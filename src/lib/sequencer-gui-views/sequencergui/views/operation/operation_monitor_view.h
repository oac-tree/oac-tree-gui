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

#ifndef SEQUENCERGUI_VIEWS_OPERATION_OPERATION_MONITOR_VIEW_H_
#define SEQUENCERGUI_VIEWS_OPERATION_OPERATION_MONITOR_VIEW_H_

#include <QWidget>
#include <sequencergui/components/component_types.h>

#include <memory>

class QSplitter;
class QShowEvent;

namespace sequencergui
{

class ApplicationModels;
class JobItem;
class JobManager;
class OperationWorkspacePanel;
class OperationRealTimePanel;
class OperationJobPanel;
class ProcedureItem;
class OperationActionHandler;
class OperationActionContext;
class RemoteConnectionService;

/**
 * @brief The OperationMonitorView class is the main window component to run sequences and monitor
 * execution. Contains the panel with jobs on the left, main sequence tree in the middle, and the
 * panel to monitor variables on the right.
 */
class OperationMonitorView : public QWidget
{
  Q_OBJECT

public:
  explicit OperationMonitorView(OperationPresentationMode mode, QWidget* parent = nullptr);
  ~OperationMonitorView() override;

  void SetModels(ApplicationModels* models);

  bool OnImportJobRequest(const QString& file_name = {});

  /**
   * @brief Returns true if there are jobs running.
   */
  bool HasRunningJobs() const;

  /**
   * @brief Stop all running jobs.
   */
  void StopAllJobs();

protected:
  void showEvent(QShowEvent* event) override;
  void closeEvent(QCloseEvent* event) override;

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void SetupWidgetActions();
  void OnJobSelected(sequencergui::JobItem* item);
  OperationActionContext CreateOperationContext();

  OperationPresentationMode m_presentation_mode;

  QWidget* CreateLeftPanel();
  QWidget* CreateCentralPanel();

  QAction* m_show_left_sidebar{nullptr};
  QAction* m_show_right_sidebar{nullptr};

  OperationJobPanel* m_job_panel{nullptr};
  OperationRealTimePanel* m_realtime_panel{nullptr};
  QWidget* m_left_panel{nullptr};
  OperationWorkspacePanel* m_workspace_panel{nullptr};

  QSplitter* m_splitter{nullptr};
  ApplicationModels* m_models{nullptr};

  JobManager* m_job_manager{nullptr};
  OperationActionHandler* m_action_handler{nullptr};
  std::unique_ptr<RemoteConnectionService> m_connection_service;

  bool m_left_panel_is_visible{true};
  bool m_right_panel_is_visible{true};

};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_OPERATION_OPERATION_MONITOR_VIEW_H_
