/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#include "sequencergui/model/guiobjectbuilder.h"

#include "Instruction.h"
#include "Procedure.h"
#include "Workspace.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <stdexcept>

namespace sequencergui
{

void GUIObjectBuilder::PopulateProcedureItem(const procedure_t *procedure,
                                             ProcedureItem *procedure_item)
{
  if (!procedure)
  {
    throw std::runtime_error("Error: uninitialised procedure");
  }

  m_to_instruction_item.clear();
  m_variable_to_id.clear();
  m_variablename_to_id.clear();

  auto instruction_container = procedure_item->GetInstructionContainer();
  PopulateInstructionContainerItem(procedure, instruction_container, /*root_only*/ true);

  auto workspace_item = procedure_item->GetWorkspace();
  PopulateWorkspaceItem(procedure, workspace_item);
}

//! Populates empty WorkspaceItem with the content from sequencer Procedure.

void GUIObjectBuilder::PopulateWorkspaceItem(const procedure_t *procedure, WorkspaceItem *workspace)
{
  if (workspace->GetTotalItemCount() > 0)
  {
    throw std::runtime_error("Error: WorkspaceItem is not empty.");
  }

  for (auto variable : procedure->GetWorkspace()->GetVariables())
  {
    auto item = sequencergui::CreateVariableItem(variable->GetType());
    Save(variable, item.get());
    item->InitFromDomain(variable);
    workspace->InsertItem(std::move(item), {"", -1});
  }
}

std::string GUIObjectBuilder::FindInstructionItemIdentifier(const instruction_t *instruction) const
{
  auto it = m_to_instruction_item.find(instruction);
  return it == m_to_instruction_item.end() ? std::string() : it->second->GetIdentifier();
}

InstructionItem *GUIObjectBuilder::FindInstructionItem(const instruction_t *instruction) const
{
  auto it = m_to_instruction_item.find(instruction);
  return it == m_to_instruction_item.end() ? nullptr : it->second;
}

std::string GUIObjectBuilder::FindVariableItemIdentifier(const variable_t *variable) const
{
  auto it = m_variable_to_id.find(variable);
  return it == m_variable_to_id.end() ? std::string() : it->second->GetIdentifier();
}

VariableItem *GUIObjectBuilder::FindVariableItem(const variable_t *variable) const
{
  auto it = m_variable_to_id.find(variable);
  return it == m_variable_to_id.end() ? nullptr : it->second;
}

std::string GUIObjectBuilder::FindVariableItemIdentifier(const std::string &variable_name) const
{
  auto it = m_variablename_to_id.find(variable_name);
  return it == m_variablename_to_id.end() ? std::string() : it->second->GetIdentifier();
}

VariableItem *GUIObjectBuilder::FindVariableItem(const std::string &variable_name) const
{
  auto it = m_variablename_to_id.find(variable_name);
  return it == m_variablename_to_id.end() ? nullptr : it->second;
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
    if (auto instruction = procedure->RootInstrunction(); instruction)
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
    for (auto instruction : procedure->GetInstructions())
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
  // method InsertItem returns just raw pointer to item
  auto next_parent = parent->InsertItem(std::move(item), {"", -1});
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
  auto it = m_variablename_to_id.find(variable->GetName());
  if (it != m_variablename_to_id.end())
  {
    throw std::runtime_error("Error in GUIObjectBuilder: domain variable already present");
  }

  m_variable_to_id.insert({variable, item});
  m_variablename_to_id.insert({variable->GetName(), item});
}

}  // namespace sequi
