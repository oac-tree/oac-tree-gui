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

#include "pvmonitor_project.h"

#include <sequencergui/pvmonitor/monitor_model.h>

namespace sequencergui
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

}  // namespace sequencergui
