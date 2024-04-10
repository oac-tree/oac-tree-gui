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

#include "procedure_item.h"

#include <sequencergui/domain/domain_object_type_registry.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/iterate_helper.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>

namespace
{

/**
 * @brief Adds plugin name coresponding to a given domain type into the container.
 */
void CollectPluginName(const std::string &domain_type, std::set<std::string> &plugin_names)
{
  const auto &registry = sequencergui::GlobalDomainObjectTypeRegistry();
  if (auto plugin_name = registry.GetPluginName(domain_type).value_or(std::string());
      !plugin_name.empty())
  {
    plugin_names.insert(plugin_name);
  }
}

/**
 * @brief Collects all plugin names necessary to handle variables in the given workspace.
 */
void CollectVariablePluginNames(const sequencergui::WorkspaceItem &workspace_item,
                                std::set<std::string> &plugin_names)
{
  for (auto variable : workspace_item.GetVariables())
  {
    CollectPluginName(variable->GetDomainType(), plugin_names);
  }
}

/**
 * @brief Collects all plugin names necessary to handle instructions in the given instruction
 * container.
 */
void CollectInstructionPluginNames(const sequencergui::InstructionContainerItem &container,
                                   std::set<std::string> &plugin_names)
{
  auto on_instruction = [&plugin_names](const sequencergui::InstructionItem *item)
  { CollectPluginName(item->GetDomainType(), plugin_names); };
  sequencergui::IterateInstructionContainer<const sequencergui::InstructionItem *>(
      container.GetInstructions(), on_instruction);
}

}  // namespace

namespace sequencergui
{

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";
static inline const std::string kPreamble = "kPreamble";
static inline const std::string kFileName = "kFileName";

ProcedureItem::ProcedureItem() : CompoundItem(Type)
{
  AddProperty(itemconstants::kName, std::string()).SetDisplayName("Name");
  AddProperty(itemconstants::kDescription, std::string()).SetDisplayName("Description");
  AddProperty(kFileName, std::string()).SetDisplayName("File name");
  AddBranch<InstructionContainerItem>(kInstructions).SetDisplayName("Instructions");
  AddBranch<WorkspaceItem>(kWorkspace).SetDisplayName("Workspace");
  AddBranch<ProcedurePreambleItem>(kPreamble).SetDisplayName("Preamble");
}

std::unique_ptr<mvvm::SessionItem> ProcedureItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ProcedureItem>(*this, make_unique_id);
}

std::string ProcedureItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void ProcedureItem::SetName(const std::string &value)
{
  SetProperty(itemconstants::kName, value);
}

std::string ProcedureItem::GetDescription() const
{
  return Property<std::string>(itemconstants::kDescription);
}

void ProcedureItem::SetDescription(const std::string &value)
{
  SetProperty(itemconstants::kDescription, value);
}

std::string ProcedureItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void ProcedureItem::SetFileName(const std::string &value)
{
  SetProperty(kFileName, value);
}

InstructionContainerItem *ProcedureItem::GetInstructionContainer() const
{
  return GetItem<InstructionContainerItem>(kInstructions);
}

WorkspaceItem *ProcedureItem::GetWorkspace() const
{
  return GetItem<WorkspaceItem>(kWorkspace);
}

ProcedurePreambleItem *ProcedureItem::GetPreambleItem() const
{
  return GetItem<ProcedurePreambleItem>(kPreamble);
}

std::set<std::string> CollectPluginNames(const ProcedureItem &item)
{
  std::set<std::string> result;

  CollectVariablePluginNames(*item.GetWorkspace(), result);
  CollectInstructionPluginNames(*item.GetInstructionContainer(), result);

  return result;
}

}  // namespace sequencergui
