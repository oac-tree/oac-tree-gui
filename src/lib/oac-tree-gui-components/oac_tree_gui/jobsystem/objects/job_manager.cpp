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

#include "job_manager.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/model/instruction_item.h>

#include <mvvm/utils/container_utils.h>

#include <sup/dto/anyvalue.h>

#include <algorithm>

namespace oac_tree_gui
{

/**
 * @brief Checks if domain reset is required.
 */
bool IsResetRequired(RunnerStatus runner_status)
{
  static const std::vector<RunnerStatus> kStatesRequiringReset = {
      RunnerStatus::kSucceeded, RunnerStatus::kFailed, RunnerStatus::kHalted};

  return mvvm::utils::Contains(kStatesRequiringReset, runner_status);
}

JobManager::JobManager(create_handler_func_t create_handler_func, QObject *parent_object)
    : QObject(parent_object), m_create_handler_func(std::move(create_handler_func))
{
  if (!m_create_handler_func)
  {
    throw RuntimeException("Uninitialized factory functions to create handlers");
  }
}

std::size_t JobManager::GetJobCount() const
{
  return m_job_handlers.size();
}

std::vector<JobItem *> JobManager::GetJobItems() const
{
  std::vector<JobItem *> result;
  auto on_element = [](const auto &handler) { return handler->GetJobItem(); };
  std::transform(m_job_handlers.begin(), m_job_handlers.end(), std::back_inserter(result),
                 on_element);
  return result;
}

JobManager::~JobManager() = default;

void JobManager::SubmitJob(JobItem *job)
{
  if (!job)
  {
    throw RuntimeException("Attempt to submit undefined job");
  }

  InsertJobHandler(m_create_handler_func(*job));
}

IJobHandler *JobManager::GetJobHandler(JobItem *job)
{
  auto on_element = [job](const auto &handler) { return handler->GetJobItem() == job; };
  auto pos = std::find_if(m_job_handlers.begin(), m_job_handlers.end(), on_element);
  return pos == m_job_handlers.end() ? nullptr : pos->get();
}

void JobManager::Start(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    if (IsResetRequired(job_handler->GetRunnerStatus()))
    {
      Reset(item);
    }
    job_handler->Start();
  }
}

void JobManager::Pause(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Pause();
  }
}

void JobManager::Stop(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Stop();
  }
}

void JobManager::Step(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    if (IsResetRequired(job_handler->GetRunnerStatus()))
    {
      Reset(item);
    }
    job_handler->Step();
  }
}

void JobManager::RemoveJobHandler(JobItem *job)
{
  if (auto job_handler = GetJobHandler(job); job_handler)
  {
    if (job_handler->IsRunning())
    {
      throw RuntimeException("Attempt to modify running job");
    }

    auto on_element = [job](auto &handler) { return handler->GetJobItem() == job; };
    m_job_handlers.erase(std::remove_if(m_job_handlers.begin(), m_job_handlers.end(), on_element),
                         m_job_handlers.end());
  }
}

bool JobManager::HasRunningJobs() const
{
  return std::any_of(m_job_handlers.begin(), m_job_handlers.end(),
                     [](const auto &handler) { return handler->IsRunning(); });
}

void JobManager::StopAllJobs()
{
  std::for_each(m_job_handlers.begin(), m_job_handlers.end(),
                [](const auto &handler) { handler->Stop(); });
}

void JobManager::SetActiveJob(JobItem *item)
{
  m_active_job = item;
}

void JobManager::OnActiveInstructionChanged(const std::vector<InstructionItem *> &active_instructions)
{
  auto sending_job_handler = qobject_cast<AbstractJobHandler *>(sender());

  // we want to send notifications only from the job the user is currently looking at
  if (sending_job_handler->GetJobItem() == m_active_job)
  {
    emit ActiveInstructionChanged(active_instructions);
  }
}

void JobManager::Reset(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Reset();
  }
}

void JobManager::InsertJobHandler(std::unique_ptr<IJobHandler> job_handler)
{
  auto job_item = job_handler->GetJobItem();

  if (auto handler = GetJobHandler(job_item); handler)
  {
    throw RuntimeException("Attempt to submit already existing job");
  }

  if (auto abstract_handler = dynamic_cast<AbstractJobHandler *>(job_handler.get()))
  {
    connect(abstract_handler, &AbstractJobHandler::ActiveInstructionChanged, this,
            &JobManager::OnActiveInstructionChanged);
  }

  m_job_handlers.push_back(std::move(job_handler));
}

}  // namespace oac_tree_gui
