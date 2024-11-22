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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_ACTION_HANDLER_H_
#define SEQUENCERGUI_OPERATION_OPERATION_ACTION_HANDLER_H_

#include <sequencergui/jobsystem/remote_connection_context.h>
#include <sequencergui/jobsystem/remote_connection_service.h>
#include <sequencergui/jobsystem/user_context.h>
#include <sequencergui/operation/operation_action_context.h>

#include <QObject>
#include <functional>

namespace sup::gui
{
class MessageEvent;
}  // namespace sup::gui

namespace sequencergui
{

class JobModel;
class JobManager;
class JobItem;
class ProcedureItem;
class InstructionItem;
class RemoteConnectionService;
/**
 * @brief The OperationActionHandler class handles high-level actions of OperationMonitorView
 * related to job submission and job removal.
 *
 * It works with the JobManager and adds reporting on exception throws, and some additional logic
 * related to job resubmission.
 */
class OperationActionHandler : public QObject
{
  Q_OBJECT

public:
  //!< A callback to get currently selected JobItem.
  using selection_callback_t = std::function<JobItem*()>;

  explicit OperationActionHandler(JobManager* job_manager, OperationActionContext operation_context,
                                  UserContext user_context, QObject* parent = nullptr);

  ~OperationActionHandler() override;

  void SetJobModel(JobModel* job_model);

  /**
   * @brief Submits given procedure for execution.
   */
  bool OnSubmitJobRequest(ProcedureItem* procedure_item);

  bool OnImportRemoteJobRequest();

  /**
   * @brief Start currently selected job.
   */
  void OnStartJobRequest();

  /**
   * @brief Pause currently selected job.
   */
  void OnPauseJobRequest();

  /**
   * @brief Make step for currently selected job.
   */
  void OnStopJobRequest();

  /**
   * @brief Execute a single step of the currently selected job.
   */
  void OnMakeStepRequest();

  /**
   * @brief Removes currently selected job. By providing an additional flag, it is possible to
   * remove the procedure on which the given job is looking. It is relevant only for
   * sequencer-operation GUI, where the removal of JobItem from the list of job should lead to the
   * removal of the corresponding imported procedure.
   *
   * @param cleanup Provide additional cleanup and remove the corresponding procedure.
   */
  bool OnRemoveJobRequest(bool cleanup = false);

  /**
   * @brief Fully regenerates submitted job.
   *
   * This will remove job handler and its domain underlying components (domain procedure, async
   * runner), and will resubmit the same job again. As a result, the domain procedure will be
   * freshly generated from its GUI source.
   */
  bool OnRegenerateJobRequest();

  /**
   * @brief Sets tick timeout of currently selected job to given value.
   */
  void OnSetTickTimeoutRequest(int msec);

  /**
   * @brief Set/unset a breakpoint for given instruction for currently active job.
   */
  void OnToggleBreakpoint(InstructionItem* instruction);

signals:
  void MakeJobSelectedRequest(sequencergui::JobItem* item);

private:
  /**
   * @brief Inserts job in the job model after current selection
   */
  JobItem* InsertJobAfterCurrentSelection(std::unique_ptr<JobItem> job_item);

  void CheckConditions();

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {}) const;
  void SendMessage(const sup::gui::MessageEvent& message_event) const;

  JobItem* GetSelectedJob();

  JobModel* m_job_model{nullptr};
  JobManager* m_job_manager{nullptr};
  OperationActionContext m_operation_context;
  std::unique_ptr<RemoteConnectionService> m_connection_service;
  int m_default_delay{0};
  UserContext m_user_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_ACTION_HANDLER_H_
