/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/domain_utils.h"

#include "AttributeMap.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "SequenceParser.h"
#include "Variable.h"
#include "VariableRegistry.h"
#include "sequencergui/core/exceptions.h"
#include "sequencergui/model/domain_constants.h"

#include <algorithm>
#include <iostream>

namespace sequencergui::DomainUtils
{
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name)
{
  auto result = ::sup::sequencer::GlobalInstructionRegistry().Create(domain_name);
  if (!result)
  {
    throw TransformToDomainException("Error in GlobalInstructionRegistry: can create instruction '"
                                     + domain_name + "'");
  }
  return result;
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

bool IsChannelAccessAvailable()
{
  static const std::vector<std::string> names = GetDomainVariableNames();
  return std::find(names.begin(), names.end(), domainconstants::kChannelAccessVariableType)
         != names.end();
}

//! Loads codac plugins.

void LoadCodacPlugins()
{
  static const std::vector<std::string> plugins = {
      "libsequencer-sup-config.so", "libsequencer-sup-pcs.so", "libsequencer-sup-pulsecounter.so",
      "libsequencer-sup-timing.so", "libsequencer-ca.so",      "libsequencer-pvxs.so",
      "libsequencer-misc.so"};

  for (const auto& name : plugins)
  {
    auto result = ::sup::sequencer::LoadPlugin(name);
    std::cout << "Loading " + name + " plugin, result:" << result << std::endl;
  }
}

//! Loads local plugins.

void LoadLocalPlugins()
{
  static const std::vector<std::string> plugins = {"libsequencerpluginepics.so"};

  for (const auto& name : plugins)
  {
    auto result = ::sup::sequencer::LoadPlugin(name);
    std::cout << "Loading " + name + " plugin, result:" << result << std::endl;
  }
}

}  // namespace sequencergui::DomainUtils
