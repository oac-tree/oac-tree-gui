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
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/views/editors/user_input_dialogs.h>

#include <sup/dto/anyvalue.h>

#include <QWidget>
#include <algorithm>

namespace sequencergui
{

JobManager::JobManager(QObject *parent) : QObject(parent) {}

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

JobManager::~JobManager() = default;

//! Set JobManager to display the status of given job.

void JobManager::SetCurrentJob(JobItem *job)
{
  if (m_current_job == job)
  {
    return;
  }

  m_current_job = job;

  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    if (m_set_joblog_cb)
    {
      m_set_joblog_cb(job_handler->GetJobLog());
    }
  }
}

JobHandler *JobManager::GetCurrentJobHandler()
{
  return GetJobHandler(m_current_job);
}

//! Returns job handler for JobItem.

JobHandler *JobManager::GetJobHandler(JobItem *job)
{
  auto iter = m_job_map.find(job);
  return iter == m_job_map.end() ? nullptr : iter->second.get();
}

//! Returns current job. It is the one that is attached to the MessagePanel and is the recipient
//! of start/stop/pause requests.

JobItem *JobManager::GetCurrentJob()
{
  return m_current_job;
}

void JobManager::OnStartJobRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->OnStartRequest();
  }
}

void JobManager::OnPauseJobRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->OnPauseRequest();
  }
}

void JobManager::OnStopJobRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->OnStopRequest();
  }
}

void JobManager::OnMakeStepRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->OnMakeStepRequest();
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

    if (job == GetCurrentJob())
    {
      SetCurrentJob(nullptr);
    }

    m_job_map.erase(job);
  }
}

void JobManager::SetMessagePanel(set_joblog_cb cb)
{
  m_set_joblog_cb = cb;
}

bool JobManager::HasRunningJobs() const
{
  return std::any_of(m_job_map.begin(), m_job_map.end(),
                     [](const auto &iter) { return iter.second->IsRunning(); });
}

void JobManager::StopAllJobs()
{
  std::for_each(m_job_map.begin(), m_job_map.end(),
                [](const auto &iter) { iter.second->OnStopRequest(); });
}

void JobManager::OnNextLeavesChanged(const std::vector<InstructionItem *> &leaves)
{
  auto sending_job_handler = qobject_cast<JobHandler *>(sender());

  // we do not want to send selection request for invisible job
  if (sending_job_handler == GetCurrentJobHandler())
  {
    emit NextLeavesChanged(leaves);
  }
}

std::unique_ptr<JobHandler> JobManager::CreateJobHandler(JobItem *item)
{
  auto parent_widget = dynamic_cast<QWidget *>(parent());
  auto on_user_input = [parent_widget](const auto &args)
  { return GetAnyValueEditorDialogResult(args, parent_widget); };

  auto on_user_choice = [parent_widget](const auto &args)
  { return GetUserChoiceDialogResult(args, parent_widget); };

  UserContext user_context{on_user_input, on_user_choice};

  auto job_handler = std::make_unique<JobHandler>(item, user_context);
  connect(job_handler.get(), &JobHandler::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);
  connect(job_handler.get(), &JobHandler::NextLeavesChanged, this,
          &JobManager::OnNextLeavesChanged);

  return job_handler;
}

}  // namespace sequencergui
