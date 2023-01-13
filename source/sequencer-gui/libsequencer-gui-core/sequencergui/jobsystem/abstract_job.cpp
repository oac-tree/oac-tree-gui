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

#include "sequencergui/jobsystem/abstract_job.h"

#include <iostream>

namespace
{
bool CanStartJob(::sequencergui::RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kIdle
         || current_status == ::sequencergui::RunnerStatus::kFailed
         || current_status == ::sequencergui::RunnerStatus::kStopped
         || current_status == ::sequencergui::RunnerStatus::kCompleted;
}

bool CanPauseJob(::sequencergui::RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kRunning;
}

bool CanReleaseJob(::sequencergui::RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kPaused;
}

bool CanStopJob(::sequencergui::RunnerStatus current_status)
{
  return current_status != ::sequencergui::RunnerStatus::kStopping
         && current_status != ::sequencergui::RunnerStatus::kIdle
         && current_status != ::sequencergui::RunnerStatus::kCompleted
         && current_status != ::sequencergui::RunnerStatus::kStopped;
}

}  // namespace

namespace sequencergui
{

AbstractJob::AbstractJob() = default;

AbstractJob::~AbstractJob() = default;

bool AbstractJob::Start()
{
  bool is_valid_request{false};
  if (CanStartJob(GetStatus()))
  {
    StartRequest();
    is_valid_request = true;
  }
  else if (CanReleaseJob(GetStatus()))
  {
    PauseModeOffRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJob::Stop()
{
  std::cout << " AbstractJob::Stop() 1.1 " << static_cast<int>(GetStatus()) << std::endl;
  bool is_valid_request{false};
  if (CanStopJob(GetStatus()))
  {
    std::cout << " AbstractJob::Stop() 1.2 " << static_cast<int>(GetStatus()) << std::endl;
    StopRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJob::Pause()
{
  bool is_valid_request{false};
  if (CanPauseJob(GetStatus()))
  {
    PauseModeOnRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJob::Step()
{
  bool is_valid_request{false};
  if (CanReleaseJob(GetStatus()))
  {
    StepRequest();
    is_valid_request = true;
  }
  else if (CanStartJob(GetStatus()))
  {
    PauseModeOnRequest();
    StartRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

RunnerStatus AbstractJob::GetStatus() const
{
  std::lock_guard lock(m_mutex);
  return m_status;
}

void AbstractJob::SetStatus(RunnerStatus status)
{
  std::lock_guard lock(m_mutex);
  m_status = status;
  OnStatusChange(m_status);
}

}  // namespace sequencergui
