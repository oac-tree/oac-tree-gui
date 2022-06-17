/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/job_manager.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/jobsystem/job_context.h"
#include "sequencergui/jobsystem/job_utils.h"
#include "sequencergui/jobsystem/user_context.h"
#include "sequencergui/model/instruction_container_item.h"
#include "sequencergui/model/instruction_item.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/monitor/message_panel.h"
#include "sequencergui/monitor/monitor_realtime_toolbar.h"
#include "sequencergui/widgets/widget_utils.h"

#include <QDebug>
#include <QInputDialog>
#include <iostream>
#include <sstream>

namespace sequencergui
{

JobManager::JobManager(QObject *parent)
    : QObject(parent)
    , m_current_delay(GetDefaultTickTimeoutMsc())
{
}

void JobManager::SubmitJob(JobItem *job)
{
  if (!job)
  {
    throw RuntimeException("Attempt to submit undefined job");
  }

  if (auto context = GetContext(job); context)
  {
    throw RuntimeException("Attempt to submit already existing job");
  }

  auto context = CreateContext(job);
  m_context_map.insert({job, std::move(context)});
}

JobManager::~JobManager() = default;

//! Set JobManager to display the status of given job.

void JobManager::SetCurrentJob(JobItem *job)
{
  if (m_current_job == job)
  {
    return;
  }

  // previous context, if exists,  will be detached from the message panel
  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->SetMessagePanel(nullptr);
    m_message_panel->onClearLog();
  }

  m_current_job = job;

  if (auto current_context = GetCurrentContext(); current_context)
  {
    // new context will be attached to the message panel
    current_context->SetMessagePanel(m_message_panel);
  }
}

JobContext *JobManager::GetCurrentContext()
{
  return GetContext(m_current_job);
}

//! Returns context for JobItem.

JobContext *JobManager::GetContext(JobItem *job)
{
  auto it = m_context_map.find(job);
  return it == m_context_map.end() ? nullptr : it->second.get();
}

//! Returns current job. It is the one that is attached to the MessagePanel and is the recipient
//! of start/stop/pause requests.

JobItem *JobManager::GetCurrentJob()
{
  return m_current_job;
}

void JobManager::OnStartJobRequest()
{
  qDebug() << "JobManager::onStartProcedureRequest()";
  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->onStartRequest();
  }
}

void JobManager::OnPauseJobRequest()
{
  qDebug() << "JobManager::onPauseProcedureRequest()";
  if (auto context = GetCurrentContext(); context)
  {
    context->onPauseRequest();
  }
}

void JobManager::OnStopJobRequest()
{
  qDebug() << "JobManager::onStopProcedureRequest()";
  if (auto context = GetCurrentContext(); context)
  {
    context->onStopRequest();
  }
}

void JobManager::OnMakeStepRequest()
{
  if (auto context = GetCurrentContext(); context)
  {
    context->onMakeStepRequest();
  }
}

void JobManager::OnRemoveJobRequest(JobItem *job)
{
  if (auto context = GetContext(job); context)
  {
    if (context->IsRunning())
    {
      throw RuntimeException("Attempt to modify running job");
    }

    if (job == GetCurrentJob())
    {
      SetCurrentJob(nullptr);
    }

    m_context_map.erase(job);
  }
}

void JobManager::SetMessagePanel(MessagePanel *panel)
{
  m_message_panel = panel;
}

void JobManager::onChangeDelayRequest(int msec)
{
  m_current_delay = msec;
  if (auto context = GetCurrentContext(); context)
  {
    context->SetSleepTime(m_current_delay);
  }
}

QString JobManager::onUserInputRequest(const QString &current_value, const QString &description)
{
  return QInputDialog::getText(nullptr, "Input request", description, QLineEdit::Normal,
                               current_value);
}

int JobManager::onUserChoiceRequest(const QStringList &choices, const QString &description)
{
  QStringList with_index_added;
  int index{0};
  for (const auto &str : choices)
  {
    with_index_added.push_back(QString("%1 %2").arg(index++).arg(str));
  }
  auto selection = QInputDialog::getItem(nullptr, "Input request", description, with_index_added);
  return with_index_added.indexOf(selection);
}

std::unique_ptr<JobContext> JobManager::CreateContext(JobItem *item)
{
  auto on_user_input = [this](auto value, auto description)
  { return onUserInputRequest(value, description); };

  auto on_user_choice = [this](auto choices, auto description)
  { return onUserChoiceRequest(choices, description); };

  auto context = std::make_unique<JobContext>(item);
  context->SetMessagePanel(m_message_panel);
  connect(context.get(), &JobContext::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);

  context->onPrepareJobRequest();
  // FIXME two calls below must be after onPrepareJobContext
  context->SetUserContext({on_user_input, on_user_choice});
  context->SetSleepTime(m_current_delay);

  return context;
}

}  // namespace sequencergui
