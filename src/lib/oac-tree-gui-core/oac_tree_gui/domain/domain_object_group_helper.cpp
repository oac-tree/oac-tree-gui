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

#include <algorithm>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns correspondence of group name to the plugin name.
 *
 * A single group can contain more than one plugin.
 */
std::vector<ObjectGroupInfo> CreatePluginNameGroups()
{
  const std::vector<ObjectGroupInfo> result = {
      {kCoreGroup, {domainconstants::kCorePluginName}},
      {kMathGroup, {domainconstants::kMathExprPluginName}},
      {kEPICSGroup, {domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName}},
      {kControlGroup, {domainconstants::kControlPluginName}},
      {kSupGroup, {domainconstants::kSupConfigPluginName, domainconstants::kSupTimingPluginName}},
      {kMiscGroup,
       {domainconstants::kSystemPluginName, domainconstants::kStringPluginName,
        domainconstants::kSupPulseCounterPluginName}}};
  return result;
}

}  // namespace

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

}  // namespace oac_tree_gui
