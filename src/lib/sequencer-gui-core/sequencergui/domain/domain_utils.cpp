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

#include "domain_utils.h"

#include "domain_object_type_registry.h"

#include <sequencergui/core/exceptions.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <iostream>
#include <sstream>

namespace
{
bool LoadPlugin(const std::string& name)
{
  bool is_success{false};
  try
  {
    ::sup::sequencer::LoadPlugin(name);
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
  auto& registry = sequencergui::GlobalDomainObjectTypeRegistry();
  registry.Update(plugin_name,
                  sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames());
  registry.Update(plugin_name, sup::sequencer::GlobalVariableRegistry().RegisteredVariableNames());
}

}  // namespace

namespace sequencergui
{
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name)
{
  return ::sup::sequencer::GlobalInstructionRegistry().Create(domain_name);
}

std::unique_ptr<variable_t> CreateDomainVariable(const std::string& domain_name)
{
  auto result = ::sup::sequencer::GlobalVariableRegistry().Create(domain_name);
  if (!result)
  {
    throw RuntimeException("Error in GlobalVariableRegistry: can create variable '" + domain_name
                           + "'");
  }
  return result;
}

std::vector<std::string> GetDomainInstructionNames()
{
  return ::sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
}

std::vector<std::string> GetDomainVariableNames()
{
  return ::sup::sequencer::GlobalVariableRegistry().RegisteredVariableNames();
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
      domainconstants::kMathExprPluginName};

  for (const auto& name : plugins)
  {
    if (LoadPlugin(name))
    {
      UpdateGlobalDomainObjectTypeRegistry(name);
    }
    else
    {
      failed_plugins.push_back(name);
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
  return sup::sequencer::GetMainTextFromMetadata(metadata);
}

std::string GetTitleTextFromMetadata(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::sequencer::Constants::USER_CHOICES_TEXT_NAME)
      && (metadata[sup::sequencer::Constants::USER_CHOICES_TEXT_NAME].GetType()
          == sup::dto::StringType))
  {
    return metadata[sup::sequencer::Constants::USER_CHOICES_TEXT_NAME].As<std::string>();
  }
  return {};
}

bool IsSelectTextDialog(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::sequencer::Constants::USER_CHOICES_DIALOG_TYPE_NAME))
  {
    return metadata[sup::sequencer::Constants::USER_CHOICES_DIALOG_TYPE_NAME].As<sup::dto::uint32>()
           == sup::sequencer::dialog_type::kSelection;
  }

  return false;
}

bool IsMessageBoxDialog(const anyvalue_t& metadata)
{
  if (metadata.HasField(sup::sequencer::Constants::USER_CHOICES_DIALOG_TYPE_NAME))
  {
    return metadata[sup::sequencer::Constants::USER_CHOICES_DIALOG_TYPE_NAME].As<sup::dto::uint32>()
           == sup::sequencer::dialog_type::kConfirmation;
  }

  return false;
}

}  // namespace sequencergui
