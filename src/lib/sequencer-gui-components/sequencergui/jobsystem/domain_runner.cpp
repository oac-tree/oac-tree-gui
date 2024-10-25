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

#include <sup/sequencer/instruction_map.h>  // REFACTORING
#include <sup/sequencer/local_job.h>

#include <set>

namespace sequencergui
{

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
  return m_procedure_observer_v2->GetCurrentState();
}

DomainRunner::~DomainRunner() = default;

bool DomainRunner::Start()
{
  m_local_job->Start();

  return true;
}

bool DomainRunner::Stop()
{
  m_local_job->Halt();

  return true;
}

bool DomainRunner::Pause()
{
  m_local_job->Pause();

  return true;
}

bool DomainRunner::Step()
{
  m_local_job->Step();

  return true;
}

sup::sequencer::JobState DomainRunner::WaitForFinished() const
{
  return m_procedure_observer_v2->WaitForFinished();
}

bool DomainRunner::WaitForState(sup::sequencer::JobState state, double msec) const
{
  return m_procedure_observer_v2->WaitForState(state, msec);
}

bool DomainRunner::IsFinished() const
{
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
  m_procedure_observer_v2->SetTickTimeout(msec);
}

void DomainRunner::Reset()
{
  m_local_job->Reset();
}

void DomainRunner::SetBreakpoint(size_t instr_idx)
{
  m_local_job->SetBreakpoint(instr_idx);
}

void DomainRunner::RemoveBreakpoint(size_t instr_idx)
{
  m_local_job->RemoveBreakpoint(instr_idx);
}

}  // namespace sequencergui
