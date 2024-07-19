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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_MANAGER_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_MANAGER_H_

#include <QObject>
#include <functional>
#include <memory>

namespace sequencergui
{
struct UserChoiceResult;
struct UserChoiceArgs;
struct UserInputResult;
struct UserInputArgs;
class JobModel;
class InstructionItem;
class JobItem;
class JobHandler;
class JobLog;

//! Manages JobItem execution. Maps JobItem to the JobHandler, containing all machinery to execute
//! domain procedures in a thread.

class JobManager : public QObject
{
  Q_OBJECT

public:
  using set_joblog_cb = std::function<void(JobLog*)>;

  explicit JobManager(QObject* parent = nullptr);
  ~JobManager() override;

  void SubmitJob(JobItem* job);

  void SetCurrentJob(JobItem* job);

  JobHandler* GetCurrentJobHandler();

  JobHandler* GetJobHandler(JobItem* job);

  JobItem* GetCurrentJob();

  void OnStartJobRequest();
  void OnPauseJobRequest();
  void OnStopJobRequest();
  void OnMakeStepRequest();

  /**
   * @brief Removes a job corresponding to a given JobItem from the manager.
   *
   * @details Internally removes JobHandler, do not affect JobItem.
   */
  void OnRemoveJobRequest(JobItem* job);

  void SetMessagePanel(set_joblog_cb cb);

  /**
   * @brief Returns true if there are jobs running.
   */
  bool HasRunningJobs() const;

  /**
   * @brief Stop all running jobs.
   */
  void StopAllJobs();

signals:
  void InstructionStatusChanged(sequencergui::InstructionItem* instruction);
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

private:
  void OnNextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

  std::unique_ptr<JobHandler> CreateJobHandler(JobItem* item);

  JobItem* m_current_job{nullptr};
  std::map<JobItem*, std::unique_ptr<JobHandler>> m_job_map;
  set_joblog_cb m_set_joblog_cb;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_MANAGER_H_
