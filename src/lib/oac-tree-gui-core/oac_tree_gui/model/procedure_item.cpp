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

#include "procedure_item.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/iterate_helper.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <set>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Adds plugin name coresponding to a given domain type into the container.
 */
void CollectPluginFileNames(
    const std::string &domain_type,
    const std::function<std::string(const std::string &)> &object_to_plugin_name,
    std::set<std::string> &plugin_file_names)
{
  if (auto plugin_name = object_to_plugin_name(domain_type); !plugin_name.empty())
  {
    plugin_file_names.insert(oac_tree_gui::GetPluginFileName(plugin_name));
  }
}

/**
 * @brief Collects all plugin names necessary to handle variables in the given workspace.
 */
void CollectVariablePluginNames(
    const oac_tree_gui::WorkspaceItem &workspace_item,
    const std::function<std::string(const std::string &)> &object_to_plugin_name,
    std::set<std::string> &plugin_names)
{
  for (auto variable : workspace_item.GetVariables())
  {
    CollectPluginFileNames(variable->GetDomainType(), object_to_plugin_name, plugin_names);
  }
}

/**
 * @brief Collects all plugin names necessary to handle instructions in the given instruction
 * container.
 */
void CollectInstructionPluginNames(
    const oac_tree_gui::InstructionContainerItem &container,
    const std::function<std::string(const std::string &)> &object_to_plugin_name,
    std::set<std::string> &plugin_names)
{
  auto on_instruction =
      [&plugin_names, &object_to_plugin_name](const oac_tree_gui::InstructionItem *item)
  { CollectPluginFileNames(item->GetDomainType(), object_to_plugin_name, plugin_names); };
  oac_tree_gui::IterateInstructionContainer<const oac_tree_gui::InstructionItem *>(
      container.GetInstructions(), on_instruction);
}

}  // namespace

static inline const std::string kInstructions = "kInstructions";
static inline const std::string kWorkspace = "kWorkspace";
static inline const std::string kPreamble = "kPreamble";
static inline const std::string kFileName = "kFileName";

ProcedureItem::ProcedureItem() : CompoundItem(GetStaticType())
{
  AddProperty(itemconstants::kName, std::string()).SetDisplayName("Name");
  AddProperty(itemconstants::kDescription, std::string()).SetDisplayName("Description");
  AddProperty(kFileName, std::string()).SetDisplayName("File name");
  AddBranch<InstructionContainerItem>(kInstructions).SetDisplayName("Instructions");
  AddBranch<WorkspaceItem>(kWorkspace).SetDisplayName("Workspace");
  AddBranch<ProcedurePreambleItem>(kPreamble).SetDisplayName("Preamble");

  SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::string ProcedureItem::GetStaticType()
{
  return "Procedure";
}

std::unique_ptr<mvvm::SessionItem> ProcedureItem::Clone() const
{
  return std::make_unique<ProcedureItem>(*this);
}

std::string ProcedureItem::GetName() const
{
  return Property<std::string>(itemconstants::kName);
}

void ProcedureItem::SetName(const std::string &value)
{
  (void)SetProperty(itemconstants::kName, value);
}

std::string ProcedureItem::GetDescription() const
{
  return Property<std::string>(itemconstants::kDescription);
}

void ProcedureItem::SetDescription(const std::string &value)
{
  (void)SetProperty(itemconstants::kDescription, value);
}

std::string ProcedureItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void ProcedureItem::SetFileName(const std::string &value)
{
  (void)SetProperty(kFileName, value);
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

std::vector<std::string> CollectPluginNames(
    const ProcedureItem &item,
    const std::function<std::string(const std::string &)> &object_to_plugin_name)
{
  std::set<std::string> result;

  CollectVariablePluginNames(*item.GetWorkspace(), object_to_plugin_name, result);
  CollectInstructionPluginNames(*item.GetInstructionContainer(), object_to_plugin_name, result);

  return {result.begin(), result.end()};
}

void UpdatePluginNames(const ProcedureItem &item,
                       const std::function<std::string(const std::string &)> &object_to_plugin_name)
{
  auto names = CollectPluginNames(item, object_to_plugin_name);
  item.GetPreambleItem()->SetPluginPaths(names);
}

}  // namespace oac_tree_gui
