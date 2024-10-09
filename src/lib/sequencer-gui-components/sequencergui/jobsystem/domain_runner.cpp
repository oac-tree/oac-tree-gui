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

#include "domain_job_observer.h"
#include "domain_procedure_observer.h"
#include "user_context.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/async_runner.h>

#include <set>

namespace sequencergui
{

DomainRunner::DomainRunner(const post_event_callback_t& post_event_callback,
                           const UserContext& user_context, procedure_t& procedure)
    : m_job_observer(std::make_unique<DomainJobObserver>(post_event_callback))
    , m_procedure_observer(
          std::make_unique<DomainProcedureObserver>(post_event_callback, user_context))
    , m_async_runner(std::make_unique<sup::sequencer::AsyncRunner>(
          procedure, *m_procedure_observer, *m_job_observer))
{
}

sup::sequencer::JobState DomainRunner::GetJobState() const
{
  return m_job_observer->GetCurrentState();
}

DomainRunner::~DomainRunner() = default;

bool DomainRunner::Start()
{
  m_runner_state = kStartPressed;

  m_async_runner->Start();

  return true;
}

bool DomainRunner::Stop()
{
  m_runner_state = kStopPressed;

  m_async_runner->Halt();

  return true;
}

bool DomainRunner::Pause()
{
  m_runner_state = kPausePressed;

  m_async_runner->Pause();

  return true;
}

bool DomainRunner::Step()
{
  m_runner_state = kStepPressed;

  m_async_runner->Step();

  return true;
}

sup::sequencer::JobState DomainRunner::WaitForFinished() const
{
  return m_job_observer->WaitForFinished();
}

bool DomainRunner::WaitForState(sup::sequencer::JobState state, double msec) const
{
  return m_job_observer->WaitForState(state, msec);
}

bool DomainRunner::IsFinished() const
{
  return sup::sequencer::IsFinishedJobState(m_job_observer->GetCurrentState());
}

bool DomainRunner::IsBusy() const
{
  using sup::sequencer::JobState;
  static const std::set<JobState> busy_states = {JobState::kPaused, JobState::kStepping,
                                                 JobState::kRunning};
  return busy_states.find(GetJobState()) != busy_states.end();
}

void DomainRunner::SetTickTimeout(int msec)
{
  m_job_observer->SetTickTimeout(msec);
}

void DomainRunner::Reset()
{
  m_async_runner->Reset();
}

sup::sequencer::AsyncRunner* DomainRunner::GetAsyncRunner()
{
  return m_async_runner.get();
}

}  // namespace sequencergui
