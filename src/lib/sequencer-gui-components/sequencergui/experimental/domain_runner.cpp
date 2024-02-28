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

#include "domain_runner.h"

#include "domain_event_dispatcher.h"
#include "domain_event_queue.h"
#include "domain_job_observer.h"
#include "domain_procedure_observer.h"

namespace sequencergui::experimental
{

DomainRunner::DomainRunner(DomainEventDispatcherContext context)
{
  m_event_queue = std::make_unique<DomainEventQueue>();

  context.get_event = [this]() -> domain_event_t { return m_event_queue->PopEvent(); };

  m_event_dispatcher = std::make_unique<DomainEventDispatcher>(context);

  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent);

  auto post_event = [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
  m_job_observer = std::make_unique<DomainJobObserver>(post_event);

  m_procedure_observer = std::make_unique<DomainProcedureObserver>(post_event);
}

DomainRunner::~DomainRunner() = default;

bool DomainRunner::Start()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStartPressed;
  return true;
}

bool DomainRunner::Stop()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStopPressed;
  return true;
}

bool DomainRunner::Pause()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kPausePressed;
  return true;
}

bool DomainRunner::Step()
{
  if (!IsReady())
  {
    return false;
  }

  m_runner_state = kStepPressed;
  return true;
}

DomainRunner::RunnerState DomainRunner::GetRunnerState() const
{
  return m_runner_state;
}

bool DomainRunner::IsReady()
{
  return m_runner_state == kReady;
}

}  // namespace sequencergui::experimental
