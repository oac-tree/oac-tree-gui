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

#include "variable_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace.h>
#include <sup/oac-tree/workspace_info.h>

namespace oac_tree_gui
{

std::unique_ptr<VariableItem> CreateVariableItem(const sup::oac_tree::VariableInfo& info)
{
  auto result = oac_tree_gui::CreateVariableItem(info.GetType());

  // We create domain variable only to be able to use InitFromDomain mechanism for attribute
  // propagation.
  auto domain = CreateDomainVariable(info);
  result->InitFromDomain(domain.get());

  return result;
}

std::vector<const VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::WorkspaceInfo& info,
                                                       WorkspaceItem* workspace_item)
{
  std::vector<const VariableItem*> index_to_variable_item;

  if (workspace_item->GetVariableCount() != 0)
  {
    throw RuntimeException("WorkspaceItem already contains some variables");
  }

  index_to_variable_item.resize(info.GetNumberOfVariables());

  for (const auto& [name, variable_info] : info.GetVariableInfos())
  {
    auto variable_item = CreateVariableItem(variable_info);
    auto variable_item_ptr = variable_item.get();
    index_to_variable_item[variable_info.GetIndex()] = variable_item_ptr;
    workspace_item->InsertItem(std::move(variable_item), mvvm::TagIndex::Append());
  }

  return index_to_variable_item;
}

std::vector<const VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::Workspace& workspace,
                                                       const anytype_registry_t* registry,
                                                       WorkspaceItem* workspace_item)
{
  std::vector<const VariableItem*> result;

  if (workspace_item->GetTotalItemCount() > 0)
  {
    throw std::runtime_error("Error: WorkspaceItem is not empty.");
  }

  for (auto variable : workspace.GetVariables())
  {
    auto item = oac_tree_gui::CreateVariableItem(variable->GetType());
    item->InitFromDomain(variable, registry);
    result.push_back(item.get());
    workspace_item->InsertItem(std::move(item), mvvm::TagIndex::Append());
  }

  return result;
}

}  // namespace oac_tree_gui
