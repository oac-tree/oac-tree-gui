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

#include "domain_procedure_builder.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/domain_workspace_builder.h>

#include <mvvm/model/session_model.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

#include <stack>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The InstructionStackNode class stores information during traversing SessionItem hierarchy.
 */
struct InstructionStackNode
{
  const InstructionItem& item;
  instruction_t& domain_instruction;
};

/**
 * @brief CreateAdjustedDomainInstruction
 */
std::unique_ptr<instruction_t> CreateAdjustedDomainInstruction(const InstructionItem& item)
{
  const auto behavior = item.GetBehavior();

  if (behavior == itemconstants::kNativeBehavior)
  {
    return item.CreateDomainInstruction();
  }

  if (behavior == itemconstants::kSucceedBehavior)
  {
    return CreateDomainInstruction(domainconstants::kSucceedInstructionType);
  }

  if (behavior == itemconstants::kFailBehavior)
  {
    return CreateDomainInstruction(domainconstants::kFailedInstructionType);
  }

  // kInvisibleBehavior, or any other unsupported behavior returns empty domain instruction
  return {};
}

}  // namespace

DomainProcedureBuilder::~DomainProcedureBuilder() = default;

std::unique_ptr<procedure_t> DomainProcedureBuilder::CreateProcedure(
    const ProcedureItem& procedure_item)
{
  DomainProcedureBuilder builder;
  auto result = std::make_unique<procedure_t>(procedure_item.GetFileName());
  builder.PopulateProcedure(procedure_item, *result);
  return result;
}

void DomainProcedureBuilder::PopulateDomainInstructions(const InstructionContainerItem* container,
                                                        procedure_t* procedure)
{
  if (!procedure->GetTopInstructions().empty())
  {
    throw std::runtime_error("Error in DomainObjectBuilder non-empty domain.");
  }

  m_instruction_to_id.clear();

  std::stack<InstructionStackNode> stack;

  for (const auto item : container->GetInstructions())
  {
    if (auto domain_instruction = CreateAdjustedDomainInstruction(*item); domain_instruction)
    {
      auto domain_instruction_ptr = domain_instruction.get();
      procedure->PushInstruction(std::move(domain_instruction));
      (void)m_instruction_to_id.insert({domain_instruction_ptr, item->GetIdentifier()});

      stack.push({*item, *domain_instruction_ptr});
    }
  }

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto child_item : node.item.GetInstructions())
    {
      if (auto domain_instruction = CreateAdjustedDomainInstruction(*child_item);
          domain_instruction)
      {
        auto domain_instruction_ptr = domain_instruction.get();
        (void)node.domain_instruction.InsertInstruction(std::move(domain_instruction),
                                                  node.domain_instruction.ChildrenCount());
        (void)m_instruction_to_id.insert({domain_instruction_ptr, child_item->GetIdentifier()});

        stack.push({*child_item, *domain_instruction_ptr});
      }
    }
  }
}

void DomainProcedureBuilder::PopulateDomainWorkspace(const WorkspaceItem* workspace,
                                                     procedure_t* procedure)
{
  m_workspace_builder = std::make_unique<DomainWorkspaceBuilder>();
  m_workspace_builder->PopulateDomainWorkspace(
      workspace, const_cast<workspace_t*>(&procedure->GetWorkspace()));
}
void DomainProcedureBuilder::PopulateProcedure(const ProcedureItem& procedure_item,
                                               procedure_t& procedure)
{
  PopulateDomainInstructions(procedure_item.GetInstructionContainer(), &procedure);
  PopulateDomainWorkspace(procedure_item.GetWorkspace(), &procedure);
  PopulateProcedurePreamble(*procedure_item.GetPreambleItem(), procedure.GetPreamble());
}

std::string DomainProcedureBuilder::FindInstructionIdentifier(
    const instruction_t* instruction) const
{
  auto it = m_instruction_to_id.find(instruction);
  return it == m_instruction_to_id.end() ? std::string() : it->second;
}

std::string DomainProcedureBuilder::FindVariableItemIdentifier(const variable_t* variable) const
{
  return m_workspace_builder->GetVariableItemFromDomainVariableName(variable->GetName())
      ->GetIdentifier();
}

std::unique_ptr<procedure_t> CreateDomainProcedure(const ProcedureItem& procedure_item)
{
  DomainProcedureBuilder builder;
  return builder.CreateProcedure(procedure_item);
}

}  // namespace oac_tree_gui
