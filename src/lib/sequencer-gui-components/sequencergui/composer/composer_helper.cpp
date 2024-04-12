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

#include "composer_helper.h"

#include "sequencergui/domain/domain_constants.h"
#include "sequencergui/domain/domain_object_type_registry.h"

namespace
{

/**
 * @brief Returns correspondence of group name to the plugin name.
 *
 * A single group can contain more than one plugin.
 */
std::vector<sequencergui::ObjectGroupInfo> CreatePluginNameGroups()
{
  // So far only EPICS group contains two plugins (CA and PVXS).
  // Other groups contains single plugin each.
  return {
      {sequencergui::kCoreGroup, {sequencergui::domainconstants::kCorePluginName}},
      {sequencergui::kMathGroup, {sequencergui::domainconstants::kMathExprPluginName}},
      {sequencergui::kEPICSGroup,
       {sequencergui::domainconstants::kEpicsCAPluginName,
        sequencergui::domainconstants::kEpicsPVXSPluginName}},
      {sequencergui::kControlGroup, {sequencergui::domainconstants::kControlPluginName}},
      {sequencergui::kConfigGroup, {sequencergui::domainconstants::kSupConfigPluginName}},
      {sequencergui::kMiscGroup, {sequencergui::domainconstants::kSupPulseCounterPluginName}},
  };
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
  return CreateInstructionTypeGroups(GlobalDomainObjectTypeRegistry());
}

}  // namespace sequencergui
