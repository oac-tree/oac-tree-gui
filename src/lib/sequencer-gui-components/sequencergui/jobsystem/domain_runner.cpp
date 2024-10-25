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
#include "domain_procedure_observer_v2.h"
#include "user_context.h"

#include <sequencergui/automation/remote_job_observer.h>
#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/async_runner.h>
#include <sup/sequencer/instruction_map.h>  // REFACTORING
#include <sup/sequencer/local_job.h>

#include <set>

namespace sequencergui
{

DomainRunner::DomainRunner(const post_event_callback_t& post_event_callback,
                           const UserContext& user_context, procedure_t& procedure)
    : m_job_observer(std::make_unique<DomainJobObserver>(post_event_callback))
    , m_procedure_observer(
          std::make_unique<DomainProcedureObserver>(post_event_callback, user_context))
    , m_async_runner(std::make_unique<sup::sequencer::AsyncRunner>(procedure, *m_procedure_observer,
                                                                   *m_job_observer))
{
  // REFACTORING, procedure should be after Setup already
  sup::sequencer::InstructionMap instruction_map(procedure.RootInstruction());
  m_index_to_instruction = sup::sequencer::GetReverseMap(instruction_map.GetInstructionIndexMap());
}

DomainRunner::DomainRunner(const post_event_callback_t& post_event_callback,
                           const UserContext& user_context, std::unique_ptr<procedure_t> procedure)
    : m_procedure_observer_v2(
          std::make_unique<DomainProcedureObserverV2>(post_event_callback, user_context))
{
  auto procedure_ptr = procedure.get();
  m_local_job =
      std::make_unique<sup::sequencer::LocalJob>(std::move(procedure), *m_procedure_observer_v2);

  // REFACTORING, procedure should be after Setup already
  sup::sequencer::InstructionMap instruction_map(procedure_ptr->RootInstruction());
  m_index_to_instruction = sup::sequencer::GetReverseMap(instruction_map.GetInstructionIndexMap());
}

sup::sequencer::JobState DomainRunner::GetJobState() const
{
  if (m_job_observer)
  {
    return m_job_observer->GetCurrentState();
  }
  else
  {
    return m_procedure_observer_v2->GetCurrentState();
  }
}

DomainRunner::~DomainRunner() = default;

bool DomainRunner::Start()
{
  if (m_async_runner)
  {
    m_async_runner->Start();
  }
  else
  {
    m_local_job->Start();
  }

  return true;
}

bool DomainRunner::Stop()
{
  if (m_async_runner)
  {
    m_async_runner->Halt();
  }
  else
  {
    m_local_job->Halt();
  }

  return true;
}

bool DomainRunner::Pause()
{
  if (m_async_runner)
  {
    m_async_runner->Pause();
  }
  else
  {
    m_local_job->Pause();
  }

  return true;
}

bool DomainRunner::Step()
{
  if (m_async_runner)
  {
    m_async_runner->Step();
  }
  else
  {
    m_local_job->Step();
  }

  return true;
}

sup::sequencer::JobState DomainRunner::WaitForFinished() const
{
  if (m_job_observer)
  {
    return m_job_observer->WaitForFinished();
  }

  return m_procedure_observer_v2->WaitForFinished();
}

bool DomainRunner::WaitForState(sup::sequencer::JobState state, double msec) const
{
  if (m_job_observer)
  {
    return m_job_observer->WaitForState(state, msec);
  }

  return m_procedure_observer_v2->WaitForState(state, msec);
}

bool DomainRunner::IsFinished() const
{
  if (m_job_observer)
  {
    return sup::sequencer::IsFinishedJobState(m_job_observer->GetCurrentState());
  }

  return sup::sequencer::IsFinishedJobState(m_procedure_observer_v2->GetCurrentState());
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
  if (m_job_observer)
  {
    m_job_observer->SetTickTimeout(msec);
  }
  else
  {
    m_procedure_observer_v2->SetTickTimeout(msec);
  }
}

void DomainRunner::Reset()
{
  if (m_async_runner)
  {
    m_async_runner->Reset();
  }
  else
  {
    m_local_job->Reset();
  }
}

void DomainRunner::SetBreakpoint(size_t instr_idx)
{
  if (m_async_runner)
  {
    m_async_runner->SetBreakpoint(m_index_to_instruction[instr_idx]);
  }
  else
  {
    m_local_job->SetBreakpoint(instr_idx);
  }
}

void DomainRunner::RemoveBreakpoint(size_t instr_idx)
{
  if (m_async_runner)
  {
    m_async_runner->RemoveBreakpoint(m_index_to_instruction[instr_idx]);
  }
  else
  {
    m_local_job->RemoveBreakpoint(instr_idx);
  }
}

}  // namespace sequencergui
