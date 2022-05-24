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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H
#define SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H

//! @file jobstates.h
//! Collection of

#include "sequencergui/monitor/job_types.h"

#include <memory>

namespace sequencergui
{

class AbstractJob;

//! Interface for all states of AbstractJob.

class JobStateInterface
{
public:
  virtual ~JobStateInterface();

  virtual RunnerStatus GetStatus() = 0;

  virtual std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) = 0;

  // The methods below are to cheat the friendship with AbstractJob, and let all derived
  // access AbstractJob as JobStateInterface.

  static void Start(AbstractJob *job);
  static void Pause(AbstractJob *job);
  static void Release(AbstractJob *job);
  static void Step(AbstractJob *job);
  static void Stop(AbstractJob *job);
};

//! IdleState represent a state when job just created.

class IdleState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! RunningState represent a state when job is running.

class RunningState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! PausedState represent a state when job is paused.

class PausedState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! StoppingState represent a state when job entered to stopping process.

class StoppingState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! StoppedState represent a state when job was stopped by the user.

class StoppedState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! CompletedState represent a state when job is completed.

class CompletedState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

//! FailedState represent a state when job failed during execution (throw in a runner).

class FailedState : public JobStateInterface
{
public:
  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H
