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

  if (auto job_handler = GetJobHandler(job); job_handler)
  {
    throw RuntimeException("Attempt to submit already existing job");
  }

  auto job_handler = CreateJobHandler(job);
  m_job_map.insert({job, std::move(job_handler)});
}

LocalJobHandler *JobManager::GetJobHandler(JobItem *job)
{
  auto iter = m_job_map.find(job);
  return iter == m_job_map.end() ? nullptr : iter->second.get();
}

void JobManager::OnStartJobRequest(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Start();
  }
}

void JobManager::OnPauseJobRequest(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Pause();
  }
}

void JobManager::OnStopJobRequest(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Stop();
  }
}

void JobManager::OnMakeStepRequest(JobItem *item)
{
  if (auto job_handler = GetJobHandler(item); job_handler)
  {
    job_handler->Step();
  }
}

void JobManager::OnRemoveJobRequest(JobItem *job)
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

void JobManager::OnNextLeavesChanged(const std::vector<InstructionItem *> &leaves)
{
  auto sending_job_handler = qobject_cast<LocalJobHandler *>(sender());

  // FIXME restore
  // // we do not want to send selection request for invisible job
  // if (sending_job_handler == GetCurrentJobHandler())
  // {
  //   emit NextLeavesChanged(leaves);
  // }
}

std::unique_ptr<LocalJobHandler> JobManager::CreateJobHandler(JobItem *item)
{
  auto job_handler = std::make_unique<LocalJobHandler>(item, m_user_context);
  connect(job_handler.get(), &LocalJobHandler::NextLeavesChanged, this,
          &JobManager::OnNextLeavesChanged);

  return job_handler;
}

}  // namespace sequencergui
