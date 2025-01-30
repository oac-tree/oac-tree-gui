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

#include "monitor_model.h"

#include <oac_tree_gui/model/workspace_item.h>

#include <mvvm/model/model_utils.h>

namespace oac_tree_gui
{

MonitorModel::MonitorModel() : mvvm::ApplicationModel("MonitorModel")
{
  PopulateModel();
}

WorkspaceItem *MonitorModel::GetWorkspaceItem() const
{
  return mvvm::utils::GetTopItem<WorkspaceItem>(this);
}

void MonitorModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

void MonitorModel::PopulateModel()
{
  InsertItem<WorkspaceItem>();
}

}  // namespace oac_tree_gui
