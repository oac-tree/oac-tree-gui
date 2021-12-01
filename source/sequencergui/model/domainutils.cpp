/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/domainutils.h"

#include "AttributeMap.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "Variable.h"
#include "VariableRegistry.h"
#include "sequencergui/model/domain_constants.h"

#include <algorithm>

namespace sequi::DomainUtils
{
std::unique_ptr<instruction_t> CreateDomainInstruction(const std::string& domain_name)
{
  auto result = ::sup::sequencer::GlobalInstructionRegistry().Create(domain_name);
  if (!result)
  {
    throw std::runtime_error("Error in GlobalInstructionRegistry: can create instruction '"
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

//! Returns map of current instruction attributes.

std::map<std::string, std::string> GetAttributes(const instruction_t* instruction)
{
  std::map<std::string, std::string> result;
  auto attributes = instruction->GetAttributes();
  for (const auto& it : instruction->GetAttributes())
  {
    result.insert(it);
  }
  return result;
}

//! Returns true if given instruction has a root-instruction attribute set.

bool IsRootInstruction(const instruction_t* instruction)
{
  static const std::vector<std::string> expected_values{"true", "yes"};
  if (!instruction->HasAttribute(DomainConstants::kIsRootAttribute))
  {
    return false;
  }

  auto value = instruction->GetAttribute(DomainConstants::kIsRootAttribute);
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);

  return std::find(expected_values.begin(), expected_values.end(), value) != expected_values.end();
}

}  // namespace sequi::DomainUtils
