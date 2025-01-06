/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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
std::vector<sequencergui::ObjectGroupInfo> CreatePluginNameGroups()
{
  const std::vector<sequencergui::ObjectGroupInfo> result = {
      {sequencergui::kCoreGroup, {sequencergui::domainconstants::kCorePluginName}},
      {sequencergui::kMathGroup, {sequencergui::domainconstants::kMathExprPluginName}},
      {sequencergui::kEPICSGroup,
       {sequencergui::domainconstants::kEpicsCAPluginName,
        sequencergui::domainconstants::kEpicsPVXSPluginName}},
      {sequencergui::kControlGroup, {sequencergui::domainconstants::kControlPluginName}},
      {sequencergui::kTimingGroup, {sequencergui::domainconstants::kSupTimingPluginName}},
      {sequencergui::kConfigGroup, {sequencergui::domainconstants::kSupConfigPluginName}},
      {sequencergui::kMiscGroup,
       {sequencergui::domainconstants::kSystemPluginName,
        sequencergui::domainconstants::kStringPluginName,
        sequencergui::domainconstants::kSupPulseCounterPluginName}},

  };
  return result;
}

/**
 * @brief Returns vector of instruction names from the list containing a mixture of different names.
 */
std::vector<std::string> GetInstructionNames(const std::vector<std::string>& names)
{
  std::vector<std::string> result;
  const static auto known_types = sequencergui::GetDomainInstructionNames();

  auto on_element = [](auto element) { return mvvm::utils::Contains(known_types, element); };
  std::copy_if(std::begin(names), std::end(names), std::back_inserter(result), on_element);
  return result;
}

}  // namespace

namespace sequencergui
{

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups(
    const DomainObjectTypeRegistry& type_registry)
{
  // groups with plugin names
  const static auto kPluginGroups = CreatePluginNameGroups();

  // groups with instruction names
  std::vector<sequencergui::ObjectGroupInfo> result;

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

}  // namespace sequencergui
