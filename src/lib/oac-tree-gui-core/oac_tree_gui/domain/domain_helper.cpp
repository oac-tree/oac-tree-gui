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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_helper.h"

#include "domain_object_type_registry.h"
#include "oac_tree_gui/domain/domain_constants.h"

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/core/platform.h>
#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>

#include <algorithm>
#include <iostream>
#include <sstream>

namespace
{

bool LoadPlugin(const std::string& name)
{
  bool is_success{false};
  try
  {
    ::sup::oac_tree::LoadPlugin(name);
    is_success = true;
  }
  catch (const std::exception&)
  {
    is_success = false;
  }
  return is_success;
}

/**
 * @brief Updates global registry with correspondance of plugin name to the type names of
 * instruction and variables.
 */
void UpdateGlobalDomainObjectTypeRegistry(const std::string& plugin_name)
{
  auto& registry = oac_tree_gui::GlobalDomainObjectTypeRegistry();
  registry.Update(plugin_name,
                  sup::oac_tree::GlobalInstructionRegistry().RegisteredInstructionNames());
  registry.Update(plugin_name, sup::oac_tree::GlobalVariableRegistry().RegisteredVariableNames());
}

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
  auto result = ::sup::oac_tree::GlobalVariableRegistry().Create(domain_name);
  if (!result)
  {
    throw RuntimeException("Error in GlobalVariableRegistry: can create variable '" + domain_name
                           + "'");
  }
  return result;
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

std::pair<bool, std::string> LoadPlugins()
{
  std::vector<std::string> failed_plugins;

  UpdateGlobalDomainObjectTypeRegistry(domainconstants::kCorePluginName);

  static const std::vector<std::string> plugins = {
      domainconstants::kEpicsCAPluginName,   domainconstants::kEpicsPVXSPluginName,
      domainconstants::kEpicsMiscPluginName, domainconstants::kControlPluginName,
      domainconstants::kSupConfigPluginName, domainconstants::kSupPulseCounterPluginName,
      domainconstants::kMathExprPluginName,  domainconstants::kSupTimingPluginName,
      domainconstants::kSystemPluginName,    domainconstants::kStringPluginName};

  for (const auto& name : plugins)
  {
    const auto file_name = GetPluginFileName(name);
    if (LoadPlugin(file_name))
    {
      UpdateGlobalDomainObjectTypeRegistry(name);
    }
    else
    {
      failed_plugins.push_back(file_name);
    }
  }

  if (!failed_plugins.empty())
  {
    std::ostringstream ostr;
    ostr << "Warning! Some of sequencer plugins failed to load: "
         << mvvm::utils::ToCommaSeparatedString(failed_plugins) << ". ";
    ostr << "There will be less instructions and variables available.";
    std::cout << ostr.str() << "\n";
    return {false, ostr.str()};
  }

  return {true, ""};
}

std::string GetMainTextFromMetadata(const anyvalue_t& metadata)
{
  return sup::oac_tree::GetMainTextFromMetadata(metadata);
}

std::string GetTitleTextFromMetadata(const anyvalue_t& metadata)
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

}  // namespace oac_tree_gui
