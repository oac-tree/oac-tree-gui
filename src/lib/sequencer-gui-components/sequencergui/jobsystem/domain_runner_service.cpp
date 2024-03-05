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

DomainRunnerService::DomainRunnerService(DomainEventDispatcherContext context,
                                         procedure_t& procedure)
{
  m_event_queue = std::make_unique<DomainEventQueue>();
  auto get_event_callback = [this]() -> domain_event_t { return m_event_queue->PopEvent(); };

  m_event_dispatcher = std::make_unique<DomainEventDispatcher>(get_event_callback, context);

  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);

  auto post_event = [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
  m_domain_runner = std::make_unique<DomainRunner>(post_event, procedure);
}

sup::sequencer::JobState DomainRunnerService::GetCurrentState() const
{
  return m_domain_runner->GetCurrentState();
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

DomainRunnerService::~DomainRunnerService() = default;
}  // namespace sequencergui
