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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H

#include "sequencergui/jobsystem/runner_interface.h"
#include "sequencergui/monitor/job_types.h"

#include <memory>
#include <mutex>

namespace sequencergui
{

class JobStateInterface;

//! Abstract job to execute the arbitrary task via RunnerInterface.
//! The main responsibility of a class is to process start/stop/pause/step requests in correct
//! order.

class AbstractJob : public RunnerInterface
{
public:
  AbstractJob();
  ~AbstractJob() override;

  bool Start() override;

  bool Stop() override;

  bool Pause() override;

  bool Step() override;

  RunnerStatus GetStatus() const;

  void SetStatus(RunnerStatus status);

private:
  //! Submits jobs for a execution.
  virtual void StartRequest() {}

  //! Request for pause mode.
  virtual void PauseModeOnRequest() {}

  //! Request for mode "proceed without waiting".
  virtual void PauseModeOffRequest() {}

  //! Request for single step.
  virtual void StepRequest() {}

  //! Request for job stop without waiting for its natural completion.
  virtual void StopRequest() {}

  //! Will be called on status change.
  virtual void OnStatusChange(RunnerStatus status) {}

  RunnerStatus m_status{RunnerStatus::kIdle};
  mutable std::mutex m_mutex;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
