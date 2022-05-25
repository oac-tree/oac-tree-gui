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

AbstractJobV2::AbstractJobV2() = default;

AbstractJobV2::~AbstractJobV2() = default;

bool AbstractJobV2::Start()
{
  if (CanStartJob(m_status))
  {
    StartRequest();
  }
  else if (CanReleaseJob(m_status))
  {
    ReleaseRequest();
  }
  return true;
}

bool AbstractJobV2::Stop()
{
  if (CanStopJob(m_status))
  {
    StopRequest();
  }
  return true;
}

bool AbstractJobV2::Pause()
{
  if (CanPauseJob(m_status))
  {
    PauseRequest();
  }
  return true;
}

bool AbstractJobV2::Step()
{
  if (CanReleaseJob(m_status))
  {
    ReleaseRequest();
  }
  else if (CanStartJob(m_status))
  {
    PauseRequest();
    StartRequest();
  }
  return true;
}

RunnerStatus AbstractJobV2::GetStatus() const
{
  return m_status;
}

void AbstractJobV2::SetStatus(RunnerStatus status)
{
  m_status = status;
}

}  // namespace sequencergui
