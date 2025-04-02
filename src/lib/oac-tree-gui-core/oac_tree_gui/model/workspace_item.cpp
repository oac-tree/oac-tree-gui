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

#include "workspace_item.h"

#include <oac_tree_gui/model/variable_item.h>

#include <algorithm>

namespace oac_tree_gui
{

static inline const std::string kVariableItems = "kVariableItems";

WorkspaceItem::WorkspaceItem() : CompoundItem(GetStaticType())
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kVariableItems), /*as_default*/ true);
}

std::string WorkspaceItem::GetStaticType()
{
  return "Workspace";
}

std::unique_ptr<mvvm::SessionItem> WorkspaceItem::Clone() const
{
  return std::make_unique<WorkspaceItem>(*this);
}

std::vector<VariableItem *> WorkspaceItem::GetVariables() const
{
  return GetItems<VariableItem>(kVariableItems);
}

std::vector<std::string> WorkspaceItem::GetVariableNames() const
{
  std::vector<std::string> result;
  auto variables = GetVariables();
  result.reserve(variables.size());
  std::transform(variables.begin(), variables.end(), std::back_inserter(result),
                 [](auto element) { return element->GetName(); });
  return result;
}

int WorkspaceItem::GetVariableCount() const
{
  return static_cast<int>(GetVariables().size());
}

}  // namespace oac_tree_gui
