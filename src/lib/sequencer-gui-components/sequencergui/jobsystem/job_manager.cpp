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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/monitor/message_panel.h>
#include <sequencergui/pvmonitor/anyvalue_editor_dialog.h>

#include <mvvm/widgets/widget_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <QInputDialog>
#include <QMainWindow>

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

UserInputResult JobManager::OnUserInputRequest(const UserInputArgs &args)
{
  AnyValueEditorDialog dialog(mvvm::utils::FindMainWindow());

  auto anyvalue_item = sup::gui::CreateItem(args.value);

  dialog.SetInitialValue(anyvalue_item.get());
  if (dialog.exec() == QDialog::Accepted)
  {
    auto anyvalue = sup::gui::CreateAnyValue(*dialog.GetResult());
    return UserInputResult{anyvalue, true};
  }

  return UserInputResult{{}, false};
}

UserChoiceResult JobManager::OnUserChoiceRequest(const UserChoiceArgs &args)
{
  QStringList with_index_added;
  int index{0};
  for (const auto &option : args.options)
  {
    with_index_added.push_back(QString("%1 %2").arg(index++).arg(QString::fromStdString(option.first)));
  }
  auto selection = QInputDialog::getItem(
      nullptr, "Input request", QString::fromStdString("aaa"), with_index_added);
  return {with_index_added.indexOf(selection), true};
}

std::unique_ptr<JobHandler> JobManager::CreateJobHandler(JobItem *item)
{
  auto on_user_input = [this](const auto &args) { return OnUserInputRequest(args); };

  auto on_user_choice = [this](const auto &args) { return OnUserChoiceRequest(args); };

  auto job_handler = std::make_unique<JobHandler>(item);
  connect(job_handler.get(), &JobHandler::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);

  job_handler->onPrepareJobRequest();
  // FIXME two calls below must be after onPrepareJobRequest
  job_handler->SetUserContext({on_user_input, on_user_choice});
  job_handler->SetSleepTime(m_current_delay);

  return job_handler;
}

}  // namespace sequencergui
