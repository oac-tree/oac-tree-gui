/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "domain_job_service.h"

#include "domain_job_observer.h"
#include "user_context.h"

#include <oac_tree_gui/jobsystem/objects/domain_event_dispatcher.h>
#include <oac_tree_gui/jobsystem/objects/domain_event_queue.h>

namespace oac_tree_gui
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
  (void)QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                         &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);
}

DomainJobService::~DomainJobService() = default;

sup::oac_tree::IJobInfoIO* DomainJobService::GetJobInfoIO()
{
  return m_job_observer.get();
}

sup::oac_tree::JobState DomainJobService::GetJobState() const
{
  return m_job_observer->GetCurrentState();
}

sup::oac_tree::JobState DomainJobService::WaitForFinished() const
{
  return m_job_observer->WaitForFinished();
}

bool DomainJobService::WaitForState(sup::oac_tree::JobState state,
                                    std::chrono::milliseconds duration) const
{
  return m_job_observer->WaitForState(state, duration);
}

void DomainJobService::SetTickTimeout(std::chrono::milliseconds timeout)
{
  m_job_observer->SetTickTimeout(timeout);
}

std::size_t DomainJobService::GetEventCount() const
{
  return m_event_queue->GetEventCount();
}

void DomainJobService::SetInstructionActiveFilter(const active_filter_t& filter)
{
  m_job_observer->SetInstructionActiveFilter(filter);
}

std::function<void(const domain_event_t&)> DomainJobService::CreatePostEventCallback() const
{
  return [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
}

std::function<domain_event_t()> DomainJobService::CreateGetEventCallback() const
{
  return [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
}

}  // namespace oac_tree_gui
