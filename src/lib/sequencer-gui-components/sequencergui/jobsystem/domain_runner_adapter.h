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
#include <future>
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

  void SetTickTimeout(int msec);

  bool IsBusy() const;

  void StartRequest() override;

  void PauseModeOnRequest() override;

  void PauseModeOffRequest() override;

  void StepRequest() override;

  void StopRequest() override;

  void OnStatusChange(RunnerStatus status) override;

  bool Step() override;

private:
  void RunProcedure(bool in_step_mode);

  /**
   * @brief Update status of the runner adapter when domain runner has finished his job.
   */
  void UpdateStatusOnRunnerCompletion();

  //! Domain runner for procedure.
  std::unique_ptr<runner_t> m_domain_runner;

  //! Procedure to execute in a thread. Must be after the Setup call.
  procedure_t* m_procedure{nullptr};

  //! Sequencer user interface.
  userinterface_t* m_userinterface{nullptr};

  //! Delay in event loop.
  std::atomic<int> m_tick_timeout_ms{0};

  std::function<void(RunnerStatus)> m_status_changed_callback;

  std::future<void> m_future_result;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
