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

#include <sequencergui/operation/operation_action_context.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
class ISessionModel;
}

namespace sup::gui
{
class MessageEvent;
}  // namespace sup::gui

namespace sequencergui
{

class IJobItemManager;
class JobItem;
class ProcedureItem;
class InstructionItem;

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

  explicit OperationActionHandler(IJobItemManager* job_manager,
                                  OperationActionContext operation_context,
                                  QObject* parent = nullptr);

  ~OperationActionHandler() override;

  void SetJobContainer(mvvm::SessionItem* job_container);

  /**
   * @brief Submits given procedure for execution as local job.
   *
   * Local job doesn't own procedure. Procedure will remain after local job removal.
   */
  bool SubmitLocalJob(ProcedureItem* procedure_item);

  /**
   * @brief Submits given procedure for execution as imported job.
   *
   * Imported job owns the procedure. Procedure will be removed together with the job.
   */
  bool SubmitImportedJob(std::unique_ptr<ProcedureItem> procedure_item);

  /**
   * @brief Invokes dialogs to ask for remote connection info, and submits remote jobs.
   */
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
   * @brief Execute a single step of the currently selected job.
   */
  void OnMakeStepRequest();

  /**
   * @brief Make step for currently selected job.
   */
  void OnStopJobRequest();

  /**
   * @brief Reset domain runner.
   *
   * Works only for finished/halted jobs.
   */
  void OnResetJobRequest();

  /**
   * @brief Removes currently selected job.
   */
  bool OnRemoveJobRequest();

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
   * @brief Submit job for execution.
   *
   * Job will be inserted in the model, and then submitted to the job manager.
   */
  bool SubmitJob(std::unique_ptr<JobItem> job_item);

  /**
   * @brief Inserts job in the job model after current selection
   */
  JobItem* InsertJobAfterCurrentSelection(std::unique_ptr<JobItem> job_item);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {}) const;
  void SendMessage(const sup::gui::MessageEvent& message_event) const;

  /**
   * @brief Returns model used to store jobs.
   */
  mvvm::ISessionModel* GetModel() const;

  /**
   * @brief Returns container used for job storage.
   */
  mvvm::SessionItem* GetJobContainer() const;

  JobItem* GetSelectedJob();

  mvvm::SessionItem* m_job_container{nullptr};
  IJobItemManager* m_job_manager{nullptr};
  OperationActionContext m_operation_context;
  int m_tick_timeout{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_ACTION_HANDLER_H_
