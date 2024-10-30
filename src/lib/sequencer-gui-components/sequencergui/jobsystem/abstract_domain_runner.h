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
class DomainEventDispatcherContext;
class UserContext;

/**
 * @brief The AbstractDomainRunner is a base class to run domain local and remote jobs.
 */
class AbstractDomainRunner
{
public:
  explicit AbstractDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                UserContext user_context,
                                std::unique_ptr<sup::sequencer::IJob> job);

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

private:
  std::unique_ptr<DomainJobService> m_job_service;
  std::unique_ptr<sup::sequencer::IJob> m_job;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACT_DOMAIN_RUNNER_H_
