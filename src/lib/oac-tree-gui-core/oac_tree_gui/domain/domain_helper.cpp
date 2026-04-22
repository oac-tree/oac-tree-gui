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

#include "domain_helper.h"

#include "domain_constants.h"

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/i_job_info_io.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>
#include <sup/platform/library_names.h>

#include <algorithm>

namespace oac_tree_gui
{
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name)
{
  return ::sup::oac_tree::GlobalInstructionRegistry().Create(domain_name);
}

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name)
{
  return ::sup::oac_tree::GlobalVariableRegistry().Create(domain_name);
}

std::vector<std::string> GetDomainInstructionNames()
{
  return ::sup::oac_tree::GlobalInstructionRegistry().RegisteredInstructionNames();
}

std::vector<std::string> GetDomainVariableNames()
{
  return ::sup::oac_tree::GlobalVariableRegistry().RegisteredVariableNames();
}

bool IsVariableTypeAvailable(const std::string& domain_type)
{
  static const std::vector<std::string> names = GetDomainVariableNames();
  return std::find(names.begin(), names.end(), domain_type) != names.end();
}

bool IsInstructionTypeAvailable(const std::string& domain_type)
{
  static const std::vector<std::string> names = GetDomainInstructionNames();
  return std::find(names.begin(), names.end(), domain_type) != names.end();
}

bool IsSequencerPluginEpicsAvailable()
{
  return IsVariableTypeAvailable(domainconstants::kPvAccessServerVariableType);  // simplified check
}

bool IsSequencerControlPluginAvailable()
{
  return IsInstructionTypeAvailable(
      domainconstants::kWaitForConditionInstructionType);  // simplified check
}

std::vector<std::string> GetBasicPluginList()
{
  static const std::vector<std::string> kPluginList = {
      domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName,
      domainconstants::kControlPluginName, domainconstants::kMathExprPluginName,
      domainconstants::kSystemPluginName,  domainconstants::kStringPluginName};
  return kPluginList;
}

std::vector<std::string> GetLegacyPluginNameList()
{
  const auto basic_plugin_list = GetBasicPluginList();
  const auto sup_plugin_list = GetSupPluginList();
  std::vector<std::string> plugin_list;
  plugin_list.reserve(basic_plugin_list.size() + sup_plugin_list.size());
  (void)plugin_list.insert(plugin_list.end(), basic_plugin_list.begin(), basic_plugin_list.end());
  (void)plugin_list.insert(plugin_list.end(), sup_plugin_list.begin(), sup_plugin_list.end());

  for (auto& name : plugin_list)
  {
    name = mvvm::utils::ReplaceSubString(name, "oac-tree", "sequencer");
  }
  return plugin_list;
}

std::vector<std::string> GetSupPluginList()
{
  static const std::vector<std::string> kPluginList = {domainconstants::kSupConfigPluginName,
                                                       domainconstants::kSupPulseCounterPluginName,
                                                       domainconstants::kSupTimingPluginName};
  return kPluginList;
}

std::string GetMainTextFromMetadata(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME)
      && (metadata[sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME].GetType()
          == sup::dto::StringType))
  {
    return metadata[sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME].As<std::string>();
  }
  return {};
}

bool IsSelectTextDialog(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME))
  {
    return metadata[sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME].As<sup::dto::uint32>()
           == sup::oac_tree::dialog_type::kSelection;
  }

  return false;
}

bool IsMessageBoxDialog(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME))
  {
    return metadata[sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME].As<sup::dto::uint32>()
           == sup::oac_tree::dialog_type::kConfirmation;
  }

  return false;
}

std::string GetPluginFileName(const std::string& plugin_name)
{
  const auto [path, stripped_basename] = sup::platform::SplitDynamicLibFilename(plugin_name);
  if (stripped_basename.empty())
  {
   const  std::string error_message = "GetPluginFileName(): trying to load plugin with empty name";
    throw InvalidOperationException(error_message);
  }
  return sup::platform::CreateFullDynamicLibPath(path, stripped_basename);
}

bool IsValidInstructionIndex(sup::dto::uint32 index)
{
  return index != sup::oac_tree::kInvalidInstructionIndex;
}

std::string GetDescriptionForInstructionType(const std::string& instruction_type)
{
  if (!::sup::oac_tree::GlobalInstructionRegistry().IsRegisteredInstructionName(instruction_type))
  {
    return {};
  }

  return ::sup::oac_tree::GlobalInstructionRegistry()
      .GetDocumentation(instruction_type)
      .GetField(sup::oac_tree::Constants::INSTR_DOC_DESCRIPTION);
}

}  // namespace oac_tree_gui
