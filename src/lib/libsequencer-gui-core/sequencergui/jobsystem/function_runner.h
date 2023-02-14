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

#ifndef SEQUENCERGUI_JOBSYSTEM_FUNCTION_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_FUNCTION_RUNNER_H_

#include <sequencergui/jobsystem/abstract_job.h>
#include <sequencergui/jobsystem/job_types.h>

#include <chrono>
#include <functional>
#include <memory>

namespace sequencergui
{

//! Basic implementation of the function runner.
//! It will execute `worker` function in a loop unless it returns false.
class FunctionRunner : public AbstractJob
{
public:
  explicit FunctionRunner(std::function<bool()> worker,
                          std::function<void(RunnerStatus)> status_changed_callback = {});
  ~FunctionRunner() override;

  bool IsBusy() const;

public:
  void StartRequest() override;
  void PauseModeOnRequest() override;
  void PauseModeOffRequest() override;
  void StepRequest() override;
  void StopRequest() override;
  void OnStatusChange(RunnerStatus status) override;

  struct FunctionRunnerImpl;
  std::unique_ptr<FunctionRunnerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_FUNCTION_RUNNER_H_
