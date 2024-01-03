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

#include "domain_procedure_builder.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/sessionmodel.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

namespace
{
//! Creates domain instruction corresponding to a given InstructionItem, and pushes it
//! to a given parent. Return newly created domain instruction to a user.
instruction_t* ProcessInstruction(const sequencergui::InstructionItem* instruction,
                                  instruction_t* parent)
{
  auto domain_instruction = instruction->CreateDomainInstruction();
  auto ptr = domain_instruction.get();
  if (!parent->InsertInstruction(std::move(domain_instruction), parent->ChildrenCount()))
  {
    throw std::runtime_error("Error while trying to insert instruction");
  }
  return ptr;
}

//! Creates domain instruction corresponding to a given InstructionItem, and pushes it
//! to a given procedure. Return newly created domain instruction to a user.
instruction_t* ProcessInstruction(const sequencergui::InstructionItem* instruction,
                                  procedure_t* procedure)
{
  auto domain_instruction = instruction->CreateDomainInstruction();
  auto ptr = domain_instruction.get();
  procedure->PushInstruction(std::move(domain_instruction));
  return ptr;
}

}  // namespace

namespace sequencergui
{
DomainProcedureBuilder::~DomainProcedureBuilder() = default;

void DomainProcedureBuilder::Iterate(const sequencergui::InstructionItem* instruction,
                                     instruction_t* parent)
{
  for (auto& instruction : instruction->GetInstructions())
  {
    auto domain_instruction = ProcessInstruction(instruction, parent);
    m_instruction_to_id.insert({domain_instruction, instruction->GetIdentifier()});

    if (domain_instruction)
    {
      Iterate(instruction, domain_instruction);
    }
  }
}

void DomainProcedureBuilder::PopulateDomainInstructions(const InstructionContainerItem* container,
                                                        procedure_t* procedure)
{
  if (!procedure->GetTopInstructions().empty())
  {
    throw std::runtime_error("Error in DomainObjectBuilder non-empty domain.");
  }

  m_instruction_to_id.clear();

  for (const auto instruction_item : container->GetInstructions())
  {
    auto domain_instruction = ProcessInstruction(instruction_item, procedure);
    m_instruction_to_id.insert({domain_instruction, instruction_item->GetIdentifier()});

    if (domain_instruction)
    {
      Iterate(instruction_item, domain_instruction);
    }
  }
}

void DomainProcedureBuilder::PopulateDomainWorkspace(const WorkspaceItem* workspace,
                                                     procedure_t* procedure)
{
  m_workspace_builder = std::make_unique<DomainWorkspaceBuilder>();
  m_workspace_builder->PopulateDomainWorkspace(workspace,
                                               const_cast<workspace_t*>(&procedure->GetWorkspace()));
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

std::unique_ptr<procedure_t> DomainProcedureBuilder::CreateProcedure(
    const ProcedureItem& procedure_item)
{
  DomainProcedureBuilder builder;
  auto result = std::make_unique<procedure_t>(procedure_item.GetFileName());
  builder.PopulateProcedure(procedure_item, *result);
  return result;
}

}  // namespace sequencergui
