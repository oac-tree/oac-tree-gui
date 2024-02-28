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

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/job_controller.h>

namespace sequencergui::experimental
{

DomainRunner::DomainRunner(DomainEventDispatcherContext context, procedure_t* procedure)
{
  if (!procedure)
  {
    throw RuntimeException("Procedure is not initialized");
  }

  m_event_queue = std::make_unique<DomainEventQueue>();
  context.get_event = [this]() -> domain_event_t { return m_event_queue->PopEvent(); };
  m_event_dispatcher = std::make_unique<DomainEventDispatcher>(context);

  // connecting event queue with event dispatcher using queued connection
  QObject::connect(m_event_queue.get(), &DomainEventQueue::NewEvent, m_event_dispatcher.get(),
                   &DomainEventDispatcher::OnNewEvent, Qt::QueuedConnection);

  // setting up observers to post events in a queue
  auto post_event = [this](const domain_event_t& event) { m_event_queue->PushEvent(event); };
  m_job_observer = std::make_unique<DomainJobObserver>(post_event);
  m_procedure_observer = std::make_unique<DomainProcedureObserver>(post_event);

  // creating job controller
  m_job_controller = std::make_unique<sup::sequencer::JobController>(
      *procedure, *m_procedure_observer, *m_job_observer);
}

DomainRunner::~DomainRunner() = default;

bool DomainRunner::Start()
{
  m_runner_state = kStartPressed;

  m_job_controller->Start();

  return true;
}

bool DomainRunner::Stop()
{
  m_runner_state = kStopPressed;

  m_job_controller->Halt();

  return true;
}

bool DomainRunner::Pause()
{
  m_runner_state = kPausePressed;

  m_job_controller->Pause();

  return true;
}

bool DomainRunner::Step()
{
  m_runner_state = kStepPressed;

  m_job_controller->Step();

  return true;
}

sup::sequencer::JobState DomainRunner::WaitForFinished() const
{
  return m_job_observer->WaitForFinished();
}

bool DomainRunner::IsFinished() const
{
  return sup::sequencer::IsFinishedJobState(m_job_observer->GetCurrentState());
}

}  // namespace sequencergui::experimental
