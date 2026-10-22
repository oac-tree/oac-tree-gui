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

#include "instruction_descriptor_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/transform/attribute_descriptor_helper.h>

#include <sup/oac-tree/instruction_info.h>

#include <stack>

namespace oac_tree_gui
{

InstructionDescriptor CreateInstructionDescriptor(const instruction_t& instruction)
{
  const auto domain_type = instruction.GetType();
  InstructionDescriptor result{domain_type, instruction.GetCategory(), {}};
  for (const auto& definition : instruction.GetAttributeDefinitions())
  {
    result.attributes.push_back(CreateAttributeDescriptor(domain_type, definition));
  }
  return result;
}

InstructionDescriptor CreateInstructionDescriptor(const sup::oac_tree::InstructionInfo& info)
{
  const auto domain_type = info.GetType();
  InstructionDescriptor result{domain_type, info.GetCategory(), {}};
  result.attributes.push_back(
      CreateAttributeDescriptor(domain_type, domainconstants::kNameAttribute, std::string{}, false,
                                sup::oac_tree::AttributeCategory::kLiteral));
  result.attributes.push_back(
      CreateAttributeDescriptor(domain_type, domainconstants::kIsRootAttribute, false, false,
                                sup::oac_tree::AttributeCategory::kLiteral));
  if (info.GetCategory() != sup::oac_tree::Instruction::kAction)
  {
    result.attributes.push_back(
        CreateAttributeDescriptor(domain_type, domainconstants::kShowCollapsedAttribute, false,
                                  false, sup::oac_tree::AttributeCategory::kLiteral));
  }
  return result;
}

InstructionNode CreateInstructionNode(const std::string& domain_type)
{
  return {CreateInstructionDescriptor(*CreateDomainInstruction(domain_type)), {}, 0, {}};
}

InstructionNode CreateInstructionNodeTree(const instruction_t& instruction)
{
  InstructionNode result;
  std::uint32_t next_index{0};
  std::stack<std::pair<const instruction_t*, InstructionNode*>> stack;
  stack.emplace(&instruction, &result);

  while (!stack.empty())
  {
    auto [domain, node] = stack.top();
    stack.pop();

    node->descriptor = CreateInstructionDescriptor(*domain);
    node->attributes = domain->GetStringAttributes();
    node->index = next_index++;

    // the vector is not resized later, so node pointers stay valid
    const auto children = domain->ChildInstructions();
    node->children.resize(children.size());
    for (auto index = children.size(); index > 0; --index)
    {
      // reverse order, so the first child gets the next index
      stack.emplace(children[index - 1], &node->children[index - 1]);
    }
  }
  return result;
}

InstructionNode CreateInstructionNodeTree(const sup::oac_tree::InstructionInfo& info)
{
  InstructionNode result;
  std::stack<std::pair<const sup::oac_tree::InstructionInfo*, InstructionNode*>> stack;
  stack.push({&info, &result});

  while (!stack.empty())
  {
    auto [current, node] = stack.top();
    stack.pop();

    node->descriptor = CreateInstructionDescriptor(*current);
    node->attributes = current->GetAttributes();
    node->index = current->GetIndex();

    // the vector is not resized later, so node pointers stay valid
    const auto children = current->Children();
    node->children.resize(children.size());
    for (std::size_t index = 0; index < children.size(); ++index)
    {
      stack.push({children[index], &node->children[index]});
    }
  }
  return result;
}

}  // namespace oac_tree_gui
