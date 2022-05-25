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

#include "sequencergui/jobsystem/abstractjobv2.h"

#include <iostream>

namespace
{
bool CanStartJob(::sequencergui::RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kIdle
         || current_status == ::sequencergui::RunnerStatus::kFailed
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
         && current_status != ::sequencergui::RunnerStatus::kIdle;
}

}  // namespace

namespace sequencergui
{

AbstractJobV2::AbstractJobV2() = default;

AbstractJobV2::~AbstractJobV2() = default;

bool AbstractJobV2::Start()
{
  bool is_valid_request{false};
  if (CanStartJob(m_status))
  {
    StartRequest();
    is_valid_request = true;
  }
  else if (CanReleaseJob(m_status))
  {
    PauseModeOffRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJobV2::Stop()
{
  bool is_valid_request{false};
  if (CanStopJob(m_status))
  {
    StopRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJobV2::Pause()
{
  bool is_valid_request{false};
  if (CanPauseJob(m_status))
  {
    PauseModeOnRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

bool AbstractJobV2::Step()
{
  bool is_valid_request{false};
  if (CanReleaseJob(m_status))
  {
    StepRequest();
    is_valid_request = true;
  }
  else if (CanStartJob(m_status))
  {
    PauseModeOnRequest();
    StartRequest();
    is_valid_request = true;
  }
  return is_valid_request;
}

RunnerStatus AbstractJobV2::GetStatus() const
{
  std::lock_guard lock(m_mutex);
  return m_status;
}

void AbstractJobV2::SetStatus(RunnerStatus status)
{
  std::lock_guard lock(m_mutex);
  std::cout << "status " << static_cast<int>(status) << std::endl;
  m_status = status;
  OnStatusChange(m_status);
}

}  // namespace sequencergui
