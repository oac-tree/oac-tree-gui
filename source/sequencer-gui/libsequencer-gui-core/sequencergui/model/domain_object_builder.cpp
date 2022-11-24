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

#include "sequencergui/model/domain_object_builder.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <mvvm/model/sessionmodel.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

#include <QDebug>

namespace
{
//! Creates domain instruction corresponding to a given InstructionItem, and pushes it
//! to a given parent. Return newly created domain instruction to a user.
instruction_t* ProcessInstruction(const sequencergui::InstructionItem* instruction,
                                  instruction_t* parent)
{
  auto domain_instruction = instruction->CreateDomainInstruction();
  auto ptr = domain_instruction.get();
  if (!parent->InsertInstruction(domain_instruction.release(), parent->ChildrenCount()))
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
  if (!procedure->PushInstruction(domain_instruction.release()))
  {
    throw std::runtime_error("Error while trying to push instruction");
  }
  return ptr;
}

}  // namespace

namespace sequencergui
{
DomainObjectBuilder::~DomainObjectBuilder() = default;

void DomainObjectBuilder::Iterate(const sequencergui::InstructionItem* instruction,
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

void DomainObjectBuilder::PopulateDomainInstructions(const InstructionContainerItem* container,
                                                     procedure_t* procedure)
{
  if (!procedure->GetInstructions().empty())
  {
    throw std::runtime_error("Error in DomainObjectBuilder non-empty domain.");
  }

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

void DomainObjectBuilder::PopulateDomainWorkspace(const WorkspaceItem* workspace,
                                                  procedure_t* procedure)
{
  if (!procedure->GetWorkspace()->VariableNames().empty())
  {
    throw std::runtime_error("Error in DomainObjectBuilder non-empty domain workspace.");
  }

  for (const auto variable_item : workspace->GetVariables())
  {
    auto domain_variable = variable_item->CreateDomainVariable();
    m_variable_to_id.insert({domain_variable.get(), variable_item->GetIdentifier()});

    auto it = m_variablename_to_id.find(variable_item->GetName());
    if (it != m_variablename_to_id.end())
    {
      throw std::runtime_error("Error in DomainObjectBuilder: such variable name '"
                               + variable_item->GetName() + "' already exist");
    }

    m_variablename_to_id.insert({variable_item->GetName(), variable_item->GetIdentifier()});

    procedure->AddVariable(variable_item->GetName(), domain_variable.release());
  }
}

std::unique_ptr<procedure_t> DomainObjectBuilder::CreateProcedure(
    const ProcedureItem* procedure_item)
{
  if (!procedure_item)
  {
    throw RuntimeException("Procedure is not initialised");
  }

  auto result = std::make_unique<procedure_t>();

  PopulateDomainInstructions(procedure_item->GetInstructionContainer(), result.get());
  PopulateDomainWorkspace(procedure_item->GetWorkspace(), result.get());

  return result;
}

//! Build procedure. Ownership remains in the procedure.
//! FIXME confusing logic: BuildProcedure .vs. CreateProcedure.
//! BuildProcedure ownership is kept, CreateProcedure ownership
//! is passed outside, but methods FindInstructionIdentifier stop working
void DomainObjectBuilder::BuildProcedure(const ProcedureItem* procedure_item)
{
  m_procedure_item = procedure_item;
  m_instruction_to_id.clear();
  m_variable_to_id.clear();
  m_variablename_to_id.clear();
  m_procedure = CreateProcedure(procedure_item);
}

procedure_t* DomainObjectBuilder::GetProcedure() const
{
  return m_procedure.get();
}

std::string DomainObjectBuilder::FindInstructionIdentifier(const instruction_t* instruction) const
{
  if (!m_procedure_item || !GetProcedure())
  {
    throw std::runtime_error("Error in FindInstructionItem:");
  }

  auto it = m_instruction_to_id.find(instruction);
  return it == m_instruction_to_id.end() ? std::string() : it->second;
}

std::string DomainObjectBuilder::FindVariableItemIdentifier(const variable_t* variable) const
{
  if (!m_procedure_item || !GetProcedure())
  {
    throw std::runtime_error("Error in FindVariableItemIdentifier:");
  }

  auto it = m_variable_to_id.find(variable);
  return it == m_variable_to_id.end() ? std::string() : it->second;
}

std::string DomainObjectBuilder::FindVariableItemIdentifier(const std::string& variable_name) const
{
  if (!m_procedure_item || !GetProcedure())
  {
    throw std::runtime_error("Error in FindVariableItemIdentifier:");
  }

  auto it = m_variablename_to_id.find(variable_name);
  return it == m_variablename_to_id.end() ? std::string() : it->second;
}

}  // namespace sequencergui
