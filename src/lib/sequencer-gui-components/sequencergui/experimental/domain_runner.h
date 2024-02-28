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

#ifndef SEQUENCERGUI_EXPERIMENTAL_DOMAIN_RUNNER_H_
#define SEQUENCERGUI_EXPERIMENTAL_DOMAIN_RUNNER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/sequencer/job_states.h>

#include <memory>

namespace sup::sequencer
{
class JobController;
}

namespace sequencergui::experimental
{

class DomainEventQueue;
class DomainEventDispatcher;
class DomainJobObserver;
class DomainProcedureObserver;
class DomainEventDispatcherContext;

class DomainRunner
{
public:
  enum RunnerState
  {
    kReady,
    kStartPressed,
    kStopPressed,
    kPausePressed,
    kStepPressed
  };

  explicit DomainRunner(DomainEventDispatcherContext context, procedure_t* procedure);
  ~DomainRunner();

  bool Start();

  bool Stop();

  bool Pause();

  bool Step();

  sup::sequencer::JobState WaitForFinished() const;

  bool IsFinished() const;

private:
  std::unique_ptr<DomainEventQueue> m_event_queue;
  std::unique_ptr<DomainEventDispatcher> m_event_dispatcher;
  std::unique_ptr<DomainJobObserver> m_job_observer;
  std::unique_ptr<DomainProcedureObserver> m_procedure_observer;
  std::unique_ptr<sup::sequencer::JobController> m_job_controller;

  RunnerState m_runner_state{kReady};
};

}  // namespace sequencergui::experimental

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
