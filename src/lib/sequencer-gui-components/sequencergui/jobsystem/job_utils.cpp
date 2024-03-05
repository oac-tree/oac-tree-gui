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

#include "job_utils.h"

#include <QString>
#include <algorithm>
#include <map>
#include <numeric>
#include <stdexcept>

namespace
{

// const std::map<sequencergui::RunnerStatus, std::string> kRunnerStatusMap = {
//     {sequencergui::RunnerStatus::kIdle, "Idle"},
//     {sequencergui::RunnerStatus::kRunning, "Running"},
//     {sequencergui::RunnerStatus::kPaused, "Paused"},
//     {sequencergui::RunnerStatus::kCompleted, "Completed"},
//     {sequencergui::RunnerStatus::kPaused, "Paused"},
//     {sequencergui::RunnerStatus::kStopping, "Stopping"},
//     {sequencergui::RunnerStatus::kStopped, "Stopped"},
//     {sequencergui::RunnerStatus::kFailed, "Failed"}};

// exact copy of sup::sequencer::JobState
static const std::map<sequencergui::RunnerStatus, std::string> kRunnerStatusMap = {
    { sequencergui::RunnerStatus::kInitial, "Initial" },
    { sequencergui::RunnerStatus::kPaused, "Paused" },
    { sequencergui::RunnerStatus::kStepping, "Stepping" },
    { sequencergui::RunnerStatus::kRunning, "Running" },
    { sequencergui::RunnerStatus::kSucceeded, "Success" },
    { sequencergui::RunnerStatus::kFailed, "Failure" },
    { sequencergui::RunnerStatus::kHalted, "Halted" }
};

}  // namespace

namespace sequencergui
{

std::string RunnerStatusToString(RunnerStatus status)
{
  auto iter = kRunnerStatusMap.find(status);
  return iter == kRunnerStatusMap.end() ? std::string("Unknown") : iter->second;
}

RunnerStatus GetRunnerStatus(const std::string &status_name)
{
  auto iter = std::find_if(kRunnerStatusMap.begin(), kRunnerStatusMap.end(),
                           [status_name](auto element) { return element.second == status_name; });

  if (iter == kRunnerStatusMap.end())
  {
    throw std::runtime_error("Can't convert name to RunnerStatus");
  }
  return iter->first;
}

int GetDefaultTickTimeoutMsc()
{
  const int kDefaultTimeout{20};
  return kDefaultTimeout;
}

std::string GetRegExpPattern(const std::vector<std::string> &data)
{
  auto fold = [](std::string result, std::string label) { return std::move(result) + '|' + label; };
  return data.empty()
             ? std::string("()")
             : "(" + std::accumulate(std::next(data.begin()), data.end(), data[0], fold) + ")";
}

}  // namespace sequencergui
