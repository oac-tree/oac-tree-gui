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
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/jobsystem/local_job_handler.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sup/gui/core/standard_message_handlers.h>

#include <QAbstractTableModel>

namespace
{

//! Invokes
template <typename T>
bool InvokeAndCatch(T method, const std::string &text,
                    sup::gui::MessageHandlerInterface *message_interface)
{
  try
  {
    std::invoke(method);
    return true;
  }
  catch (const std::exception &ex)
  {
    sup::gui::MessageEvent message =
        sup::gui::CreateInvalidOperationMessage(text + " failed", ex.what());
    message_interface->SendMessage(message);
    return false;
  }

  return false;
}
}  // namespace

namespace sequencergui
{

OperationActionHandler::OperationActionHandler(JobManager *job_manager,
                                               OperationActionContext operation_context,
                                               UserContext user_context, QObject *parent)
    : QObject(parent)
    , m_job_manager(job_manager)
    , m_operation_context(std::move(operation_context))
    , m_user_context(user_context)
    , m_message_handler(
          std::make_unique<sup::gui::ThrowingMessageHandler<::sequencergui::RuntimeException>>())
    , m_default_delay(itemconstants::kDefaultTickTimeoutMsec)

{
  if (!m_operation_context.selected_job)
  {
    throw RuntimeException("Selection callback is not defined");
  }

  if (!m_operation_context.send_message)
  {
    throw RuntimeException("Send message callback is not defined");
  }

  if (!m_job_manager)
  {
    throw RuntimeException("JobManager is not defined");
  }
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

  m_job_manager->Start(GetSelectedJob());
}

void OperationActionHandler::OnPauseJobRequest()
{
  CheckConditions();

  m_job_manager->Pause(GetSelectedJob());
}

void OperationActionHandler::OnStopJobRequest()
{
  CheckConditions();

  m_job_manager->Stop(GetSelectedJob());
}

void OperationActionHandler::OnMakeStepRequest()
{
  CheckConditions();

  m_job_manager->Step(GetSelectedJob());
}

bool OperationActionHandler::OnRemoveJobRequest(bool cleanup)
{
  CheckConditions();

  auto job = GetSelectedJob();

  if (!job)
  {
    return false;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->RemoveJobHandler(job); },
                                   "Job removal", m_message_handler.get());

  if (is_success)
  {
    auto procedure = job->GetProcedure();
    m_job_model->RemoveItem(job);
    if (cleanup)
    {
      procedure->GetModel()->RemoveItem(procedure);
    }
  }

  return is_success;
}

void OperationActionHandler::OnRegenerateJobRequest()
{
  CheckConditions();

  auto job = GetSelectedJob();

  if (!job)
  {
    return;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->RemoveJobHandler(job); },
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

void OperationActionHandler::OnToggleBreakpoint(InstructionItem *instruction)
{
  if (auto job_handler = m_job_manager->GetJobHandler(GetSelectedJob()); job_handler)
  {
    job_handler->OnToggleBreakpointRequest(instruction);
  }
}

void OperationActionHandler::CheckConditions()
{
  if (!m_job_model)
  {
    throw RuntimeException("JobModel is not defined");
  }
}

void OperationActionHandler::SendMessage(const std::string &text, const std::string &informative,
                                         const std::string &details) const
{
  SendMessage(sup::gui::CreateInvalidOperationMessage(text, informative, details));
}

void OperationActionHandler::SendMessage(const sup::gui::MessageEvent &message_event) const
{
  m_operation_context.send_message(message_event);
}

JobItem *OperationActionHandler::GetSelectedJob()
{
  return m_operation_context.selected_job();
}

}  // namespace sequencergui
