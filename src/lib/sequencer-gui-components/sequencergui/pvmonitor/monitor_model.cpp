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

#include "monitor_model.h"

#include <sequencergui/model/workspace_item.h>

#include <mvvm/model/model_utils.h>

namespace sequencergui
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

}  // namespace sequencergui
