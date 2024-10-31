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

#include "automation_job_handler.h"

#include "remote_job_observer.h"

#include <sequencergui/jobsystem/domain_event_dispatcher.h>
#include <sequencergui/jobsystem/domain_event_dispatcher_context.h>
#include <sequencergui/jobsystem/domain_event_queue.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/transform/procedure_item_job_info_builder.h>
#include <sequencergui/transform/anyvalue_item_transform_helper.h>

#include <QDebug>
#include <iostream>

namespace sequencergui
{

AutomationJobHandler::AutomationJobHandler(JobItem *job_item,
                                           const sup::sequencer::JobInfo &job_info)
    : m_builder(std::make_unique<ProcedureItemJobInfoBuilder>())
    , m_job_observer(std::make_unique<RemoteJobObserver>(CreatePostEventCallback()))
    , m_event_queue(std::make_unique<DomainEventQueue>())
    , m_event_dispatcher(std::make_unique<DomainEventDispatcher>(CreateGetEventCallback(),
                                                                 CreateDispatcherContext()))
    , m_job_item(job_item)
{
  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);

  SetupProcedureItem(job_info);
}

AutomationJobHandler::~AutomationJobHandler() = default;

sup::sequencer::IJobInfoIO *AutomationJobHandler::GetJobObserver() const
{
  return m_job_observer.get();
}

void AutomationJobHandler::SetupProcedureItem(const sup::sequencer::JobInfo &job_info)
{
  auto procedure_item = m_builder->CreateProcedureItem(job_info);
  auto procedure_item_ptr = procedure_item.get();

  // The difference with JobHandler is that our Procedure and ExpandedProcedure is the same.
  m_job_item->GetModel()->InsertItem(std::move(procedure_item), m_job_item,
                                     mvvm::TagIndex::Append());
  m_job_item->SetProcedure(procedure_item_ptr);
}

DomainEventDispatcherContext AutomationJobHandler::CreateDispatcherContext()
{
  DomainEventDispatcherContext result;

  result.process_job_state_changed = [this](const auto &event) { OnJobStateChanged(event); };
  result.process_instruction_state_updated = [this](const auto &event)
  { OnInstructionStateUpdated(event); };
  result.process_variable_updated = [this](const auto &event) { OnVariableUpdated(event); };

  return result;
}

std::function<void(const domain_event_t &)> AutomationJobHandler::CreatePostEventCallback() const
{
  return [this](const domain_event_t &event) { m_event_queue->PushEvent(event); };
}

std::function<domain_event_t()> AutomationJobHandler::CreateGetEventCallback() const
{
  return [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
}

void AutomationJobHandler::OnInstructionStateUpdated(const InstructionStateUpdatedEvent &event)
{
  if (auto *item = m_builder->GetInstruction(event.index); item)
  {
    item->SetStatus(::sup::sequencer::StatusToString(event.state.m_execution_status));
  }
  else
  {
    qWarning() << "Error in ProcedureReporter: can't find domain instruction counterpart";
  }
}

void AutomationJobHandler::OnJobStateChanged(const JobStateChangedEvent &event)
{
  m_job_item->SetStatus(::sup::sequencer::ToString(event.status));
}

void AutomationJobHandler::OnVariableUpdated(const VariableUpdatedEvent &event)
{
  if (auto item = m_builder->GetVariable(event.index); item)
  {
    if (event.connected && sup::dto::IsEmptyValue(event.value) && !item->IsAvailable())
    {
      item->SetIsAvailable(event.connected);
      return;
    }

    item->SetIsAvailable(event.connected);
    UpdateAnyValue(event.value, *item);
  }
  else
  {
    std::cout << "Can't find variable \n";
  }
}

}  // namespace sequencergui
