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

#include "gui_object_builder.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <sup/dto/anytype_registry.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <stdexcept>

namespace
{

//! Creates Registry containing type defined in a preamble.
//! We can't used registry on board of procedure itself, because it hasn't been setup  yet.
std::unique_ptr<sup::dto::AnyTypeRegistry> CreateRegistry(const procedure_t &procedure)
{
  auto result = std::make_unique<sup::dto::AnyTypeRegistry>();

  for (const auto &type_registration : procedure.GetPreamble().GetTypeRegistrations())
  {
    auto anytype = ParseTypeRegistrationInfo(type_registration, procedure.GetFilename(), *result);
    result->RegisterType(anytype);
  }

  return result;
}

}  // namespace

namespace sequencergui
{

GUIObjectBuilder::~GUIObjectBuilder() = default;

std::unique_ptr<ProcedureItem> GUIObjectBuilder::CreateProcedureItem(const procedure_t *procedure,
                                                                     bool root_only)
{
  // REFACTORING
  m_instruction_map =
      std::make_unique<sup::sequencer::InstructionMap>(procedure->RootInstruction());

  auto result = std::make_unique<ProcedureItem>();
  PopulateProcedureItem(procedure, result.get(), root_only);
  return result;
}

void GUIObjectBuilder::PopulateProcedureItem(const procedure_t *procedure,
                                             ProcedureItem *procedure_item, bool root_only)
{
  if (!procedure)
  {
    throw std::runtime_error("Error: uninitialised procedure");
  }

  m_to_instruction_item.clear();
  m_domain_variable_to_item.clear();
  m_variablename_to_item.clear();

  auto instruction_container = procedure_item->GetInstructionContainer();
  PopulateInstructionContainerItem(procedure, instruction_container, root_only);

  PopulateProcedurePreambleItem(procedure->GetPreamble(), *procedure_item->GetPreambleItem());

  auto registry = CreateRegistry(*procedure);

  auto workspace_item = procedure_item->GetWorkspace();
  PopulateWorkspaceItem(procedure, workspace_item, registry.get());
}

//! Populates empty WorkspaceItem with the content from sequencer Procedure.

void GUIObjectBuilder::PopulateWorkspaceItem(const procedure_t *procedure, WorkspaceItem *workspace,
                                             const anytype_registry_t *registry)
{
  if (workspace->GetTotalItemCount() > 0)
  {
    throw std::runtime_error("Error: WorkspaceItem is not empty.");
  }

  for (auto variable : procedure->GetWorkspace().GetVariables())
  {
    auto item = sequencergui::CreateVariableItem(variable->GetType());
    Save(variable, item.get());
    item->InitFromDomain(variable, registry);
    workspace->InsertItem(std::move(item), {"", -1});
  }
}

InstructionItem *GUIObjectBuilder::FindInstructionItem(const instruction_t *instruction) const
{
  auto it = m_to_instruction_item.find(instruction);
  return it == m_to_instruction_item.end() ? nullptr : it->second;
}

VariableItem *GUIObjectBuilder::FindVariableItem(const variable_t *variable) const
{
  auto it = m_domain_variable_to_item.find(variable);
  return it == m_domain_variable_to_item.end() ? nullptr : it->second;
}

VariableItem *GUIObjectBuilder::FindVariableItem(const std::string &variable_name) const
{
  auto it = m_variablename_to_item.find(variable_name);
  return it == m_variablename_to_item.end() ? nullptr : it->second;
}

const instruction_t *GUIObjectBuilder::FindInstruction(
    const InstructionItem *instruction_item) const
{
  auto predicate = [instruction_item](auto it) { return it.second == instruction_item; };
  auto it = std::find_if(m_to_instruction_item.begin(), m_to_instruction_item.end(), predicate);
  return it == m_to_instruction_item.end() ? nullptr : it->first;
}

size_t GUIObjectBuilder::FindInstructionItemIndex(const InstructionItem *instruction_item) const
{
  // REFACTORING
  if (auto domain_instruction = FindInstruction(instruction_item); domain_instruction)
  {
    return m_instruction_map->FindInstructionIndex(domain_instruction);
  }

  throw RuntimeException("Can't find instruction index");
}

//! Populates empty InstructionContainerItem with the content from sequencer Procedure.

void GUIObjectBuilder::PopulateInstructionContainerItem(const procedure_t *procedure,
                                                        InstructionContainerItem *container,
                                                        bool root_only)
{
  if (container->GetTotalItemCount() > 0)
  {
    throw std::runtime_error("Error: InstructionContainerItem is not empty.");
  }

  if (root_only)
  {
    if (auto instruction = procedure->RootInstruction(); instruction)
    {
      auto next_parent_item = ProcessInstruction(instruction, container);
      if (next_parent_item)
      {
        Iterate(instruction, next_parent_item);
      }
    }
  }
  else
  {
    for (auto instruction : procedure->GetTopInstructions())
    {
      auto next_parent_item = ProcessInstruction(instruction, container);
      if (next_parent_item)
      {
        Iterate(instruction, next_parent_item);
      }
    }
  }
}

mvvm::SessionItem *GUIObjectBuilder::ProcessInstruction(const instruction_t *instruction,
                                                        mvvm::SessionItem *parent)
{
  auto item = sequencergui::CreateInstructionItem(instruction->GetType());

  Save(instruction, item.get());

  item->InitFromDomain(instruction);
  auto next_parent = parent->InsertItem(std::move(item), mvvm::TagIndex::Append());
  return next_parent;
}

void GUIObjectBuilder::Iterate(const instruction_t *instruction, mvvm::SessionItem *parent)
{
  for (auto &instruction : instruction->ChildInstructions())
  {
    auto next_parent_item = ProcessInstruction(instruction, parent);
    if (next_parent_item)
    {
      Iterate(instruction, next_parent_item);
    }
  }
}

void GUIObjectBuilder::Save(const instruction_t *instruction, InstructionItem *item)
{
  auto it = m_to_instruction_item.find(instruction);
  if (it != m_to_instruction_item.end())
  {
    throw std::runtime_error("Error in GUIObjectBuilder: domain instruction already present");
  }
  m_to_instruction_item.insert({instruction, item});
}

void GUIObjectBuilder::Save(const variable_t *variable, VariableItem *item)
{
  auto it = m_variablename_to_item.find(variable->GetName());
  if (it != m_variablename_to_item.end())
  {
    throw std::runtime_error("Error in GUIObjectBuilder: domain variable already present");
  }

  m_domain_variable_to_item.insert({variable, item});
  m_variablename_to_item.insert({variable->GetName(), item});
}

}  // namespace sequencergui
