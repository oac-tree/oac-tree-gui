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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_OBJECTS_JOB_MANAGER_H_
#define OAC_TREE_GUI_JOBSYSTEM_OBJECTS_JOB_MANAGER_H_

#include <oac_tree_gui/jobsystem/i_job_item_manager.h>

#include <QObject>
#include <memory>

namespace oac_tree_gui
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
class JobManager : public QObject, public IJobItemManager
{
  Q_OBJECT

public:
  using create_handler_func_t = std::function<std::unique_ptr<IJobHandler>(JobItem&)>;

  explicit JobManager(create_handler_func_t create_handler_func, QObject* parent_object = nullptr);
  ~JobManager() override;

  JobManager(const JobManager&) = delete;
  JobManager& operator=(const JobManager&) = delete;
  JobManager(JobManager&&) = delete;
  JobManager& operator=(JobManager&&) = delete;

  std::size_t GetJobCount() const override;

  std::vector<JobItem*> GetJobItems() const override;

  void SubmitJob(JobItem* job) override;

  IJobHandler* GetJobHandler(JobItem* job) override;

  void Start(JobItem* item) override;

  void Pause(JobItem* item) override;

  void Step(JobItem* item) override;

  void Stop(JobItem* item) override;

  void Reset(JobItem* item) override;

  void RemoveJobHandler(JobItem* job) override;

  bool HasRunningJobs() const override;

  void StopAllJobs() override;

  void SetActiveJob(JobItem* item) override;

signals:
  void ActiveInstructionChanged(const std::vector<oac_tree_gui::InstructionItem*>&);

private:
  /**
   * @brief Inserts job handler into the job map.
   */
  void InsertJobHandler(std::unique_ptr<IJobHandler> job_handler);

  /**
   * @brief Process "Active instructions" events from all job handlers, forwards active job
   * notifications up.
   */
  void OnActiveInstructionChanged(const std::vector<oac_tree_gui::InstructionItem*>&);

  std::vector<std::unique_ptr<IJobHandler>> m_job_handlers;
  JobItem* m_active_job{nullptr};  //!< job which is allowed to send signals up
  create_handler_func_t m_create_handler_func;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_OBJECTS_JOB_MANAGER_H_
