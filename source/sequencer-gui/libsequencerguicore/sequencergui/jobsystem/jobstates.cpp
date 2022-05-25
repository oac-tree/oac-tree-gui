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

#include "sequencergui/jobsystem/jobstates.h"

#include "sequencergui/jobsystem/abstractjob.h"

namespace sequencergui
{

// ----------------------------------------------------------------------------
// JobStateInterface
// ----------------------------------------------------------------------------

JobStateInterface::~JobStateInterface() = default;

void JobStateInterface::Start(AbstractJob *job)
{
  job->StartRequest();
}

void JobStateInterface::Pause(AbstractJob *job)
{
  job->PauseRequest();
}

void JobStateInterface::Release(AbstractJob *job)
{
  job->ReleaseRequest();
}

void JobStateInterface::Step(AbstractJob *job)
{
  job->StepRequest();
}

void JobStateInterface::Stop(AbstractJob *job)
{
  job->StopRequest();
}

// ----------------------------------------------------------------------------
// IdleState
// ----------------------------------------------------------------------------

RunnerStatus IdleState::GetStatus()
{
  return RunnerStatus::kIdle;
}

std::unique_ptr<JobStateInterface> IdleState::Handle(JobAction action, AbstractJob *job)
{
  if (action == JobAction::kStart)
  {
    Start(job);
    return std::make_unique<RunningState>();
  }

  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// RunningState
// ----------------------------------------------------------------------------

RunnerStatus RunningState::GetStatus()
{
  return RunnerStatus::kRunning;
}

std::unique_ptr<JobStateInterface> RunningState::Handle(JobAction action, AbstractJob *job)
{
  if (action == JobAction::kPause)
  {
    Pause(job);
    return std::make_unique<PausedState>();
  }

  if (action == JobAction::kStep)
  {
    // FIXME provide switch to step mode here
    Step(job);
    return std::make_unique<PausedState>();
  }

  if (action == JobAction::kStop)
  {
    Stop(job);
    return std::make_unique<StoppedState>();
  }

  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// PausedState
// ----------------------------------------------------------------------------

RunnerStatus PausedState::GetStatus()
{
  return RunnerStatus::kPaused;
}

std::unique_ptr<JobStateInterface> PausedState::Handle(JobAction action, AbstractJob *job)
{
  if (action == JobAction::kStart)
  {
    Start(job);
    return std::make_unique<RunningState>();
  }

  if (action == JobAction::kStep)
  {
    // FIXME provide switch to step mode here
    Step(job);
    return std::make_unique<PausedState>();
  }

  if (action == JobAction::kStop)
  {
    Stop(job);
    return std::make_unique<StoppedState>();
  }

  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// StoppedState
// ----------------------------------------------------------------------------

RunnerStatus StoppingState::GetStatus()
{
  return RunnerStatus::kStopping;
}

std::unique_ptr<JobStateInterface> StoppingState::Handle(JobAction action, AbstractJob *job)
{
  (void)action;
  (void)job;
  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// StoppedState
// ----------------------------------------------------------------------------

RunnerStatus StoppedState::GetStatus()
{
  return RunnerStatus::kStopped;
}

std::unique_ptr<JobStateInterface> StoppedState::Handle(JobAction action, AbstractJob *job)
{
  if (action == JobAction::kStart)
  {
    Start(job);
    return std::make_unique<RunningState>();
  }

  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// CompletedState
// ----------------------------------------------------------------------------

RunnerStatus CompletedState::GetStatus()
{
  return RunnerStatus::kCompleted;
}

std::unique_ptr<JobStateInterface> CompletedState::Handle(JobAction action, AbstractJob *job)
{
  (void)action;
  (void)job;
  return {};  // other actions are ignored
}

// ----------------------------------------------------------------------------
// FailedState
// ----------------------------------------------------------------------------

RunnerStatus FailedState::GetStatus()
{
  return RunnerStatus::kFailed;
}

std::unique_ptr<JobStateInterface> FailedState::Handle(JobAction action, AbstractJob *job)
{
  (void)action;
  (void)job;
  return {};  // other actions are ignored
}

}  // namespace sequencergui
