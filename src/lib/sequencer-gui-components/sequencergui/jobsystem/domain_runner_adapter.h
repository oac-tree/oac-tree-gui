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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_

#include <sequencergui/jobsystem/abstract_job.h>
#include <sequencergui/jobsystem/domain_runner_context.h>

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
  explicit DomainRunnerAdapter(const DomainRunnerContext& context);

  ~DomainRunnerAdapter() override;

  void SetTickTimeout(int msec);

  bool IsBusy() const;

  void StartRequest() override;

  void PauseModeOnRequest() override;

  void PauseModeOffRequest() override;

  void StepRequest() override;

  void StopRequest() override;

  runner_t* GetDomainRunner();

protected:
  void OnStatusChange(RunnerStatus status) override;

private:
  void RunProcedure(bool in_step_mode);

  /**
   * @brief Update status of the runner adapter when domain runner has finished his job.
   */
  void UpdateStatusOnRunnerCompletion();

  std::unique_ptr<runner_t> m_domain_runner;  //! domain runner for procedure
  std::atomic<int> m_tick_timeout_ms{0};      //! delay in event loop
  DomainRunnerContext m_context;              //! adapter's input data
  std::future<void> m_future_result;          //! result of async job
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
