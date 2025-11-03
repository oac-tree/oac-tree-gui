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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_status.h"

#include <oac_tree_gui/core/exceptions.h>

#include <algorithm>
#include <map>

namespace
{

/**
 * @brief Returns correspondance between InstructionStatus and string description.
 */
std::map<oac_tree_gui::InstructionStatus, std::string> CreateRunnerStatusMap()
{
  const std::map<oac_tree_gui::InstructionStatus, std::string> result = {
      {oac_tree_gui::InstructionStatus::kNotStarted, "Not started"},
      {oac_tree_gui::InstructionStatus::kNotFinished, "Not finished"},
      {oac_tree_gui::InstructionStatus::kRunning, "Running"},
      {oac_tree_gui::InstructionStatus::kSuccess, "Success"},
      {oac_tree_gui::InstructionStatus::kFailure, "Failure"},
      {oac_tree_gui::InstructionStatus::kUndefined, ""}};
  return result;
}

}  // namespace

namespace oac_tree_gui
{
std::string ToString(InstructionStatus status)
{
  static const auto kRunnerStatusMap = CreateRunnerStatusMap();
  auto iter = kRunnerStatusMap.find(status);
  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Unknown runner status");
  }
  return iter->second;
}

InstructionStatus GetInstructionStatus(const std::string& status_name)
{
  static const auto kRunnerStatusMap = CreateRunnerStatusMap();
  auto iter =
      std::find_if(kRunnerStatusMap.begin(), kRunnerStatusMap.end(),
                   [status_name](const auto& element) { return element.second == status_name; });

  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Can't convert name to InstructionStatus");
  }
  return iter->first;
}

}  // namespace oac_tree_gui
