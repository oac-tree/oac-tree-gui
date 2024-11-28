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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/sequencer/job_states.h>

#include <memory>

namespace sequencergui
{

class DomainJobService;
struct DomainEventDispatcherContext;
struct UserContext;

/**
 * @brief The AbstractDomainRunner is a base class to run domain local and remote jobs.
 *
 * It holds JobInfoIO and all the machinery to handle domain events and user input.
 */
class AbstractDomainRunner
{
public:
  explicit AbstractDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                UserContext user_context);

  virtual ~AbstractDomainRunner();

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

  /**
   * @brief Returns sequencer JobInfo interface.
   */
  sup::sequencer::IJobInfoIO* GetJobInfoIO();

  /**
   * @brief Returns last reported job state.
   */
  sup::sequencer::JobState GetJobState() const;

  /**
   * @brief Waits for job to finish.
   */
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
   * @brief Returns number of events in a queue.
   *
   * This number should be zero, when all domain events have been processed by the GUI.
   */
  size_t GetEventCount() const;

  /**
   * @brief Returns sequencer job info.
   */
  const sup::sequencer::JobInfo& GetJobInfo() const;

  /**
   * @brief Sets sequencer Job to execute.
   */
  void SetDomainJob(std::unique_ptr<sup::sequencer::IJob> job);

private:
  void ValidateJob() const;

  std::unique_ptr<DomainJobService> m_domain_job_service;
  std::unique_ptr<sup::sequencer::IJob> m_domain_job;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_
