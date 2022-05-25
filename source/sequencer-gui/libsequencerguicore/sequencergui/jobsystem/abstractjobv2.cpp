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
  return current_status != ::sequencergui::RunnerStatus::kStopping;
}


}  // namespace

namespace sequencergui
{

AbstractJobV2::AbstractJobV2() {}

AbstractJobV2::~AbstractJobV2() {}

RunnerStatus AbstractJobV2::GetStatus() const
{
  return m_status;
}

bool AbstractJobV2::PerformAction(JobAction action)
{
  switch (action)
  {
  case JobAction::kStart:
  {
    if (CanStartJob(m_status))
    {
      StartRequest();
    }
    else if (CanReleaseJob(m_status))
    {
      ReleaseRequest();
    }
    break;
  }
  case JobAction::kPause:
  {
    if (CanPauseJob(m_status))
    {
      PauseRequest();
    }
    break;
  }
  case JobAction::kStep:
  {
    if (CanReleaseJob(m_status))
    {
      ReleaseRequest();
    }
    break;
  }
  case JobAction::kStop:
  {
    if (CanStopJob(m_status))
    {
      StopRequest();
    }
    break;
  }
  default:
  {
    break;
  }
  }

  return {};
}

void AbstractJobV2::SetStatus(RunnerStatus state) {}

}  // namespace sequencergui
