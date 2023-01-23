/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/abstract_job.h>
#include <sequencergui/jobsystem/job_types.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>

namespace sequencergui
{

class FunctionRunner;

//! Adapter for domain runner to perform stepwise execution in a thread.
//! We call the domain Runner::ExecuteSingle in the event loop provided by FunctionRunner.
//! The Procedure and UserInterface should be alive during the lifetime of the DomainRunnerAdapter.
//!
//! This adapter is intended to work with the domain Procedure after the Setup call. It will throw
//! on attempt to use it again if the procedure has been already successfully completed.

class DomainRunnerAdapter : public AbstractJob
{
public:
  DomainRunnerAdapter(procedure_t* procedure, userinterface_t* interface,
                      std::function<void(RunnerStatus)> status_changed_callback);

  ~DomainRunnerAdapter() override;

  RunnerStatus GetStatus() const override;

  void SetStatus(RunnerStatus status) override;

  void SetTickTimeout(int msec);

  bool IsBusy() const;

  void StartRequest() override;

  void PauseModeOnRequest() override;

  void PauseModeOffRequest() override;

  void StepRequest() override;

  void StopRequest() override;

  void OnStatusChange(RunnerStatus status) override;

private:
  bool ExecuteSingle();

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
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
