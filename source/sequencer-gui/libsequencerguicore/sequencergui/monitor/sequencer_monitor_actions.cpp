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

#include "sequencergui/monitor/sequencer_monitor_actions.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/core/message_handler_factory.h"
#include "sequencergui/jobsystem/job_manager.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_item.h"

#include <iostream>
#include <sstream>

namespace
{

//! Invokes
template <typename T>
bool InvokeAndCatch(T method, const std::string &message,
                    sequencergui::MessageHandlerInterface *message_interface)
{
  try
  {
    std::invoke(method);
    return true;
  }
  catch (const std::exception &ex)
  {
    std::ostringstream ostr;
    ostr << message << " falied with the message '" << std::string(ex.what()) << "'";
    message_interface->SendMessage(ostr.str());
    return false;
  }

  return false;
}
}  // namespace

namespace sequencergui
{

SequencerMonitorActions::SequencerMonitorActions(JobManager *job_manager,
                                                 selection_callback_t selection_callback,
                                                 QObject *parent)
    : QObject(parent)
    , m_job_manager(job_manager)
    , m_job_selection_callback(std::move(selection_callback))
    , m_message_handler(CreateNullMessageHandler())

{
}

SequencerMonitorActions::~SequencerMonitorActions() = default;

void SequencerMonitorActions::SetMessageHandler(
    std::unique_ptr<MessageHandlerInterface> message_handler)
{
  m_message_handler = std::move(message_handler);
}

void SequencerMonitorActions::SetJobModel(JobModel *job_model)
{
  m_job_model = job_model;
}

void SequencerMonitorActions::OnSubmitJobRequest(ProcedureItem *procedure_item)
{
  if (!procedure_item)
  {
    return;
  }

  CheckConditions();

  auto job = m_job_model->InsertItem<JobItem>();
  job->SetProcedure(procedure_item);

  InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                 m_message_handler.get());

  emit MakeJobSelectedRequest(job);
}

void SequencerMonitorActions::OnStartJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnStartJobRequest();
}

void SequencerMonitorActions::OnPauseJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnPauseJobRequest();
}

void SequencerMonitorActions::OnStopJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnStopJobRequest();
}

void SequencerMonitorActions::OnMakeStepRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnMakeStepRequest();
}

void SequencerMonitorActions::OnRemoveJobRequest()
{
  CheckConditions();

  auto job = m_job_selection_callback();

  if (!job)
  {
    return;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->OnRemoveJobRequest(job); },
                                   "Job removal", m_message_handler.get());

  if (is_success)
  {
    std::cout << m_job_model->GetTopItems<JobItem>().size() << std::endl;
    m_job_model->RemoveItem(job);
    std::cout << m_job_model->GetTopItems<JobItem>().size() << std::endl;
  }
}

void SequencerMonitorActions::OnRegenerateJobRequest()
{
  CheckConditions();

  auto job = m_job_selection_callback();

  if (!job)
  {
    return;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->OnRemoveJobRequest(job); },
                                   "Job removal", m_message_handler.get());

  if (is_success)
  {
    InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                   m_message_handler.get());

    emit MakeJobSelectedRequest(job);
  }
}

void SequencerMonitorActions::CheckConditions()
{
  if (!m_job_manager)
  {
    throw RuntimeException("JobManager is not defined");
  }

  if (!m_job_model)
  {
    throw RuntimeException("JobModel is not defined");
  }

  if (!m_job_selection_callback)
  {
    throw RuntimeException("Selection callback is not defined");
  }
}

}  // namespace sequencergui
