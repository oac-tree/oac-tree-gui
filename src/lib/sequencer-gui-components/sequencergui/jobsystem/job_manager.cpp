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

#include <sup/dto/anyvalue.h>

#include <algorithm>

namespace sequencergui
{

JobManager::JobManager(UserContext user_context, QObject *parent)
    : QObject(parent), m_user_context(user_context)
{
}

JobManager::~JobManager() = default;

void JobManager::SubmitJob(JobItem *job)
{
  if (!job)
  {
    throw RuntimeException("Attempt to submit undefined job");
  }

  SubmitJob(std::make_unique<LocalJobHandler>(job, m_user_context));
}

void JobManager::SubmitJob(std::unique_ptr<AbstractJobHandler> job_handler)
{
  auto job_item = job_handler->GetJobItem();

  if (auto handler = GetJobHandler(job_item); handler)
  {
    throw RuntimeException("Attempt to submit already existing job");
  }

  connect(job_handler.get(), &AbstractJobHandler::NextLeavesChanged, this,
          &JobManager::OnNextLeavesChanged);

  m_job_map.insert({job_item, std::move(job_handler)});
}

AbstractJobHandler *JobManager::GetJobHandler(JobItem *job)
{
  auto iter = m_job_map.find(job);
  return iter == m_job_map.end() ? nullptr : iter->second.get();
}

void JobManager::Start(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    ResetJobIfNecessary(item);
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
    ResetJobIfNecessary(item);
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

void JobManager::ResetJobIfNecessary(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    auto status = job_handler->GetRunnerStatus();
    if (status == RunnerStatus::kFailed || status == RunnerStatus::kSucceeded
        || status == RunnerStatus::kHalted)
    {
      job_handler->Reset();
    }
  }
}

}  // namespace sequencergui
