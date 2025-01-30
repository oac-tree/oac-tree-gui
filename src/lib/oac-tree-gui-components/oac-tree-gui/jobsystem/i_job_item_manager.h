/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_I_JOB_ITEM_MANAGER_H_
#define SEQUENCERGUI_JOBSYSTEM_I_JOB_ITEM_MANAGER_H_

#include <vector>

namespace oac_tree_gui
{

class JobItem;
class IJobHandler;

/**
 * @brief The AbstractJobItemManager class is a base to manage the execution of sequencer jobs.
 *
 * The job is represented by the JobItem, that carries all information about sequencer procedure.
 * The API contains interface to submit, start, pause and stop jobs.
 *
 * JobManager holds all jobs, submitted, paused, or running. Only one job at a time, set as the
 * active job, can report its status up.
 */
class IJobItemManager
{
public:
  virtual ~IJobItemManager() = default;

  /**
   * @brief Returns number of submitted jobs.
   */
  virtual std::size_t GetJobCount() const = 0;

  /**
   * @brief Returns list of conrtolled job items.
   */
  virtual std::vector<JobItem*> GetJobItems() const = 0;

  /**
   * @brief Submits job for execution.
   *
   * Internally creates JobHandler which will takes care about execution logic. Do not start
   * execution yet.
   */
  virtual void SubmitJob(JobItem* job) = 0;

  /**
   * @brief Returns job handler for a given job.
   */
  virtual IJobHandler* GetJobHandler(JobItem* job) = 0;

  /**
   * @brief Start job.
   */
  virtual void Start(JobItem* item) = 0;

  /**
   * @brief Pause given job.
   */
  virtual void Pause(JobItem* item) = 0;

  /**
   * @brief Execute a single step of the given job.
   */
  virtual void Step(JobItem* item) = 0;

  /**
   * @brief Stop given job.
   */
  virtual void Stop(JobItem* item) = 0;

  /**
   * @brief Reset sequencer domain runner to the initial state.
   *
   * Works for finished/halted procedures.
   */
  virtual void Reset(JobItem* item) = 0;

  /**
   * @brief Removes a job corresponding to a given JobItem from the manager.
   *
   * Internally removes JobHandler, do not affect JobItem.
   */
  virtual void RemoveJobHandler(JobItem* job) = 0;

  /**
   * @brief Checks if there are jobs running.
   */
  virtual bool HasRunningJobs() const = 0;

  /**
   * @brief Stop all running jobs.
   */
  virtual void StopAllJobs() = 0;

  /**
   * @brief Set active job.
   *
   * An active job is what is currently selected by the user. This job is allowed to send signals
   * and is used for toggling breakpoints requests.
   */
  virtual void SetActiveJob(JobItem* item) = 0;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_I_JOB_ITEM_MANAGER_H_
