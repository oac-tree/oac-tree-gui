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

#include <sup/sequencer/i_job.h>

namespace sequencergui
{

AbstractDomainRunner::AbstractDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                           UserContext user_context,
                                           std::unique_ptr<sup::sequencer::IJob> job)
    : m_job_service(std::make_unique<DomainJobService>(std::move(dispatcher_context),
                                                       std::move(user_context)))
    , m_job(std::move(job))
{
}

AbstractDomainRunner::~AbstractDomainRunner() = default;

bool AbstractDomainRunner::Start()
{
  m_job->Start();

  return true;
}

bool AbstractDomainRunner::Stop()
{
  m_job->Halt();

  return true;
}

bool AbstractDomainRunner::Pause()
{
  m_job->Pause();

  return true;
}

bool AbstractDomainRunner::Step()
{
  m_job->Step();

  return true;
}

void AbstractDomainRunner::Reset()
{
  m_job->Reset();
}

void AbstractDomainRunner::SetBreakpoint(size_t instr_idx)
{
  m_job->SetBreakpoint(instr_idx);
}

void AbstractDomainRunner::RemoveBreakpoint(size_t instr_idx)
{
  m_job->RemoveBreakpoint(instr_idx);
}

sup::sequencer::IJobInfoIO *AbstractDomainRunner::GetJobInfoIO()
{
  return m_job_service->GetJobInfoIO();
}

}  // namespace sequencergui
