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

#include "domain_helper.h"

#include "domain_constants.h"

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/core/platform.h>
#include <mvvm/utils/file_utils.h>
#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/i_job_info_io.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>

#include <algorithm>
#include <filesystem>

namespace
{

std::string_view RemovePrefix(std::string_view name, const std::string& prefix)
{
  name.remove_prefix(std::min(name.find_first_not_of(prefix), name.size()));
  return name;
}

std::string_view RemoveSuffix(std::string_view name, const std::string& suffix)
{
  if (name.rfind(suffix) == (name.size() - suffix.size()))
  {
    name.remove_suffix(suffix.size());
  }
  return name;
}

}  // namespace

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

std::vector<std::string> GetSupPluginList()
{
  static const std::vector<std::string> kPluginList = {domainconstants::kSupConfigPluginName,
                                                       domainconstants::kSupPulseCounterPluginName,
                                                       domainconstants::kSupTimingPluginName};
  return kPluginList;
}

std::vector<std::string> GetBasicPluginFileNames()
{
  std::vector<std::string> result;
  for (const auto& name : GetBasicPluginList())
  {
    result.push_back(GetPluginFileName(name));
  }
  return result;
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
  if (plugin_name.empty() || mvvm::IsWindowsHost())
  {
    return plugin_name;
  }

  if (std::filesystem::path(plugin_name).has_parent_path())
  {
    return plugin_name;
  }

  std::string_view view = plugin_name;
  view = RemovePrefix(view, "lib");
  view = RemoveSuffix(view, ".so");
  view = RemoveSuffix(view, ".dylib");

  if (mvvm::IsLinuxHost())
  {
    return "lib" + std::string(view) + ".so";
  }

  if (mvvm::IsMacHost())
  {
    return "lib" + std::string(view) + ".dylib";
  }

  return plugin_name;
}

std::string GetPluginNameFromFileName(const std::string& file_name)
{
  const std::string base_name = mvvm::utils::GetFileName(file_name);
  if (base_name.empty() || mvvm::IsWindowsHost())
  {
    return {};
  }
  std::string_view view = base_name;
  view = RemovePrefix(view, "lib");
  view = RemoveSuffix(view, ".so");
  view = RemoveSuffix(view, ".dylib");
  return std::string(view);
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
