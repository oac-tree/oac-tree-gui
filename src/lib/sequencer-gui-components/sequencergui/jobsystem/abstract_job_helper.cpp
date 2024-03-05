/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "abstract_job_helper.h"

namespace sequencergui
{

bool CanStartJob(RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kInitial
         || current_status == ::sequencergui::RunnerStatus::kFailed
         || current_status == ::sequencergui::RunnerStatus::kHalted
         || current_status == ::sequencergui::RunnerStatus::kSucceeded;
}

bool CanPauseJob(RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kRunning;
}

bool CanReleaseJob(RunnerStatus current_status)
{
  return current_status == ::sequencergui::RunnerStatus::kPaused;
}

bool CanStopJob(RunnerStatus current_status)
{
  return current_status != ::sequencergui::RunnerStatus::kInitial
         && current_status != ::sequencergui::RunnerStatus::kSucceeded
         && current_status != ::sequencergui::RunnerStatus::kHalted;
}

}  // namespace sequencergui
