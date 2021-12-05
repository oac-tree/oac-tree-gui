/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/jobcontext.h"
#include "sequencergui/monitor/messagepanel.h"
#include "sequencergui/monitor/monitortreetoolbar.h"

#include <QDebug>

namespace sequi
{

JobManager::JobManager(QObject *parent)
    : QObject(parent), m_current_delay(MonitorTreeToolBar::GetDefaultDelay())
{
}

JobManager::~JobManager() = default;

void JobManager::SetModel(SequencerModel *model)
{
  m_model = model;
}

//! Set JobManager to display the status of given procedure.

void JobManager::SetCurrentProcedure(ProcedureItem *procedure)
{
  if (m_current_procedure == procedure)
  {
    return;
  }

  if (auto current_context = GetCurrentContext(); current_context)
  {
    // previous context, if exists,  will be detached from the message panel
    current_context->SetMessagePanel(nullptr);
    m_message_panel->onClearLog();
  }

  m_current_procedure = procedure;

  if (auto current_context = GetCurrentContext(); current_context)
  {
    // new context will be attached to the message panel
    current_context->SetMessagePanel(m_message_panel);
  }
  else
  {
    auto context = CreateContext();
    context->SetWaitingMode(WaitingMode::kSleepFor);
  }
}

JobContext *JobManager::GetCurrentContext()
{
  auto it = m_context_map.find(m_current_procedure);
  return it == m_context_map.end() ? nullptr : it->second;
}

void JobManager::onStartProcedureRequest(ProcedureItem *procedure)
{
  if (!procedure)
  {
    return;
  }

  SetCurrentProcedure(procedure);

  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->SetWaitingMode(WaitingMode::kSleepFor);
    current_context->onStartRequest();
  }
}

void JobManager::onPauseProcedureRequest()
{
  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->onPauseRequest();
  }
}

void JobManager::onStopProcedureRequest()
{
  if (auto current_context = GetCurrentContext(); current_context)
  {
    current_context->onStopRequest();
  }
}

void JobManager::onMakeStepRequest()
{
  if (!GetCurrentContext())
  {
    auto context = CreateContext();
    context->SetWaitingMode(WaitingMode::kWaitForRelease);
  }

  if (auto current_context = GetCurrentContext(); current_context)
  {
    if (!current_context->IsRunning())
    {
      current_context->SetWaitingMode(WaitingMode::kWaitForRelease);
      current_context->onStartRequest();
    }
    else
    {
      current_context->onMakeStepRequest();
    }
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

JobContext *JobManager::CreateContext()
{
  auto context = new JobContext(m_current_procedure, this);
  context->SetSleepTime(m_current_delay);
  context->SetMessagePanel(m_message_panel);
  connect(context, &JobContext::InstructionStatusChanged, this,
          &JobManager::InstructionStatusChanged);
  m_context_map[m_current_procedure] = context;
  context->onPrepareJobRequest();
  return context;
}

}  // namespace sequi
