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

#include "domain_event_dispatcher.h"
#include "domain_event_queue.h"
#include "domain_runner.h"

namespace sequencergui
{

DomainRunnerService::DomainRunnerService(DomainEventDispatcherContext dispatcher_context,
                                         const UserContext& user_context, procedure_t& procedure)
    : m_event_queue(std::make_unique<DomainEventQueue>())
    , m_event_dispatcher(std::make_unique<DomainEventDispatcher>(CreateGetEventCallback(),
                                                                 std::move(dispatcher_context)))
{
  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);
  m_domain_runner =
      std::make_unique<DomainRunner>(CreatePostEventCallback(), user_context, procedure);
}

DomainRunnerService::~DomainRunnerService() = default;

sup::sequencer::JobState DomainRunnerService::GetJobState() const
{
  return m_domain_runner->GetJobState();
}

bool DomainRunnerService::Start()
{
  return m_domain_runner->Start();
}

bool DomainRunnerService::Stop()
{
  return m_domain_runner->Stop();
}

bool DomainRunnerService::Pause()
{
  return m_domain_runner->Pause();
}

bool DomainRunnerService::Step()
{
  return m_domain_runner->Step();
}

bool DomainRunnerService::IsFinished() const
{
  return m_domain_runner->IsFinished();
}

bool DomainRunnerService::IsBusy() const
{
  return m_domain_runner->IsBusy();
}

void DomainRunnerService::SetTickTimeout(int msec)
{
  m_domain_runner->SetTickTimeout(msec);
}

void DomainRunnerService::Reset()
{
  m_domain_runner->Reset();
}

void DomainRunnerService::SetBreakpoint(size_t instr_idx)
{
  m_domain_runner->SetBreakpoint(instr_idx);
}

void DomainRunnerService::RemoveBreakpoint(size_t instr_idx)
{
  m_domain_runner->RemoveBreakpoint(instr_idx);
}

sup::sequencer::AsyncRunner* DomainRunnerService::GetAsyncRunner()
{
  return m_domain_runner->GetAsyncRunner();
}

std::function<void(const domain_event_t&)> DomainRunnerService::CreatePostEventCallback() const
{
  return [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
}

std::function<domain_event_t()> DomainRunnerService::CreateGetEventCallback() const
{
  return [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
}

}  // namespace sequencergui
