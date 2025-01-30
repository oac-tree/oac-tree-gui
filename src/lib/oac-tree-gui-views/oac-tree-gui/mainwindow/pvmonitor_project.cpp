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

#include "pvmonitor_project.h"

#include <oac-tree-gui/pvmonitor/monitor_model.h>

namespace oac_tree_gui
{

PvMonitorProject::PvMonitorProject(const mvvm::ProjectContext &context) : AppProject(context)
{
  RegisterModel<MonitorModel>();
}

PvMonitorProject::~PvMonitorProject() = default;

MonitorModel *PvMonitorProject::GetMonitorModel()
{
  return GetModelCount() > 0 ? GetModel<MonitorModel>() : nullptr;
}

}  // namespace oac_tree_gui
