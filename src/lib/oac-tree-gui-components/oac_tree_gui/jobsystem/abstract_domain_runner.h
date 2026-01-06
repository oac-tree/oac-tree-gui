/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_
#define OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/oac-tree/job_states.h>
#include <chrono>

#include <memory>

namespace oac_tree_gui
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

  AbstractDomainRunner(const AbstractDomainRunner&) = delete;
  AbstractDomainRunner& operator=(const AbstractDomainRunner&) = delete;
  AbstractDomainRunner(AbstractDomainRunner&&) = delete;
  AbstractDomainRunner& operator=(AbstractDomainRunner&&) = delete;

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
  void SetBreakpoint(std::uint32_t instr_idx);

  /**
   * @brief Removes the breakpoint from the instruction with given index.
   */
  void RemoveBreakpoint(std::uint32_t instr_idx);

  /**
   * @brief Returns sequencer JobInfo interface.
   */
  sup::oac_tree::IJobInfoIO* GetJobInfoIO();

  /**
   * @brief Returns last reported job state.
   */
  sup::oac_tree::JobState GetJobState() const;

  /**
   * @brief Waits for job to finish.
   */
  sup::oac_tree::JobState WaitForFinished() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::oac_tree::JobState state, std::chrono::milliseconds duration) const;

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
  void SetTickTimeout(std::chrono::milliseconds timeout);

  /**
   * @brief Returns number of events in a queue.
   *
   * This number should be zero, when all domain events have been processed by the GUI.
   */
  std::size_t GetEventCount() const;

  /**
   * @brief Returns sequencer job info.
   */
  const sup::oac_tree::JobInfo& GetJobInfo() const;

  /**
   * @brief Sets sequencer Job to execute.
   */
  void SetDomainJob(std::unique_ptr<sup::oac_tree::IJob> job);

private:
  void ValidateJob() const;

  std::unique_ptr<DomainJobService> m_domain_job_service;
  std::unique_ptr<sup::oac_tree::IJob> m_domain_job;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_
