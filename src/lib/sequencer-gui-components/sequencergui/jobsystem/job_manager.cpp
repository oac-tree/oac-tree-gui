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

#include "job_manager.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/local_job_handler.h>
#include <sequencergui/model/instruction_item.h>

#include <mvvm/utils/container_utils.h>

#include <sup/dto/anyvalue.h>

#include <algorithm>

namespace sequencergui
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

JobManager::JobManager(create_handler_func_t create_handler_func, QObject *parent)
    : QObject(parent), m_create_handler_func(std::move(create_handler_func))
{
  if (!m_create_handler_func)
  {
    throw RuntimeException("Uninitialized factory functions to create handlers");
  }
}

size_t JobManager::GetJobCount() const
{
  return m_job_map.size();
}

std::vector<JobItem *> JobManager::GetJobItems() const
{
  std::vector<JobItem *> result;
  auto on_element = [](const auto &element) { return element.first; };
  std::transform(m_job_map.begin(), m_job_map.end(), std::back_inserter(result), on_element);
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
  auto iter = m_job_map.find(job);
  return iter == m_job_map.end() ? nullptr : iter->second.get();
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

    m_job_map.erase(job);
  }
}

bool JobManager::HasRunningJobs() const
{
  return std::any_of(m_job_map.begin(), m_job_map.end(),
                     [](const auto &iter) { return iter.second->IsRunning(); });
}

void JobManager::StopAllJobs()
{
  std::for_each(m_job_map.begin(), m_job_map.end(), [](const auto &iter) { iter.second->Stop(); });
}

void JobManager::SetActiveJob(JobItem *item)
{
  m_active_job = item;
}

void JobManager::OnNextLeavesChanged(const std::vector<InstructionItem *> &leaves)
{
  auto sending_job_handler = qobject_cast<AbstractJobHandler *>(sender());

  // we want to send notifications only from the job the user is currently looking at
  if (sending_job_handler->GetJobItem() == m_active_job)
  {
    emit NextLeavesChanged(leaves);
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
    connect(abstract_handler, &AbstractJobHandler::NextLeavesChanged, this,
            &JobManager::OnNextLeavesChanged);
  }

  m_job_map.insert({job_item, std::move(job_handler)});
}

}  // namespace sequencergui
