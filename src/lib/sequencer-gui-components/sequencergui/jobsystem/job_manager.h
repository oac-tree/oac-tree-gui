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

#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace sequencergui
{

class JobModel;
class InstructionItem;
class JobItem;
class LocalJobHandler;

/**
 * @brief The JobManager class manages the execution of sequencer jobs.
 *
 * The job is represented by the JobItem, that carries all information about sequencer procedure.
 * The API contains interface to submit, start, pause and stop jobs.
 *
 * JobManager holds all jobs, submitted, paused, or running. Only one job at a time, set as the
 * current job, can be manipulated with start/pause/step/stop commands. Also, only current job
 * reports its status outside.
 */
class JobManager : public QObject
{
  Q_OBJECT

public:
  explicit JobManager(UserContext user_context, QObject* parent = nullptr);
  ~JobManager() override;

  /**
   * @brief Submits job for execution.
   *
   * Internally creates JobHandler which will takes care about execution logic. Do not start
   * execution yet.
   */
  void SubmitJob(JobItem* job);

  /**
   * @brief Returns job handler for a given job.
   */
  LocalJobHandler* GetJobHandler(JobItem* job);

  /**
   * @brief Start job.
   */
  void Start(JobItem* item);

  /**
   * @brief Pause given job.
   */
  void Pause(JobItem* item);

  /**
   * @brief Stop given job.
   */
  void Stop(JobItem* item);

  /**
   * @brief Execute a single step of the given job.
   */
  void Step(JobItem* item);

  /**
   * @brief Removes a job corresponding to a given JobItem from the manager.
   *
   * Internally removes JobHandler, do not affect JobItem.
   */
  void RemoveJobHandler(JobItem* job);

  /**
   * @brief Checks if there are jobs running.
   */
  bool HasRunningJobs() const;

  /**
   * @brief Stop all running jobs.
   */
  void StopAllJobs();

  /**
   * @brief Set active job.
   *
   * An active job is what is currently selected by the user. This job is allowed to send signals
   * and is used for toggling breakpoints requests.
   */
  void SetActiveJob(JobItem* item);

signals:
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

private:
  void OnNextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

  std::unique_ptr<LocalJobHandler> CreateJobHandler(JobItem* item);

  std::map<JobItem*, std::unique_ptr<LocalJobHandler>> m_job_map;
  UserContext m_user_context;
  JobItem* m_active_job{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_MANAGER_H_
