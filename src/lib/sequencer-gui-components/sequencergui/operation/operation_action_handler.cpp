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

#include "operation_action_handler.h"

#include <sequencergui/components/message_handler_factory.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>

#include <QAbstractTableModel>
#include <iostream>
#include <sstream>

namespace
{

//! Invokes
template <typename T>
bool InvokeAndCatch(T method, const std::string &message,
                    sup::gui::MessageHandlerInterface *message_interface)
{
  try
  {
    std::invoke(method);
    return true;
  }
  catch (const std::exception &ex)
  {
    std::ostringstream ostr;
    ostr << message << " failed with the message '" << std::string(ex.what()) << "'";
    message_interface->SendMessage(ostr.str());
    return false;
  }

  return false;
}
}  // namespace

namespace sequencergui
{

OperationActionHandler::OperationActionHandler(JobManager *job_manager,
                                                 selection_callback_t selection_callback,
                                                 QObject *parent)
    : QObject(parent)
    , m_job_manager(job_manager)
    , m_job_selection_callback(std::move(selection_callback))
    , m_message_handler(CreateThrowingMessageHandler())

{
}

OperationActionHandler::~OperationActionHandler() = default;

void OperationActionHandler::SetMessageHandler(
    std::unique_ptr<sup::gui::MessageHandlerInterface> message_handler)
{
  m_message_handler = std::move(message_handler);
}

void OperationActionHandler::SetJobModel(JobModel *job_model)
{
  m_job_model = job_model;
}

void OperationActionHandler::OnSubmitJobRequest(ProcedureItem *procedure_item)
{
  if (!procedure_item)
  {
    return;
  }

  CheckConditions();

  auto job = m_job_model->InsertItem<JobItem>();
  job->SetProcedure(procedure_item);
  job->SetDisplayName(procedure_item->GetDisplayName());

  InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                 m_message_handler.get());

  emit MakeJobSelectedRequest(job);
}

void OperationActionHandler::OnStartJobRequest()
{
  CheckConditions();

  ResubmitIfNecessary();

  m_job_manager->SetCurrentJob(m_job_selection_callback());

  m_job_manager->OnStartJobRequest();
}

void OperationActionHandler::OnPauseJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnPauseJobRequest();
}

void OperationActionHandler::OnStopJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());
  m_job_manager->OnStopJobRequest();
}

void OperationActionHandler::OnMakeStepRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(m_job_selection_callback());

  ResubmitIfNecessary();

  m_job_manager->OnMakeStepRequest();
}

bool OperationActionHandler::OnRemoveJobRequest()
{
  CheckConditions();

  auto job = m_job_selection_callback();

  if (!job)
  {
    return false;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->OnRemoveJobRequest(job); },
                                   "Job removal", m_message_handler.get());

  if (is_success)
  {
    m_job_model->RemoveItem(job);
  }

  return is_success;
}

//! Removes job and cleanup original ProcedureItem.

void OperationActionHandler::OnRemoveJobAndCleanupRequest()
{
  auto job = m_job_selection_callback();
  if (!job)
  {
    return;
  }

  auto procedure = job->GetProcedure();

  if (OnRemoveJobRequest())
  {
    procedure->GetModel()->RemoveItem(procedure);
  }
}

void OperationActionHandler::OnRegenerateJobRequest()
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
    auto is_success = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); },
                                     "Job submission", m_message_handler.get());

    if (is_success)
    {
      job->SetStatus({});
      emit MakeJobSelectedRequest(job);
    }
  }
}

void OperationActionHandler::CheckConditions()
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

//! Resubmit job if last time it was terminated, or was completed succesfully.

void OperationActionHandler::ResubmitIfNecessary()
{
  if (auto job_handler = m_job_manager->GetCurrentJobHandler(); job_handler)
  {
    auto status = job_handler->GetRunnerStatus();
    if (status == RunnerStatus::kFailed || status == RunnerStatus::kCompleted
        || status == RunnerStatus::kStopped)
    {
      OnRegenerateJobRequest();
    }
  }
}

}  // namespace sequencergui
