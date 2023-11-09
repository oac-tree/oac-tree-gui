/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include "job_handler.h"
#include "job_utils.h"
#include "user_input_dialogs.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/operation/message_panel.h>

#include <sup/dto/anyvalue.h>

#include <algorithm>

namespace sequencergui
{

JobManager::JobManager(QObject *parent)
    : QObject(parent), m_current_delay(GetDefaultTickTimeoutMsc())
{
}

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
    if (m_message_panel)
    {
      m_message_panel->SetLog(job_handler->GetJobLog());
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
    job_handler->onStartRequest();
  }
}

void JobManager::OnPauseJobRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->onPauseRequest();
  }
}

void JobManager::OnStopJobRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->onStopRequest();
  }
}

void JobManager::OnMakeStepRequest()
{
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->onMakeStepRequest();
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

void JobManager::SetMessagePanel(MessagePanel *panel)
{
  m_message_panel = panel;
}

void JobManager::OnChangeDelayRequest(int msec)
{
  m_current_delay = msec;
  if (auto job_handler = GetCurrentJobHandler(); job_handler)
  {
    job_handler->SetSleepTime(m_current_delay);
  }
}

bool JobManager::HasRunningJobs() const
{
  return std::any_of(m_job_map.begin(), m_job_map.end(),
                     [](const auto &iter) { return iter.second->IsRunning(); });
}

void JobManager::StopAllJobs()
{
  std::for_each(m_job_map.begin(), m_job_map.end(),
                [](const auto &iter) { iter.second->onStopRequest(); });
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
  auto on_user_input = [](const auto &args) { return GetAnyValueEditorDialogResult(args); };

  auto on_user_choice = [](const auto &args) { return GetUserChoiceDialogResult(args); };

  auto job_handler = std::make_unique<JobHandler>(item);
  connect(job_handler.get(), &JobHandler::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);
  connect(job_handler.get(), &JobHandler::NextLeavesChanged, this,
          &JobManager::OnNextLeavesChanged);

  job_handler->onPrepareJobRequest();
  // FIXME two calls below must be after onPrepareJobRequest
  job_handler->SetUserContext({on_user_input, on_user_choice});
  job_handler->SetSleepTime(m_current_delay);

  return job_handler;
}

}  // namespace sequencergui
