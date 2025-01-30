/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <oac-tree-gui/core/exceptions.h>

#include <algorithm>
#include <map>

namespace
{

const std::map<oac_tree_gui::RunnerStatus, std::string> kRunnerStatusMap = {
    {oac_tree_gui::RunnerStatus::kInitial, "Initial"},
    {oac_tree_gui::RunnerStatus::kPaused, "Paused"},
    {oac_tree_gui::RunnerStatus::kStepping, "Stepping"},
    {oac_tree_gui::RunnerStatus::kRunning, "Running"},
    {oac_tree_gui::RunnerStatus::kSucceeded, "Success"},
    {oac_tree_gui::RunnerStatus::kFailed, "Failure"},
    {oac_tree_gui::RunnerStatus::kHalted, "Halted"},
    {oac_tree_gui::RunnerStatus::kUndefined, ""}};

}  // namespace

namespace oac_tree_gui
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

}  // namespace oac_tree_gui
