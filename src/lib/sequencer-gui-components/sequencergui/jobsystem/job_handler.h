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
#include <sequencergui/jobsystem/runner_status.h>
#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace mvvm
{
template <typename T>
class ItemListener;
}

namespace sequencergui
{

class ProcedureItem;
class JobLog;
class JobItem;
class InstructionItem;
class GUIObjectBuilder;
class LogEvent;
class WorkspaceItemListener;
class JobModel;
class BreakpointController;
class DomainRunnerService;
class DomainEventDispatcherContext;

/**
 * @brief The JobHandler class is the main class to run a job represented by the JobItem.
 *
 * It is used by the JobManager, where each JobItem is handled by its own JobHandler. JobHandler has
 * multiple responsibilities:
 *
 * - generate sequencer domain procedure using ProcedureItem
 * - setup domain procedure, and generate corresponding expanded ProcedureItem
 * - handle start/stop/pause/step requests
 * - listen for all sequencer domain events and update GUI items accordingly
 */
class JobHandler : public QObject
{
  Q_OBJECT

public:
  explicit JobHandler(JobItem* job_item, const UserContext& user_context = {});
  ~JobHandler() override;

  /**
   * @brief Run sequencer procedure in continuous manner, if allowed.
   */
  void OnStartRequest();

  /**
   * @brief Pause procedure's execution.
   */
  void OnPauseRequest();

  /**
   * @brief Execute a single step of the procedure, if allowed.
   */
  void OnMakeStepRequest();

  /**
   * @brief Stops the procedure execution.
   */
  void OnStopRequest();

  /**
   * @brief Checks if the job is running, which is one of pause/stepping/running states.
   */
  bool IsRunning() const;

  /**
   * @brief Returns expanded ProcedureItem.
   */
  ProcedureItem* GetExpandedProcedure() const;

  /**
   * @brief Returns runner status.
   */
  RunnerStatus GetRunnerStatus() const;

  /**
   * @brief Returns a job log.
   */
  JobLog* GetJobLog() const;

  /**
   * @brief Set/unset a breakpoint for given instruction.
   *
   * It is used to handle double-clicks in a tree of instructions belonging to ExpandedProcedure
   * item. Each double-click will toggle breakpoint status in a circle set/disabled/not-set.
   *
   * @param instruction Instruction item from expanded procedure tree.
   *
   */
  void OnToggleBreakpointRequest(sequencergui::InstructionItem* instruction);

signals:
  void InstructionStatusChanged(sequencergui::InstructionItem* instruction);
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

private:
  /**
   * @brief Processes instruction status change in the domain, and update InstructionItem's status
   * accordingly.
   */
  void OnInstructionStatusChanged(const InstructionStatusChangedEvent& event);

  /**
   * @brief Processes job status changed from the domain, and update JobItem status accordingly.
   */
  void OnJobStateChanged(const JobStateChangedEvent& event);

  /**
   * @brief Processes log events from the domain, and put them in GUI JobLog.
   */
  void onLogEvent(const sequencergui::LogEvent& event);

  /**
   * @brief Handles events reporting for changes in domain's next leaves.
   */
  void OnNextLeavesChangedEvent(const NextLeavesChangedEvent& event);

  /**
   * @brief Handles events reporting update in the domain variable.
   */
  void OnWorkspaceEvent(const WorkspaceEvent& event);

  /**
   * @brief Returns job model to which our JobItem
   */
  JobModel* GetJobModel();

  /**
   * @brief Setups breakpoint controller.
   */
  void SetupBreakpointController();

  /**
   * @brief Creates domain procedure from ProcedureItem.
   */
  void CreateDomainProcedure();

  /**
   * @brief Setup expanded procedure item.
   *
   * It will reflect the content of domain procedure after its Setup.
   */
  void SetupExpandedProcedureItem();

  /**
   * @brief Setups the domain runner.
   *
   * @param user_context The context to provide interaction with the user.
   * @param sleep_time_msec Sleep time at the end of each tick.
   */
  void SetupDomainRunner(const UserContext& user_context, int sleep_time_msec);

  /**
   * @brief Returns a context necessary for sequencer event queue to function.
   */
  DomainEventDispatcherContext CreateContext();

  //!< GUI object builder holding domain/GUI object correspondance
  std::unique_ptr<GUIObjectBuilder> m_guiobject_builder;

  //!< domain procedure should live longer than the runner
  std::unique_ptr<procedure_t> m_domain_procedure;

  //!< main runner to start/stop jobs
  std::unique_ptr<DomainRunnerService> m_domain_runner_service;

  //!< dedicated listener to provide communication bween domain/GUI workspace variables
  std::unique_ptr<WorkspaceItemListener> m_workspace_item_listener;

  //!< main controller to handle breakpoints toggling
  std::unique_ptr<BreakpointController> m_breakpoint_controller;

  //!< listens for JobItem property change
  std::unique_ptr<mvvm::ItemListener<JobItem>> m_property_listener;

  //!< the job log
  JobLog* m_job_log{nullptr};

  //!< the JobItem being handled
  JobItem* m_job_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_HANDLER_H_
