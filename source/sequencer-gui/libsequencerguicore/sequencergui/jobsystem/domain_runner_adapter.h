/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAINRUNNERADAPTER_H
#define SEQUENCERGUI_JOBSYSTEM_DOMAINRUNNERADAPTER_H

#include "sequencergui/jobsystem/runner_interface.h"
#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/monitor/job_types.h"

#include <functional>
#include <memory>
#include <atomic>
#include <chrono>

namespace sequencergui
{

class FunctionRunner;

//! Adapter for domain runner to perform stepwise execution in a thread.
//! We call the domain Runner::ExecuteSingle in the event loop provided by FunctionRunner.
//! The Procedure and UserInterface should be alive during the life-time of the DomainRunnerAdapter.

class DomainRunnerAdapter : public RunnerInterface
{
public:
  DomainRunnerAdapter(procedure_t* procedure, userinterface_t* interface,
                      std::function<void(RunnerStatus)> status_changed_callback);

  ~DomainRunnerAdapter() override;

  bool Start() override;

  bool Stop() override;

  bool Pause() override;

  bool Step() override;

  RunnerStatus GetStatus() const;

  bool WaitForCompletion(std::chrono::milliseconds timeout_msec);

  void SetTickTimeout(int msec);

  bool IsBusy() const;

private:
  bool ExecuteSingle();  
  void ResetIfNecessary();

  //! Domain runner for procedure.
  std::unique_ptr<runner_t> m_domain_runner;

  //! Our own runner introduced to start/stop/pause jobs.
  std::unique_ptr<FunctionRunner> m_function_runner;

  //! Procedure to execute in a thread. Must be after the Setup call.
  procedure_t* m_procedure{nullptr};

  //! Sequencer user interface.
  userinterface_t* m_userinterface{nullptr};

  //! Delay in event loop.
  std::atomic<int> m_tick_timeout_ms{0};

  bool m_was_stopped{false};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
