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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_HANDLER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/jobsystem/job_types.h>
#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace sequencergui
{

class ProcedureItem;
class JobLog;
class JobItem;
class InstructionItem;
class GUIObjectBuilder;
class LogEvent;
class WorkspaceSynchronizer;
class JobModel;
class BreakpointController;
class DomainRunnerService;
class DomainEventDispatcherContext;

//! Contains all information necessary for job to run: runner, log, domain builder.
class JobHandler : public QObject
{
  Q_OBJECT

public:
  explicit JobHandler(JobItem* job_item, const UserContext& user_context = {},
                      int sleep_time_msec = 0);
  ~JobHandler() override;

  void onStartRequest();

  void onPauseRequest();

  void onMakeStepRequest();

  void onStopRequest();

  bool IsRunning() const;

  void SetSleepTime(int time_msec);

  ProcedureItem* GetExpandedProcedure() const;

  RunnerStatus GetRunnerStatus() const;

  JobLog* GetJobLog() const;

  void OnToggleBreakpointRequest(sequencergui::InstructionItem* instruction);

signals:
  void InstructionStatusChanged(sequencergui::InstructionItem* instruction);
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

private:
  void OnInstructionStatusChanged(const InstructionStatusChangedEvent& event);
  void OnJobStateChanged(const JobStateChangedEvent& event);
  void onLogEvent(const sequencergui::LogEvent& event);
  void OnNextLeavesChangedEvent(const NextLeavesChangedEvent& event);

  JobModel* GetJobModel();

  void PrepareForRun();
  void SetupDomainProcedure();
  void SetupExpandedProcedureItem();
  void SetupDomainRunner(const UserContext& user_context, int sleep_time_msec);
  DomainEventDispatcherContext CreateContext();

  std::unique_ptr<GUIObjectBuilder> m_guiobject_builder;

  //!< domain procedure should live longer than the runner
  std::unique_ptr<procedure_t> m_domain_procedure;

  std::unique_ptr<DomainRunnerService> m_domain_runner_service;

  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;

  std::unique_ptr<BreakpointController> m_breakpoint_controller;

  JobLog* m_job_log{nullptr};

  JobItem* m_job_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_HANDLER_H_
