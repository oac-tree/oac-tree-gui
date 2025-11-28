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

#include "abstract_domain_runner.h"

#include "domain_event_dispatcher_context.h"
#include "domain_event_helper.h"
#include "domain_job_observer.h"
#include "domain_job_service.h"
#include "user_context.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/oac-tree/i_job.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/job_info.h>
#include <sup/oac-tree/job_states.h>

#include <set>

namespace oac_tree_gui
{

AbstractDomainRunner::AbstractDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                           UserContext user_context)
    : m_domain_job_service(std::make_unique<DomainJobService>(std::move(dispatcher_context),
                                                              std::move(user_context)))
{
}

AbstractDomainRunner::~AbstractDomainRunner() = default;

bool AbstractDomainRunner::Start()
{
  ValidateJob();
  m_domain_job->Start();

  return true;
}

bool AbstractDomainRunner::Stop()
{
  ValidateJob();
  m_domain_job->Halt();

  return true;
}

bool AbstractDomainRunner::Pause()
{
  ValidateJob();
  m_domain_job->Pause();

  return true;
}

bool AbstractDomainRunner::Step()
{
  ValidateJob();
  m_domain_job->Step();

  return true;
}

void AbstractDomainRunner::Reset()
{
  ValidateJob();
  m_domain_job->Reset();
}

void AbstractDomainRunner::SetBreakpoint(std::uint32_t instr_idx)
{
  ValidateJob();
  m_domain_job->SetBreakpoint(instr_idx);
}

void AbstractDomainRunner::RemoveBreakpoint(std::uint32_t instr_idx)
{
  ValidateJob();
  m_domain_job->RemoveBreakpoint(instr_idx);
}

sup::oac_tree::IJobInfoIO* AbstractDomainRunner::GetJobInfoIO()
{
  return m_domain_job_service->GetJobInfoIO();
}

sup::oac_tree::JobState AbstractDomainRunner::GetJobState() const
{
  return m_domain_job_service->GetJobState();
}

sup::oac_tree::JobState AbstractDomainRunner::WaitForFinished() const
{
  return m_domain_job_service->WaitForFinished();
}

bool AbstractDomainRunner::WaitForState(sup::oac_tree::JobState state, double msec) const
{
  return m_domain_job_service->WaitForState(state, msec);
}

bool AbstractDomainRunner::IsFinished() const
{
  return sup::oac_tree::IsFinishedJobState(m_domain_job_service->GetJobState());
}

bool AbstractDomainRunner::IsBusy() const
{
  using sup::oac_tree::JobState;
  static const std::set<JobState> busy_states = {JobState::kPaused, JobState::kStepping,
                                                 JobState::kRunning};
  return busy_states.find(GetJobState()) != busy_states.end();
}

void AbstractDomainRunner::SetTickTimeout(int msec)
{
  m_domain_job_service->SetTickTimeout(msec);
}

std::size_t AbstractDomainRunner::GetEventCount() const
{
  return m_domain_job_service->GetEventCount();
}

const sup::oac_tree::JobInfo& AbstractDomainRunner::GetJobInfo() const
{
  ValidateJob();

  return m_domain_job->GetInfo();
}

void AbstractDomainRunner::SetDomainJob(std::unique_ptr<sup::oac_tree::IJob> job)
{
  m_domain_job = std::move(job);
  const auto filter =
      CreateInstructionAncestorFilter(*m_domain_job->GetInfo().GetRootInstructionInfo());
  m_domain_job_service->SetInstructionActiveFilter(filter);
}

void AbstractDomainRunner::ValidateJob() const
{
  if (!m_domain_job)
  {
    throw RuntimeException("Job is not initialized");
  }
}

}  // namespace oac_tree_gui
