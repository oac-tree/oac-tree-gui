/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/domain/domain_utils.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>

#include <sup/sequencer/attribute_map.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

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
  std::cout << "Loading " + name + " plugin, result:" << is_success << std::endl;
  return is_success;
}
}  // namespace

namespace sequencergui::DomainUtils
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
    throw std::runtime_error("Error in GlobalVariableRegistry: can create variable '" + domain_name
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

//! Returns map of current instruction attributes.

std::map<std::string, std::string> GetAttributes(const instruction_t* instruction)
{
  std::map<std::string, std::string> result;
  for (const auto& it : instruction->GetAttributes())
  {
    result.insert(it);
  }
  return result;
}

//! Returns map of current variable attributes.

std::map<std::string, std::string> GetAttributes(const variable_t* variable)
{
  std::map<std::string, std::string> result;
  for (const auto& it : variable->GetAttributes())
  {
    result.insert(it);
  }
  return result;
}

//! Returns true if given instruction has a root-instruction attribute set.

bool IsRootInstruction(const instruction_t* instruction)
{
  static const std::vector<std::string> expected_values{"true", "yes"};
  if (!instruction->HasAttribute(domainconstants::kIsRootAttribute))
  {
    return false;
  }

  auto value = instruction->GetAttribute(domainconstants::kIsRootAttribute);
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);

  return std::find(expected_values.begin(), expected_values.end(), value) != expected_values.end();
}

bool IsChannelAccessClientAvailable()
{
  return IsVariableTypeAvailable(domainconstants::kChannelAccessVariableType);
}

bool IsPVAccessClientAvailable()
{
  return IsVariableTypeAvailable(domainconstants::kPVClientVariableType);
}

bool IsPVAccessServerAvailable()
{
  return IsVariableTypeAvailable(domainconstants::kPVServerVariableType);
}

bool IsVariableTypeAvailable(const std::string& domain_variable_type)
{
  static const std::vector<std::string> names = GetDomainVariableNames();
  return std::find(names.begin(), names.end(), domain_variable_type) != names.end();
}

//! Loads codac plugins.

void LoadCodacPlugins()
{
  static const std::vector<std::string> plugins = {"libsequencer-ca.so", "libsequencer-pvxs.so",
                                                   "libsequencer-misc.so"};

  for (const auto& name : plugins)
  {
    LoadPlugin(name);
  }
}

//! Loads local plugins.

void LoadLocalPlugins()
{
  static const std::vector<std::string> plugins = {"libsequencer-ca.so", "libsequencer-pvxs.so",
                                                   "libsequencer-misc.so"};

  for (const auto& name : plugins)
  {
    LoadPlugin(name);
  }
}

void LoadPlugins()
{
#ifdef SEQUENCERGUI_CODAC
  LoadCodacPlugins();
#else
  LoadLocalPlugins();
#endif
}

void AddNonEmptyAttribute(variable_t* variable, const std::string& attribute_name,
                          const std::string& attribute_value)
{
  if (!attribute_value.empty())
  {
    variable->AddAttribute(attribute_name, attribute_value);
  }
}

}  // namespace sequencergui::DomainUtils
