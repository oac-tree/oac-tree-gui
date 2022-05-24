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

#ifndef SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNERV2_H
#define SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNERV2_H

#include "sequencergui/jobsystem/abstractjob.h"

#include <memory>
#include <functional>

namespace sequencergui
{

class FunctionRunnerV2 : public AbstractJob
{
public:
  explicit FunctionRunnerV2(std::function<bool()> worker,
                          std::function<void(RunnerStatus)> status_changed_callback = {});

  ~FunctionRunnerV2() override;

  bool IsBusy() const;

private:
  void Start() override;
  void Pause() override;
  void Release() override;
  void Step() override;
  void Stop() override;

  struct FunctionRunnerImpl;
  std::unique_ptr<FunctionRunnerImpl> p_impl;
};

//! Will wait a given amount of sec for job completion. Returns `true` is runner has finished before
//! the timeout, `false` otherwise. Internally has a precision of 10 msec.
bool WaitForCompletion(const FunctionRunnerV2& runner, double timeout_sec);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
