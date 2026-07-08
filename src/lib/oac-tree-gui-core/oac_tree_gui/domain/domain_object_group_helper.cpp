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

#include "domain_object_group_helper.h"

#include "domain_constants.h"

#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/variable_registry.h>
#include <sup/platform/library_names.h>

#include <algorithm>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Creates a map of plugin names to their group names.
 *
 */
std::map<std::string, std::string> CreatePluginNameToGroupNameMap()
{
  std::map<std::string, std::string> result = {
      {domainconstants::kCorePluginName, kCoreGroup},
      {domainconstants::kMathExprPluginName, kMathGroup},
      {domainconstants::kEpicsCAPluginName, kEPICSGroup},
      {domainconstants::kEpicsPVXSPluginName, kEPICSGroup},
      {domainconstants::kControlPluginName, kControlGroup},
      {domainconstants::kSupConfigPluginName, kSupGroup},
      {domainconstants::kSupTimingPluginName, kSupGroup},
  };
  // kSystemPluginName, kStringPluginName and kSupPulseCounterPluginName will go to Misc group

  // list of plugins that have legacy names
  const std::vector<std::string> kHasLegacyVersion = {
      domainconstants::kMathExprPluginName,  domainconstants::kEpicsCAPluginName,
      domainconstants::kEpicsPVXSPluginName, domainconstants::kControlPluginName,
      domainconstants::kSupConfigPluginName, domainconstants::kSupTimingPluginName,
  };

  // extending map with legacy names
  for (const auto& plugin_name : kHasLegacyVersion)
  {
    const auto legacy_name = mvvm::utils::ReplaceSubString(plugin_name, "oac-tree", "sequencer");
    result[legacy_name] = result[plugin_name];
  }
  return result;
}

}  // namespace

std::vector<std::string> GetDefaultPluginNameGroups()
{
  return {
      kCoreGroup, kMathGroup, kEPICSGroup, kControlGroup, kSupGroup, kMiscGroup,
  };
}

std::string GetGroupNameFromPluginName(const std::string& plugin_name)
{
  static const auto kGroupMap = CreatePluginNameToGroupNameMap();

  const auto [path, stripped_basename] = sup::platform::SplitDynamicLibFilename(plugin_name);

  auto iter = kGroupMap.find(stripped_basename);
  if (iter != kGroupMap.end())
  {
    return iter->second;
  }

  // kSystemPluginName, kStringPluginName and kSupPulseCounterPluginName and unknown plugins
  return kMiscGroup;
}

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups()
{
  auto loaded_plugin_names = GetAllPluginNames(sup::oac_tree::GlobalInstructionRegistry());

  // add core "library" to loaded plugins
  (void)loaded_plugin_names.insert(domainconstants::kCorePluginName);

  std::map<std::string, std::vector<std::string>> group_to_instructions;
  for (const auto& plugin_name : loaded_plugin_names)
  {
    const auto group_name = GetGroupNameFromPluginName(plugin_name);
    auto& instructions = group_to_instructions[group_name];
    auto instruction_names = sup::oac_tree::GetInstructionsFromPlugin(
        sup::oac_tree::GlobalInstructionRegistry(), plugin_name);
    (void)std::copy(instruction_names.begin(), instruction_names.end(),
                    std::back_inserter(instructions));
  }

  // packing result according to default group order
  std::vector<ObjectGroupInfo> result;
  for (const auto& group_name : GetDefaultPluginNameGroups())
  {
    ObjectGroupInfo info;
    info.group_name = group_name;
    info.object_names = group_to_instructions[group_name];
    result.push_back(info);
  }

  return result;
}

std::string GetPluginNameForVariable(const std::string& domain_type)
{
  auto& variable_registry = sup::oac_tree::GlobalVariableRegistry();
  const auto domain_plugin_name =
      sup::oac_tree::GetPluginForVariable(variable_registry, domain_type);

  const auto [path, stripped_basename] = sup::platform::SplitDynamicLibFilename(domain_plugin_name);
  return stripped_basename;
}

std::string GetPluginNameForInstruction(const std::string& domain_type)
{
  auto& instruction_registry = sup::oac_tree::GlobalInstructionRegistry();
  const auto domain_plugin_name =
      sup::oac_tree::GetPluginForInstruction(instruction_registry, domain_type);

  const auto [path, stripped_basename] = sup::platform::SplitDynamicLibFilename(domain_plugin_name);
  return stripped_basename;
}

std::string GetPluginNameFromDomainTypeName(const std::string& object_name)
{
  std::string domain_plugin_name = GetPluginNameForVariable(object_name);
  if (domain_plugin_name.empty())
  {
    domain_plugin_name = GetPluginNameForInstruction(object_name);
  }
  return domain_plugin_name;
}

}  // namespace oac_tree_gui
