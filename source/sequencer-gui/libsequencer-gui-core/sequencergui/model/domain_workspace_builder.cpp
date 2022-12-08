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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/domain_workspace_builder.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

namespace sequencergui
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

    auto it = m_domain_name_to_item.find(variable_item->GetName());
    if (it != m_domain_name_to_item.end())
    {
      throw ExistingKeyException("Variable name '" + variable_item->GetName() + "' already exist");
    }

    m_domain_variable_to_item.insert({domain_variable.get(), variable_item});
    m_domain_name_to_item.insert({variable_item->GetName(), variable_item});

    workspace->AddVariable(variable_item->GetName(), domain_variable.release());
  }
}

VariableItem* DomainWorkspaceBuilder::GetVariableItemFromDomainVariableName(
    const std::string& name) const
{
  auto it = m_domain_name_to_item.find(name);
  return it == m_domain_name_to_item.end() ? nullptr : it->second;
}

}  // namespace sequencergui
