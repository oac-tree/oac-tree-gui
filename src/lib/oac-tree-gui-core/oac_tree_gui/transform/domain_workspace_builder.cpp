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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_workspace_builder.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

//! Populates empty sequencer workspace with the content of WorkspaceItem.

void DomainWorkspaceBuilder::PopulateDomainWorkspace(const WorkspaceItem* item,
                                                     workspace_t* workspace)
{
  if (!workspace->VariableNames().empty())
  {
    throw LogicErrorException("Non-empty domain workspace.");
  }

  m_domain_variable_to_item.clear();
  m_domain_name_to_item.clear();

  for (const auto variable_item : item->GetVariables())
  {
    auto domain_variable = variable_item->CreateDomainVariable();

    auto iter = m_domain_name_to_item.find(variable_item->GetName());
    if (iter != m_domain_name_to_item.end())
    {
      throw RuntimeException("Variable name '" + variable_item->GetName() + "' already exist");
    }

    m_domain_variable_to_item.insert({domain_variable.get(), variable_item});
    m_domain_name_to_item.insert({variable_item->GetName(), variable_item});

    workspace->AddVariable(variable_item->GetName(), std::move(domain_variable));
  }
}

VariableItem* DomainWorkspaceBuilder::GetVariableItemFromDomainVariableName(
    const std::string& name) const
{
  auto iter = m_domain_name_to_item.find(name);
  return iter == m_domain_name_to_item.end() ? nullptr : iter->second;
}

}  // namespace oac_tree_gui
