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

#include "sequencergui/monitor/jobmanager.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/model/instructioncontaineritem.h"
#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/jobitem.h"
#include "sequencergui/model/jobmodel.h"
#include "sequencergui/monitor/jobcontext.h"
#include "sequencergui/monitor/jobutils.h"
#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/monitor/monitorrealtimetoolbar.h"
#include "sequencergui/monitor/usercontext.h"
#include "sequencergui/widgets/widgetutils.h"

#include <QDebug>
#include <QInputDialog>
#include <iostream>

namespace sequencergui
{

JobManager::JobManager(QObject *parent)
    : QObject(parent), m_current_delay(GetDefaultTickTimeoutMsc())
{
}

JobManager::~JobManager() = default;

//! Set JobManager to display the status of given job.

void JobManager::SetCurrentJob(JobItem *job)
{
  if (!job)
  {
    return;
  }

  if (m_current_job == job)
  {
    return;
  }

  if (auto current_context = GetCurrentContext(); current_context)
  {
    // previous context, if exists,  will be detached from the message panel
    current_context->SetMessagePanel(nullptr);
    m_message_panel->onClearLog();
  }

  m_current_job = job;

  if (auto current_context = GetCurrentContext(); current_context)
  {
    // new context will be attached to the message panel
    current_context->SetMessagePanel(m_message_panel);
  }
  else
  {
    auto context = CreateContext();
  }
}

JobContext *JobManager::GetCurrentContext()
{
  auto it = m_context_map.find(m_current_job);
  return it == m_context_map.end() ? nullptr : it->second;
}

void JobManager::onStartProcedureRequest(JobItem *job_item)
{
  if (!job_item)
  {
    return;
  }

  SetCurrentJob(job_item);

  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->onStartRequest();
  }
}

void JobManager::onPauseProcedureRequest()
{
  if (auto context = GetCurrentContext(); context)
  {
    context->onPauseRequest();
  }
}

void JobManager::onStopProcedureRequest()
{
  qDebug() << "JobManager::onStopProcedureRequest()";
  if (auto context = GetCurrentContext(); context)
  {
    context->onStopRequest();
  }
}

void JobManager::onMakeStepRequest()
{
  if (auto context = GetCurrentContext(); context)
  {
    context->onMakeStepRequest();
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

JobContext *JobManager::CreateContext()
{
  auto it = m_context_map.find(m_current_job);
  if (it != m_context_map.end())
  {
    throw RuntimeException("JobManager::CreateContext() : already created context");
  }

  auto on_user_input = [this](auto value, auto description)
  { return onUserInputRequest(value, description); };

  auto on_user_choice = [this](auto choices, auto description)
  { return onUserChoiceRequest(choices, description); };

  auto context = new JobContext(m_current_job, this);
  context->SetMessagePanel(m_message_panel);
  connect(context, &JobContext::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);

  context->SetUserContext({on_user_input, on_user_choice});

  InvokeAndCatch([context]() { context->onPrepareJobRequest(); });
  std::cout << "XXXXX " << m_current_delay << std::endl;
  context->SetSleepTime(m_current_delay);  // FIXME must be after onPrepareJobContext

  // FIXME Refactor logic. What to do when context is pointing to invalid procedure?
  m_context_map.insert({m_current_job, context});

  return context;
}

}  // namespace sequencergui
