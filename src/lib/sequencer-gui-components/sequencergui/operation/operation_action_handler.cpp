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
#include <sequencergui/jobsystem/remote_connection_info.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_job_items.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>

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

void OperationActionHandler::SetJobContainer(mvvm::SessionItem *job_container)
{
  m_job_container = job_container;
}

bool OperationActionHandler::SubmitLocalJob(ProcedureItem *procedure_item)
{
  if (!procedure_item)
  {
    return false;
  }

  return SubmitJob(CreateLocalJobItem(procedure_item, m_tick_timeout));
}

bool OperationActionHandler::SubmitImportedJob(std::unique_ptr<ProcedureItem> procedure_item)
{
  if (!procedure_item)
  {
    return false;
  }

  return SubmitJob(CreateImportedJobItem(std::move(procedure_item), m_tick_timeout));
}

bool OperationActionHandler::OnImportRemoteJobRequest()
{
  if (!m_operation_context.get_remote_connection_info)
  {
    return false;
  }

  bool is_success{false};
  if (auto user_choice = m_operation_context.get_remote_connection_info(); user_choice.has_value())
  {
    is_success = true;
    auto user_choice_value = user_choice.value();
    for (auto index : user_choice_value.job_indexes)
    {
      // all should succeed
      is_success &= SubmitJob(CreateRemoteJobItem(user_choice_value.server_name, index));
    }
  }

  return is_success;
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

void OperationActionHandler::OnResetJobRequest()
{
  m_job_manager->Reset(GetSelectedJob());
}

bool OperationActionHandler::OnRemoveJobRequest()
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
    auto next_to_select = mvvm::utils::FindNextSiblingToSelect(job);
    GetModel()->RemoveItem(job);
    if (next_to_select)
    {
      // suggest to select something else instead of just deleted item
      emit MakeJobSelectedRequest(dynamic_cast<JobItem *>(next_to_select));
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

bool OperationActionHandler::SubmitJob(std::unique_ptr<JobItem> job_item)
{
  auto job = InsertJobAfterCurrentSelection(std::move(job_item));

  auto result = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                               m_operation_context.send_message);

  // current implementation is that even if submission fails, JobItem remains in a list
  emit MakeJobSelectedRequest(job);

  return result;
}

JobItem *OperationActionHandler::InsertJobAfterCurrentSelection(std::unique_ptr<JobItem> job_item)
{
  if (!GetModel())
  {
    throw RuntimeException("JobModel is not defined");
  }

  auto result = job_item.get();

  auto selected_job = GetSelectedJob();
  auto tagindex = selected_job ? selected_job->GetTagIndex().Next() : mvvm::TagIndex::Append();
  GetModel()->InsertItem(std::move(job_item), GetJobContainer(), tagindex);
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

mvvm::ISessionModel *OperationActionHandler::GetModel() const
{
  return GetJobContainer() ? GetJobContainer()->GetModel() : nullptr;
}

mvvm::SessionItem *OperationActionHandler::GetJobContainer() const
{
  return m_job_container;
}

JobItem *OperationActionHandler::GetSelectedJob()
{
  return m_operation_context.selected_job();
}

}  // namespace sequencergui
