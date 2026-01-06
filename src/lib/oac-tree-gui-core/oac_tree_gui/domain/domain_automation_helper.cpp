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

#include "domain_automation_helper.h"

#include "domain_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/variable_utils.h>

#include <algorithm>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns optional value of the attribute with the given name.
 */
std::optional<std::string> GetAttribute(const std::vector<sup::oac_tree::AttributeInfo>& attributes,
                                        const std::string& attribute_name)
{
  auto pos =
      std::find_if(attributes.begin(), attributes.end(), [&attribute_name](const auto& element)
                   { return element.first == attribute_name; });
  return pos == attributes.end() ? std::optional<std::string>{} : pos->second;
}

}  // namespace

sup::oac_tree::InstructionInfo CreateInstructionInfo(const sup::oac_tree::Instruction& instruction,
                                                     sup::dto::uint32 index)
{
  return {instruction.GetType(), instruction.GetCategory(), index,
          instruction.GetStringAttributes()};
}

sup::oac_tree::VariableInfo CreateVariableInfo(const sup::oac_tree::Variable& variable,
                                               sup::dto::uint32 index)
{
  return sup::oac_tree::utils::CreateVariableInfo(&variable, index);
}

std::unique_ptr<sup::oac_tree::Instruction> CreateDomainInstruction(
    const sup::oac_tree::InstructionInfo& info)
{
  auto result = CreateDomainInstruction(info.GetType());
  (void)result->AddAttributes(info.GetAttributes());
  return result;
}

std::unique_ptr<sup::oac_tree::Variable> CreateDomainVariable(
    const sup::oac_tree::VariableInfo& info)
{
  auto result = CreateDomainVariable(info.GetType());
  (void)result->AddAttributes(info.GetAttributes());
  return result;
}

std::optional<std::string> GetAttribute(const sup::oac_tree::InstructionInfo& info,
                                        const std::string& attribute_name)
{
  return GetAttribute(info.GetAttributes(), attribute_name);
}

std::optional<std::string> GetAttribute(const sup::oac_tree::VariableInfo& info,
                                        const std::string& attribute_name)
{
  return GetAttribute(info.GetAttributes(), attribute_name);
}

}  // namespace oac_tree_gui
