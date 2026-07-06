/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "operation_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/remote_connection_info.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>

#include <optional>

namespace oac_tree_gui
{

namespace
{

template <typename T>
bool InvokeAndCatch(T method, const std::string& text,
                    const std::function<void(const sup::gui::MessageEvent&)>& send_message)
{
  try
  {
    std::invoke(method);
    return true;
  }
  catch (const std::exception& ex)
  {
    const sup::gui::MessageEvent message =
        sup::gui::CreateInvalidOperationMessage(text + " failed", ex.what());
    send_message(message);
    return false;
  }

  return false;
}

}  // namespace

OperationActionHandler::OperationActionHandler(IJobItemManager* job_manager,
                                               OperationActionContext operation_context,
                                               QObject* parent_object)
    : QObject(parent_object)
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

  if (m_job_manager == nullptr)
  {
    throw RuntimeException("JobManager is not defined");
  }
}

OperationActionHandler::~OperationActionHandler() = default;

void OperationActionHandler::SetJobContainer(mvvm::SessionItem* job_container)
{
  m_job_container = job_container;
}

bool OperationActionHandler::SubmitLocalJob(ProcedureItem* procedure_item)
{
  if (procedure_item == nullptr)
  {
    return false;
  }

  return SubmitJob(CreateLocalJobItem(procedure_item, std::chrono::milliseconds{m_tick_timeout}));
}

bool OperationActionHandler::SubmitFileBasedJob(const std::string& file_name)
{
  return SubmitJob(CreateFileBasedJobItem(file_name, std::chrono::milliseconds{m_tick_timeout}));
}

bool OperationActionHandler::OnImportRemoteJobRequest()
{
  if (!m_operation_context.get_remote_connection_info)
  {
    return false;
  }

  bool is_success{false};
  if (auto connection_info = m_operation_context.get_remote_connection_info();
      connection_info.has_value())
  {
    is_success = true;
    auto connection_info_value = connection_info.value();
    for (auto index : connection_info_value.job_indexes)
    {
      // all should succeed
      is_success &= SubmitJob(CreateRemoteJobItem(connection_info_value.server_info, index));
    }
  }

  return is_success;
}

void OperationActionHandler::OnStartJobRequest(JobItem* job)
{
  m_job_manager->Start(job);
}

void OperationActionHandler::OnPauseJobRequest(JobItem* job)
{
  m_job_manager->Pause(job);
}

void OperationActionHandler::OnMakeStepRequest(JobItem* job)
{
  m_job_manager->Step(job);
}

void OperationActionHandler::OnStopJobRequest(JobItem* job)
{
  m_job_manager->Stop(job);
}

void OperationActionHandler::OnResetJobRequest(JobItem* job)
{
  m_job_manager->Reset(job);
}

bool OperationActionHandler::OnRemoveJobRequest()
{
  auto job = GetSelectedJob();

  if (job == nullptr)
  {
    return false;
  }

  // capture the remote server before removal, so the connection can be cleaned up afterwards
  const auto remote_server_info = GetAutomationServerInfo(*job);

  // a running job can be removed only after an explicit user confirmation
  auto removal_policy = RemovalPolicy::kRejectIfRunning;
  if (auto job_handler = m_job_manager->GetJobHandler(job);
      job_handler != nullptr && job_handler->IsRunning())
  {
    const std::string question =
        IsRemoteJobItem(*job)
            ? "Do you really want to remove the running remote job? It will keep running on the "
              "server."
            : "Do you really want to remove the running local job? This will stop it.";

    if (!m_operation_context.confirm_job_removal
        || !m_operation_context.confirm_job_removal(question))
    {
      return false;
    }
    removal_policy = RemovalPolicy::kForce;
  }

  auto is_success = InvokeAndCatch([this, job, removal_policy]()
                                   { m_job_manager->RemoveJobHandler(job, removal_policy); },
                                   "Job removal", m_operation_context.send_message);

  if (is_success)
  {
    auto next_to_select = mvvm::utils::FindNextSiblingToSelect(job);
    GetModel()->RemoveItem(job);

    // The job (and its handler) is gone now; drop the remote connection if no other remote job
    // uses the same automation server.
    if (remote_server_info.has_value() && m_operation_context.drop_remote_connection
        && !IsAutomationServerInUse(m_job_manager->GetJobItems(), remote_server_info.value()))
    {
      m_operation_context.drop_remote_connection(remote_server_info.value());
    }

    if (next_to_select != nullptr)
    {
      // suggest to select something else instead of just deleted item
      emit MakeJobSelectedRequest(dynamic_cast<const JobItem*>(next_to_select));
    }
  }

  return is_success;
}

bool OperationActionHandler::OnRegenerateJobRequest()
{
  auto job = GetSelectedJob();

  if (job == nullptr)
  {
    return false;
  }

  auto is_success = InvokeAndCatch(
      [this, job]() { m_job_manager->RemoveJobHandler(job, RemovalPolicy::kRejectIfRunning); },
      "Job removal", m_operation_context.send_message);

  if (is_success)
  {
    is_success = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                                m_operation_context.send_message);

    job->SetStatus(is_success ? RunnerStatus::kUndefined : RunnerStatus::kSubmitFailure);
    if (is_success)
    {
      // FIXME This is quick hack to notify OperationSplittableWidget
      // that extended procedure on board of JobItem has changed due to resubmit.
      // We emit nullptr to pretend that JobItem has changed, even if it is remained the same.
      emit MakeJobSelectedRequest(nullptr);

      emit MakeJobSelectedRequest(job);
    }
  }

  return is_success;
}

void OperationActionHandler::OnSetTickTimeoutRequest(std::chrono::milliseconds timeout)
{
  m_tick_timeout = timeout;
}

void OperationActionHandler::OnToggleBreakpoint(JobItem* job, InstructionItem* instruction)
{
  if (auto job_handler = m_job_manager->GetJobHandler(job); job_handler)
  {
    job_handler->OnToggleBreakpointRequest(instruction);
  }
}

bool OperationActionHandler::SubmitJob(std::unique_ptr<JobItem> job_item)
{
  auto job = InsertJobAfterCurrentSelection(std::move(job_item));

  auto result = InvokeAndCatch([this, job]() { m_job_manager->SubmitJob(job); }, "Job submission",
                               m_operation_context.send_message);

  if (!result)
  {
    job->SetStatus(RunnerStatus::kSubmitFailure);
  }

  // current implementation is that even if submission fails, JobItem remains in a list
  emit MakeJobSelectedRequest(job);

  return result;
}

JobItem* OperationActionHandler::InsertJobAfterCurrentSelection(std::unique_ptr<JobItem> job_item)
{
  if (GetModel() == nullptr)
  {
    throw RuntimeException("JobModel is not defined");
  }

  auto result = job_item.get();

  auto selected_job = GetSelectedJob();
  auto tagindex =
      (selected_job != nullptr) ? selected_job->GetTagIndex().Next() : mvvm::TagIndex::Append();
  GetModel()->InsertItem(std::move(job_item), GetJobContainer(), tagindex);
  return result;
}

void OperationActionHandler::SendMessage(const std::string& text, const std::string& informative,
                                         const std::string& details) const
{
  SendMessage(sup::gui::CreateInvalidOperationMessage(text, informative, details));
}

void OperationActionHandler::SendMessage(const sup::gui::MessageEvent& message_event) const
{
  m_operation_context.send_message(message_event);
}

mvvm::ISessionModel* OperationActionHandler::GetModel() const
{
  return (GetJobContainer() != nullptr) ? GetJobContainer()->GetModel() : nullptr;
}

mvvm::SessionItem* OperationActionHandler::GetJobContainer() const
{
  return m_job_container;
}

JobItem* OperationActionHandler::GetSelectedJob() const
{
  return const_cast<JobItem*>(m_operation_context.selected_job());
}

}  // namespace oac_tree_gui
