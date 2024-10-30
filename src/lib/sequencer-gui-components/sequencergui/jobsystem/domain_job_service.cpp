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

#include "domain_job_service.h"

#include "domain_event_dispatcher.h"
#include "domain_event_queue.h"
#include "domain_job_observer.h"
#include "user_context.h"

namespace sequencergui
{

DomainJobService::DomainJobService(DomainEventDispatcherContext dispatcher_context,
                                   UserContext user_context)
    : m_event_queue(std::make_unique<DomainEventQueue>())
    , m_event_dispatcher(std::make_unique<DomainEventDispatcher>(CreateGetEventCallback(),
                                                                 std::move(dispatcher_context)))
    , m_job_observer(
          std::make_unique<DomainJobObserver>(CreatePostEventCallback(), std::move(user_context)))
{
  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);
}

DomainJobService::~DomainJobService() = default;

sup::sequencer::IJobInfoIO *DomainJobService::GetJobInfoIO()
{
  return m_job_observer.get();
}

sup::sequencer::JobState DomainJobService::GetJobState() const
{
  return m_job_observer->GetCurrentState();
}

sup::sequencer::JobState DomainJobService::WaitForFinished() const
{
  return m_job_observer->WaitForFinished();
}

bool DomainJobService::WaitForState(sup::sequencer::JobState state, double msec) const
{
  return m_job_observer->WaitForState(state, msec);
}

void DomainJobService::SetTickTimeout(int msec)
{
  m_job_observer->SetTickTimeout(msec);
}

std::function<void(const domain_event_t &)> DomainJobService::CreatePostEventCallback() const
{
  return [this](const domain_event_t &event) { m_event_queue->PushEvent(event); };
}

std::function<domain_event_t()> DomainJobService::CreateGetEventCallback() const
{
  return [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
}

}  // namespace sequencergui
