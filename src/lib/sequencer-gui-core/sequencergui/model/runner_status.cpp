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

#include "runner_status.h"

#include <sequencergui/core/exceptions.h>

#include <algorithm>
#include <map>

namespace
{

const std::map<sequencergui::RunnerStatus, std::string> kRunnerStatusMap = {
    {sequencergui::RunnerStatus::kInitial, "Initial"},
    {sequencergui::RunnerStatus::kPaused, "Paused"},
    {sequencergui::RunnerStatus::kStepping, "Stepping"},
    {sequencergui::RunnerStatus::kRunning, "Running"},
    {sequencergui::RunnerStatus::kSucceeded, "Success"},
    {sequencergui::RunnerStatus::kFailed, "Failure"},
    {sequencergui::RunnerStatus::kHalted, "Halted"},
    {sequencergui::RunnerStatus::kUndefined, ""}};

}  // namespace

namespace sequencergui
{
std::string ToString(RunnerStatus status)
{
  auto iter = kRunnerStatusMap.find(status);
  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Unknown runner status");
  }
  return iter->second;
}

RunnerStatus GetRunnerStatus(const std::string &status_name)
{
  auto iter = std::find_if(kRunnerStatusMap.begin(), kRunnerStatusMap.end(),
                           [status_name](auto element) { return element.second == status_name; });

  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Can't convert name to RunnerStatus");
  }
  return iter->first;
}

}  // namespace sequencergui
