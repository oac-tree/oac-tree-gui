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

#include "operation_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sup/gui/core/standard_message_handlers.h>

#include <QAbstractTableModel>
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
    , m_message_handler(
          std::make_unique<sup::gui::ThrowingMessageHandler<::sequencergui::RuntimeException>>())
    , m_default_delay(itemconstants::kDefaultTickTimeoutMsec)

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

bool OperationActionHandler::OnSubmitJobRequest(ProcedureItem *procedure_item)
{
  if (!procedure_item)
  {
    return false;
  }

  CheckConditions();

  auto job = m_job_model->InsertItem<JobItem>();
  job->SetProcedure(procedure_item);
  job->SetDisplayName(procedure_item->GetDisplayName());
  job->SetTickTimeout(m_default_delay);

  auto result = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                               m_message_handler.get());

  emit MakeJobSelectedRequest(job);

  return result;
}

void OperationActionHandler::OnStartJobRequest()
{
  CheckConditions();

  ResubmitIfNecessary();

  m_job_manager->SetCurrentJob(GetSelectedJob());

  m_job_manager->OnStartJobRequest();
}

void OperationActionHandler::OnPauseJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(GetSelectedJob());
  m_job_manager->OnPauseJobRequest();
}

void OperationActionHandler::OnStopJobRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(GetSelectedJob());
  m_job_manager->OnStopJobRequest();
}

void OperationActionHandler::OnMakeStepRequest()
{
  CheckConditions();

  m_job_manager->SetCurrentJob(GetSelectedJob());

  ResubmitIfNecessary();

  m_job_manager->OnMakeStepRequest();
}

bool OperationActionHandler::OnRemoveJobRequest()
{
  CheckConditions();

  auto job = GetSelectedJob();

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
  auto job = GetSelectedJob();
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

  auto job = GetSelectedJob();

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

void OperationActionHandler::OnSetTickTimeoutRequest(int msec)
{
  m_default_delay = msec;
  if (auto job_item = GetSelectedJob(); job_item)
  {
    job_item->SetTickTimeout(msec);
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
    if (status == RunnerStatus::kFailed || status == RunnerStatus::kSucceeded
        || status == RunnerStatus::kHalted)
    {
      OnRegenerateJobRequest();
    }
  }
}

JobItem *OperationActionHandler::GetSelectedJob()
{
  return m_job_selection_callback ? m_job_selection_callback() : nullptr;
}

}  // namespace sequencergui
