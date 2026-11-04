/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "instruction_behavior.h"

#include <oac_tree_gui/core/exceptions.h>

#include <algorithm>
#include <map>

namespace
{

/**
 * @brief Returns correspondance between InstructionBehavior and string description.
 */
std::map<oac_tree_gui::InstructionBehavior, std::string> CreateBehaviorMap()
{
  const std::map<oac_tree_gui::InstructionBehavior, std::string> result = {
      {oac_tree_gui::InstructionBehavior::kNative, "Native"},
      {oac_tree_gui::InstructionBehavior::kHidden, "Hidden"},
      {oac_tree_gui::InstructionBehavior::kSucceed, "Succeed"},
      {oac_tree_gui::InstructionBehavior::kFail, "Fail"}};
  return result;
}

}  // namespace

namespace oac_tree_gui
{
std::string ToString(InstructionBehavior behavior)
{
  static const auto kBehaviorMap = CreateBehaviorMap();
  auto iter = kBehaviorMap.find(behavior);
  if (iter == kBehaviorMap.end())
  {
    throw RuntimeException("Unknown instruction behavior");
  }
  return iter->second;
}

InstructionBehavior GetInstructionBehavior(const std::string& behavior_name)
{
  static const auto kBehaviorMap = CreateBehaviorMap();
  auto iter =
      std::find_if(kBehaviorMap.begin(), kBehaviorMap.end(), [behavior_name](const auto& element)
                   { return element.second == behavior_name; });

  if (iter == kBehaviorMap.end())
  {
    throw RuntimeException("Can't convert name [" + behavior_name + "] to InstructionBehavior");
  }
  return iter->first;
}

}  // namespace oac_tree_gui
