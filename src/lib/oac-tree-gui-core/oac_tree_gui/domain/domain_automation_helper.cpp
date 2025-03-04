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

#include "domain_automation_helper.h"

#include "domain_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

#include <algorithm>

namespace oac_tree_gui
{

std::unique_ptr<sup::oac_tree::Instruction> CreateDomainInstruction(
    const sup::oac_tree::InstructionInfo &info)
{
  auto result = CreateDomainInstruction(info.GetType());
  result->AddAttributes(info.GetAttributes());
  return result;
}

std::unique_ptr<sup::oac_tree::Variable> CreateDomainVariable(
    const sup::oac_tree::VariableInfo &info)
{
  auto result = CreateDomainVariable(info.GetType());
  result->AddAttributes(info.GetAttributes());
  return result;
}

std::optional<std::string> GetAttribute(const sup::oac_tree::InstructionInfo &info,
                                        const std::string &attribute_name)
{
  auto attributes = info.GetAttributes();
  auto pos =
      std::find_if(attributes.begin(), attributes.end(), [&attribute_name](const auto &element)
                   { return element.first == attribute_name; });
  return pos == attributes.end() ? std::optional<std::string>{} : pos->second;
}

}  // namespace oac_tree_gui
