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

#include "abstract_domain_runner.h"

#include "domain_event_dispatcher_context.h"
#include "domain_job_observer.h"
#include "domain_job_service.h"
#include "user_context.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/i_job.h>
#include <sup/sequencer/job_states.h>

#include <set>

namespace sequencergui
{

AbstractDomainRunner::AbstractDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                           UserContext user_context)
    : m_job_service(std::make_unique<DomainJobService>(std::move(dispatcher_context),
                                                       std::move(user_context)))
{
}

AbstractDomainRunner::~AbstractDomainRunner() = default;

bool AbstractDomainRunner::Start()
{
  ValidateJob();
  m_job->Start();

  return true;
}

bool AbstractDomainRunner::Stop()
{
  ValidateJob();
  m_job->Halt();

  return true;
}

bool AbstractDomainRunner::Pause()
{
  ValidateJob();
  m_job->Pause();

  return true;
}

bool AbstractDomainRunner::Step()
{
  ValidateJob();
  m_job->Step();

  return true;
}

void AbstractDomainRunner::Reset()
{
  ValidateJob();
  m_job->Reset();
}

void AbstractDomainRunner::SetBreakpoint(size_t instr_idx)
{
  ValidateJob();
  m_job->SetBreakpoint(instr_idx);
}

void AbstractDomainRunner::RemoveBreakpoint(size_t instr_idx)
{
  ValidateJob();
  m_job->RemoveBreakpoint(instr_idx);
}

sup::sequencer::IJobInfoIO *AbstractDomainRunner::GetJobInfoIO()
{
  return m_job_service->GetJobInfoIO();
}

sup::sequencer::JobState AbstractDomainRunner::GetJobState() const
{
  return m_job_service->GetJobState();
}

sup::sequencer::JobState AbstractDomainRunner::WaitForFinished() const
{
  return m_job_service->WaitForFinished();
}

bool AbstractDomainRunner::WaitForState(sup::sequencer::JobState state, double msec) const
{
  return m_job_service->WaitForState(state, msec);
}

bool AbstractDomainRunner::IsFinished() const
{
  return sup::sequencer::IsFinishedJobState(m_job_service->GetJobState());
}

bool AbstractDomainRunner::IsBusy() const
{
  using sup::sequencer::JobState;
  static const std::set<JobState> busy_states = {JobState::kPaused, JobState::kStepping,
                                                 JobState::kRunning};
  return busy_states.find(GetJobState()) != busy_states.end();
}

void AbstractDomainRunner::SetTickTimeout(int msec)
{
  m_job_service->SetTickTimeout(msec);
}

void AbstractDomainRunner::SetJob(std::unique_ptr<sup::sequencer::IJob> job)
{
  m_job = std::move(job);
}

void AbstractDomainRunner::ValidateJob()
{
  if (!m_job)
  {
    throw RuntimeException("Job is not initialized");
  }
}

}  // namespace sequencergui
