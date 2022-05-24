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

#ifndef SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNER_H
#define SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNER_H

#include "sequencergui/jobsystem/runnerinterface.h"
#include "sequencergui/monitor/job_types.h"

#include <functional>
#include <memory>

namespace sequencergui
{

//! Basic implementation of the function runner.
//! It will execute `worker` function in a loop unless it returns false.
class FunctionRunner : public RunnerInterface
{
public:
  explicit FunctionRunner(std::function<bool()> worker,
                          std::function<void(RunnerStatus)> status_changed_callback = {});
  ~FunctionRunner() override;

  bool Start() override;

  bool Stop() override;

  bool Pause() override;

  bool Step() override;

  bool IsInTransition() const override;

  RunnerStatus GetRunnerStatus() const;

  bool IsBusy() const;

  void SetWaitingMode(WaitingMode waiting_mode);

private:
  struct FunctionRunnerImpl;
  std::unique_ptr<FunctionRunnerImpl> p_impl;
};

//! Will wait a given amount of sec for job completion. Returns `true` is runner has finished before
//! the timeout, `false` otherwise. Internally has a precision of 10 msec.
bool WaitForCompletion(const FunctionRunner& runner, double timeout_sec);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNER_H
