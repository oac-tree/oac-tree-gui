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

namespace
{

//! Collection of group names for widget trees.
const std::string kCoreGroup = "Core";
const std::string kMathGroup = "Math";
const std::string kEPICSGroup = "EPICS";
const std::string kControlGroup = "SUP Control";
const std::string kConfigGroup = "SUP Configuration";
const std::string kMiscGroup = "Miscellaneous";

/**
 * @brief Returns correspondance of group name to plugin name.
 *
 * A single group can contain more than one plugin.
 */
std::vector<sequencergui::ObjectGroupInfo> CreatePluginNameGroups()
{
  return {
      {kCoreGroup, {sequencergui::domainconstants::kCorePluginName}},
      {kMathGroup, {sequencergui::domainconstants::kMathExprPluginName}},
      {kEPICSGroup,
       {sequencergui::domainconstants::kEpicsCAPluginName,
        sequencergui::domainconstants::kEpicsPVXSPluginName}},
      {kControlGroup, {sequencergui::domainconstants::kControlPluginName}},
      {kConfigGroup, {sequencergui::domainconstants::kSupConfigPluginName}},
      {kMiscGroup, {sequencergui::domainconstants::kSupPulseCounterPluginName}},
  };
}

}  // namespace

namespace sequencergui
{

std::vector<ObjectGroupInfo> CreateInstructionTypeGroups()
{
  std::vector<sequencergui::ObjectGroupInfo> result;

  const static auto group_name_to_plugin_names = CreatePluginNameGroups();

  for (const auto& group_info : group_name_to_plugin_names)
  {
  }

  return result;
}

}  // namespace sequencergui
