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

#include "workspace_filter_viewmodel.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_variable_items.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/viewmodel.h>

namespace oac_tree_gui
{

WorkspaceFilterViewModel::WorkspaceFilterViewModel(QObject *parent_object)
    : QSortFilterProxyModel(parent_object)
{
  setRecursiveFilteringEnabled(true);
}

void WorkspaceFilterViewModel::SetPattern(const QString &pattern)
{
  m_pattern = pattern.toLower();
  invalidateFilter();
}

bool WorkspaceFilterViewModel::filterAcceptsRow(int source_row,
                                                const QModelIndex &source_parent) const
{
  if (m_pattern.isEmpty())
  {
    return true;
  }

  const QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);

  if (auto viewmodel = GetViewModel(); viewmodel)
  {
    return IsItemAccepted(viewmodel->GetSessionItemFromIndex(index0));
  }

  return false;
}

bool WorkspaceFilterViewModel::IsItemAccepted(const mvvm::SessionItem *item) const
{
  if (!item)
  {
    return false;
  }

  // check if parent is connectable variable, and check its channel name
  if (auto variable_item = mvvm::utils::FindItemUp<ConnectableVariableItem>(item); variable_item)
  {
    return IsValidName(variable_item->GetChannel()) ? true : IsValidName(variable_item->GetName());
  }

  // if not a channel, check variable name
  if (auto variable_item = mvvm::utils::FindItemUp<LocalVariableItem>(item); variable_item)
  {
    return IsValidName(variable_item->GetName());
  }

  return false;
}

bool WorkspaceFilterViewModel::IsValidName(const std::string &name) const
{
  const QString str = QString::fromStdString(name).toLower();
  return str.contains(m_pattern);
}

mvvm::ViewModel *WorkspaceFilterViewModel::GetViewModel() const
{
  return dynamic_cast<mvvm::ViewModel *>(sourceModel());
}

}  // namespace oac_tree_gui
