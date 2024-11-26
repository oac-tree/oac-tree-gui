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
#include <sequencergui/jobsystem/remote_connection_context.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_job_items.h>
#include <sup/gui/core/standard_message_handlers.h>

namespace sequencergui
{

namespace
{

template <typename T>
bool InvokeAndCatch(T method, const std::string &text,
                    const std::function<void(const sup::gui::MessageEvent &)> &send_message)
{
  try
  {
    std::invoke(method);
    return true;
  }
  catch (const std::exception &ex)
  {
    const sup::gui::MessageEvent message =
        sup::gui::CreateInvalidOperationMessage(text + " failed", ex.what());
    send_message(message);
    return false;
  }

  return false;
}

}  // namespace

OperationActionHandler::OperationActionHandler(IJobItemManager *job_manager,
                                               OperationActionContext operation_context,
                                               QObject *parent)
    : QObject(parent)
    , m_job_manager(job_manager)
    , m_operation_context(std::move(operation_context))
    , m_tick_timeout(itemconstants::kDefaultTickTimeoutMsec)
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

void OperationActionHandler::SetJobModel(JobModel *job_model)
{
  m_job_model = job_model;
}

bool OperationActionHandler::SubmitLocalJob(ProcedureItem *procedure_item)
{
  if (!procedure_item)
  {
    return false;
  }

  auto job = InsertJobAfterCurrentSelection(CreateLocalJobItem(procedure_item, m_tick_timeout));

  auto result = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                               m_operation_context.send_message);

  return result;
}

bool OperationActionHandler::OnImportRemoteJobRequest()
{
  if (!m_operation_context.get_remote_context)
  {
    return false;
  }

  if (auto user_choice = m_operation_context.get_remote_context(); user_choice.has_value())
  {
    auto user_choice_value = user_choice.value();

    for (auto index : user_choice_value.job_indexes)
    {
      auto job =
          InsertJobAfterCurrentSelection(CreateRemoteJobItem(user_choice_value.server_name, index));

      auto result = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); },
                                   "Remote job submission", m_operation_context.send_message);
    }

    return true;
  }

  return false;
}

void OperationActionHandler::OnStartJobRequest()
{
  m_job_manager->Start(GetSelectedJob());
}

void OperationActionHandler::OnPauseJobRequest()
{
  m_job_manager->Pause(GetSelectedJob());
}

void OperationActionHandler::OnStopJobRequest()
{
  m_job_manager->Stop(GetSelectedJob());
}

void OperationActionHandler::OnMakeStepRequest()
{
  m_job_manager->Step(GetSelectedJob());
}

bool OperationActionHandler::OnRemoveJobRequest(bool cleanup)
{
  auto job = GetSelectedJob();

  if (!job)
  {
    return false;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->RemoveJobHandler(job); },
                                   "Job removal", m_operation_context.send_message);

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

bool OperationActionHandler::OnRegenerateJobRequest()
{
  auto job = GetSelectedJob();

  if (!job)
  {
    return false;
  }

  auto is_success = InvokeAndCatch([this, job]() { m_job_manager->RemoveJobHandler(job); },
                                   "Job removal", m_operation_context.send_message);

  if (is_success)
  {
    is_success = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                                m_operation_context.send_message);

    if (is_success)
    {
      job->SetStatus({});
      emit MakeJobSelectedRequest(job);
    }
  }

  return is_success;
}

void OperationActionHandler::OnSetTickTimeoutRequest(int msec)
{
  m_tick_timeout = msec;
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

JobItem *OperationActionHandler::InsertJobAfterCurrentSelection(std::unique_ptr<JobItem> job_item)
{
  if (!m_job_model)
  {
    throw RuntimeException("JobModel is not defined");
  }

  auto result = job_item.get();

  auto selected_job = GetSelectedJob();
  auto parent = selected_job ? selected_job->GetParent() : m_job_model->GetRootItem();
  auto tagindex = selected_job ? selected_job->GetTagIndex().Next() : mvvm::TagIndex::Append();
  m_job_model->InsertItem(std::move(job_item), parent, tagindex);
  emit MakeJobSelectedRequest(result);
  return result;
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
