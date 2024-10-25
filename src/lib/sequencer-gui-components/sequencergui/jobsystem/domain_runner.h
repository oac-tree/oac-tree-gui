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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/domain_events.h>

#include <sup/sequencer/job_states.h>

#include <functional>
#include <memory>

namespace sup::sequencer
{
class AsyncRunner;
class LocalJob;
}

namespace sequencergui
{

class DomainJobObserver;
class DomainProcedureObserver;
class DomainProcedureObserverV2;
class UserContext;

/**
 * @brief The DomainRunner class provides the necessary components to run sequencer domain
 * procedure.
 *
 * It owns a job monitor, sequencer observer, and job controller and provides a way to report to the
 * GUI events happening on the domain side.
 */
class DomainRunner
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  DomainRunner(const post_event_callback_t& post_event_callback,
               const UserContext& user_context, std::unique_ptr<procedure_t> procedure);

  ~DomainRunner();

  /**
   * @brief Returns last reported job state.
   */
  sup::sequencer::JobState GetJobState() const;

  /**
   * @brief Starts procedure execution.
   */
  bool Start();

  /**
   * @brief Stops procedure execution.
   */
  bool Stop();

  /**
   * @brief Pauses execution.
   */
  bool Pause();

  /**
   * @brief Make a step after pause.
   */
  bool Step();

  sup::sequencer::JobState WaitForFinished() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::sequencer::JobState state, double msec) const;

  /**
   * @brief Checks if the job is in one of succeeded/failed/halted states.
   */
  bool IsFinished() const;

  /**
   * @brief Checks if the the job is in one of paused/stepping/running states.
   */
  bool IsBusy() const;

  /**
   * @brief Sets sleep time on every tick.
   */
  void SetTickTimeout(int msec);

  /**
   * @brief Resets procedure to initial state.
   */
  void Reset();

  /**
   * @brief Sets the breakpoint on the instruction with given index.
   */
  void SetBreakpoint(size_t instr_idx);

  /**
   * @brief Removes the breakpoint from the instruction with given index.
   */
  void RemoveBreakpoint(size_t instr_idx);

private:
  std::unique_ptr<DomainProcedureObserverV2> m_procedure_observer_v2;
  std::unique_ptr<sup::sequencer::LocalJob> m_local_job;

  std::vector<const sup::sequencer::Instruction*> m_index_to_instruction; // REFACTORING
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_H_
