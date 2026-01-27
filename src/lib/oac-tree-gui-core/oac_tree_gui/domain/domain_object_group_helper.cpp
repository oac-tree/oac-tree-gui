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
#include "domain_object_type_registry.h"

#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/instruction_registry.h>
#include <sup/platform/library_names.h>

#include <algorithm>
#include <iostream>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Registers a plugin name for ObjectGroupInfo using given group name.
 *
 * @param group_name The name of the group
 * @param plugin_name The name of the plugin in this group
 * @param group_info The group info object to modify
 * @param legacy_exists Add also legacy name, if true.
 */
void AddPluginNameToGroupInfo(const std::string& group_name, const std::string& plugin_name,
                              std::vector<ObjectGroupInfo>& group_info, bool legacy_exists)
{
  auto iter =
      std::find_if(group_info.begin(), group_info.end(), [&group_name](const ObjectGroupInfo& info)
                   { return info.group_name == group_name; });
  if (iter == group_info.end())
  {
    group_info.push_back({group_name, {}});
    iter = std::prev(group_info.end());
  }

  iter->object_names.push_back(plugin_name);
  if (legacy_exists)
  {
    const auto legacy_name = mvvm::utils::ReplaceSubString(plugin_name, "oac-tree", "sequencer");
    iter->object_names.push_back(legacy_name);
  }
}

}  // namespace

std::vector<std::string> GetDefaultPluginNameGroups()
{
  return {
      kCoreGroup, kMathGroup, kEPICSGroup, kControlGroup, kSupGroup, kMiscGroup,
  };
}

std::vector<ObjectGroupInfo> CreatePluginNameGroups()
{
  std::vector<ObjectGroupInfo> result;
  AddPluginNameToGroupInfo(kCoreGroup, domainconstants::kCorePluginName, result, false);
  AddPluginNameToGroupInfo(kCoreGroup, domainconstants::kCorePluginName2, result, false);
  AddPluginNameToGroupInfo(kMathGroup, domainconstants::kMathExprPluginName, result, true);
  AddPluginNameToGroupInfo(kEPICSGroup, domainconstants::kEpicsCAPluginName, result, true);
  AddPluginNameToGroupInfo(kEPICSGroup, domainconstants::kEpicsPVXSPluginName, result, true);
  AddPluginNameToGroupInfo(kControlGroup, domainconstants::kControlPluginName, result, true);
  AddPluginNameToGroupInfo(kSupGroup, domainconstants::kSupConfigPluginName, result, true);
  AddPluginNameToGroupInfo(kSupGroup, domainconstants::kSupTimingPluginName, result, true);
  AddPluginNameToGroupInfo(kMiscGroup, domainconstants::kSystemPluginName, result, true);
  AddPluginNameToGroupInfo(kMiscGroup, domainconstants::kStringPluginName, result, true);
  AddPluginNameToGroupInfo(kMiscGroup, domainconstants::kSupPulseCounterPluginName, result, true);
  return result;
}

std::string GetGroupNameFromPluginName(const std::string& plugin_name)
{
  static std::map<std::string, std::string> kGroupMap = []()
  {
    std::map<std::string, std::string> result;
    auto plugin_groups = CreatePluginNameGroups();
    for (const auto& group_info : plugin_groups)
    {
      for (const auto& name : group_info.object_names)
      {
        result[name] = group_info.group_name;
      }
    }
    return result;
  }();

  const auto [path, stripped_basename] = sup::platform::SplitDynamicLibFilename(plugin_name);

  auto iter = kGroupMap.find(stripped_basename);
  if (iter != kGroupMap.end())
  {
    return iter->second;
  }
  return kMiscGroup;
}

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups(
    const DomainObjectTypeRegistry& type_registry)
{
  // groups with plugin names
  const static auto kPluginGroups = CreatePluginNameGroups();

  // groups with instruction names
  std::vector<oac_tree_gui::ObjectGroupInfo> result;

  // loop over all groups with plugin names
  for (const auto& plugin_group_info : kPluginGroups)
  {
    // collecting instruction names for all plugins in a group
    std::vector<std::string> instructions_in_group;
    for (const auto& plugin_name : plugin_group_info.object_names)
    {
      auto instructions_in_plugin = type_registry.GetObjectNames(plugin_name);
      (void)std::copy(instructions_in_plugin.begin(), instructions_in_plugin.end(),
                      std::back_inserter(instructions_in_group));
    }

    // The name of instruction group coincides with the name of plugin group.
    // The content of a group is all instruction names found in plugins.
    result.push_back({plugin_group_info.group_name, instructions_in_group});
  }

  return result;
}

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups()
{
  auto loaded_plugin_names = GetAllPluginNames(sup::oac_tree::GlobalInstructionRegistry());

  // add core "library" to loaded plugins
  loaded_plugin_names.insert(sup::oac_tree::Constants::DOC_CORE_LIBRARY_NAME);

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

}  // namespace oac_tree_gui
