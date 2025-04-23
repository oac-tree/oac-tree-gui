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

#include "domain_object_group_helper.h"

#include "domain_constants.h"
#include "domain_helper.h"
#include "domain_object_type_registry.h"

#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace
{

/**
 * @brief Returns correspondence of group name to the plugin name.
 *
 * A single group can contain more than one plugin.
 */
std::vector<oac_tree_gui::ObjectGroupInfo> CreatePluginNameGroups()
{
  const std::vector<oac_tree_gui::ObjectGroupInfo> result = {
      {oac_tree_gui::kCoreGroup, {oac_tree_gui::domainconstants::kCorePluginName}},
      {oac_tree_gui::kMathGroup, {oac_tree_gui::domainconstants::kMathExprPluginName}},
      {oac_tree_gui::kEPICSGroup,
       {oac_tree_gui::domainconstants::kEpicsCAPluginName,
        oac_tree_gui::domainconstants::kEpicsPVXSPluginName}},
      {oac_tree_gui::kControlGroup, {oac_tree_gui::domainconstants::kControlPluginName}},
      {oac_tree_gui::kTimingGroup, {oac_tree_gui::domainconstants::kSupTimingPluginName}},
      {oac_tree_gui::kConfigGroup, {oac_tree_gui::domainconstants::kSupConfigPluginName}},
      {oac_tree_gui::kMiscGroup,
       {oac_tree_gui::domainconstants::kSystemPluginName,
        oac_tree_gui::domainconstants::kStringPluginName,
        oac_tree_gui::domainconstants::kSupPulseCounterPluginName}},

  };
  return result;
}

/**
 * @brief Returns vector of instruction names from the list containing a mixture of different names.
 */
std::vector<std::string> GetInstructionNames(const std::vector<std::string>& names)
{
  std::vector<std::string> result;
  const static auto known_types = oac_tree_gui::GetDomainInstructionNames();

  auto on_element = [](auto element) { return mvvm::utils::Contains(known_types, element); };
  std::copy_if(std::begin(names), std::end(names), std::back_inserter(result), on_element);
  return result;
}

}  // namespace

namespace oac_tree_gui
{

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
      std::copy(instructions_in_plugin.begin(), instructions_in_plugin.end(),
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
  // Global type registry contains both instruction and variables. We have to remove variable names
  // fbefore returning info to the user.

  auto group_infos = CreateInstructionTypeGroups(GlobalDomainObjectTypeRegistry());

  for (auto& info : group_infos)
  {
    info.object_names = GetInstructionNames(info.object_names);
  }
  return group_infos;
}

}  // namespace oac_tree_gui
