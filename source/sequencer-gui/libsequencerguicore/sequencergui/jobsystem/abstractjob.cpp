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

#include "sequencergui/jobsystem/abstractjob.h"

namespace sequencergui
{
AbstractJob::AbstractJob() : m_runner_status(RunnerStatus::kIdle) {}

AbstractJob::~AbstractJob() = default;

RunnerStatus AbstractJob::GetStatus() const
{
  return m_runner_status;
}

bool AbstractJob::PerformAction(JobAction action)
{
  switch (m_runner_status)
  {
  case RunnerStatus::kIdle:
    break;
  case RunnerStatus::kRunning:
    break;
  case RunnerStatus::kPaused:
    break;
  case RunnerStatus::kCompleted:
    break;
  case RunnerStatus::kCanceling:
    break;
  case RunnerStatus::kCanceled:
    break;
  case RunnerStatus::kFailed:
    break;
  default:
    break;
  }

  return false;
}

}  // namespace sequencergui
