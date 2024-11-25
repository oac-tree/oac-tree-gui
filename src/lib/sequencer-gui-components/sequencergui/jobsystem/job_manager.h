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

#include <sequencergui/jobsystem/abstract_job_item_manager.h>

namespace sequencergui
{

class JobModel;
class InstructionItem;

/**
 * @brief The JobManager class manages the execution of sequencer jobs.
 *
 * The job is represented by the JobItem, that carries all information about sequencer procedure.
 * The API contains interface to submit, start, pause and stop jobs.
 *
 * JobManager holds all jobs, submitted, paused, or running. Only one job at a time, set as the
 * active job, can report its status up.
 */
class JobManager : public AbstractJobItemManager
{
  Q_OBJECT

public:
  using create_handler_func_t = std::function<std::unique_ptr<AbstractJobHandler>(JobItem&)>;

  explicit JobManager(create_handler_func_t create_handler_func, QObject* parent = nullptr);
  ~JobManager() override;

  void SubmitJob(JobItem* job) override;

  AbstractJobHandler* GetJobHandler(JobItem* job) override;

  void Start(JobItem* item) override;

  void Pause(JobItem* item) override;

  void Stop(JobItem* item) override;

  void Step(JobItem* item) override;

  void RemoveJobHandler(JobItem* job) override;

  bool HasRunningJobs() const override;

  void StopAllJobs() override;

  void SetActiveJob(JobItem* item) override;

signals:
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

private:
  /**
   * @brief Inserts job handler into the job map.
   */
  void InsertJobHandler(std::unique_ptr<AbstractJobHandler> job_handler);

  /**
   * @brief Process "Next Leaves" events from all job handlers, forwards active job notifications
   * up.
   */
  void OnNextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

  /**
   * @brief Resets domain async runner of the given job to initial state, if necessary.
   *
   * This concerns jobs that are not running but were running before (i.e. jobs that are in one
   * of kFailed, kSucceeded, or kHalted states).
   *
   * Effectively, it leads to instruction/procedure states reset to the initial state. The rest
   * (domain procedure, JobHandler, collapse/expand status of the tree) should stay as before.
   */
  void ResetJobIfNecessary(JobItem* item);

  std::map<JobItem*, std::unique_ptr<AbstractJobHandler>> m_job_map;
  JobItem* m_active_job{nullptr};  //!< job which is allowed to send signals up
  create_handler_func_t m_create_handler_func;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_MANAGER_H_
