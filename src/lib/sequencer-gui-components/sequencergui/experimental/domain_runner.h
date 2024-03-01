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
#include <sequencergui/experimental/domain_events.h>

#include <sup/sequencer/job_states.h>

#include <functional>
#include <memory>

namespace sup::sequencer
{
class JobController;
}

namespace sequencergui
{

class DomainJobObserver;
class DomainProcedureObserver;

class DomainRunner
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  enum RunnerState
  {
    kReady,
    kStartPressed,
    kStopPressed,
    kPausePressed,
    kStepPressed
  };

  explicit DomainRunner(const post_event_callback_t& post_event_callback, procedure_t& procedure);
  ~DomainRunner();

  sup::sequencer::JobState GetCurrentState() const;

  bool Start();

  bool Stop();

  bool Pause();

  bool Step();

  sup::sequencer::JobState WaitForFinished() const;

  bool IsFinished() const;

  void SetTickTimeout(int msec);

private:
  std::unique_ptr<DomainJobObserver> m_job_observer;
  std::unique_ptr<DomainProcedureObserver> m_procedure_observer;
  std::unique_ptr<sup::sequencer::JobController> m_job_controller;

  RunnerState m_runner_state{kReady};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
