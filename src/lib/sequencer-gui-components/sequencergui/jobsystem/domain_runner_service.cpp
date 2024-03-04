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

#include "domain_runner_service.h"

#include <sequencergui/experimental/domain_event_dispatcher.h>
#include <sequencergui/experimental/domain_event_queue.h>
#include <sequencergui/experimental/domain_runner.h>

namespace sequencergui
{

DomainRunnerService::DomainRunnerService(procedure_t& procedure)
{
  SetupDomainRunner(procedure);
}

DomainRunnerService::~DomainRunnerService() = default;

void DomainRunnerService::SetupDomainRunner(procedure_t& procedure)
{
  m_event_queue = std::make_unique<DomainEventQueue>();
  m_event_dispatcher = std::make_unique<DomainEventDispatcher>(CreateDispatcherContext());

  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);

  auto post_event = [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
  m_domain_runner = std::make_unique<DomainRunner>(post_event, procedure);
}

DomainEventDispatcherContext DomainRunnerService::CreateDispatcherContext()
{
  auto get_event = [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
  auto instruction_status_changed = [this](const InstructionStatusChanged& event)
  { OnInstructionStatusChanged(event); };

  auto job_state_changed = [this](const JobStateChanged& event) { OnJobStateChanged(event); };

  return {get_event, instruction_status_changed, job_state_changed};
}

void DomainRunnerService::OnInstructionStatusChanged(const InstructionStatusChanged& event)
{
  (void)event;
}

void DomainRunnerService::OnJobStateChanged(const JobStateChanged& event)
{
  (void)event;
}

}  // namespace sequencergui
