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

PvMonitorProject::PvMonitorProject(callback_t modified_callback, callback_t loaded_callback)
    : AppProject({std::move(modified_callback), std::move(loaded_callback),
                  PvMonitorProject::kApplicationType})
{
  RegisterModel<MonitorModel>();
}

PvMonitorProject::~PvMonitorProject() = default;

MonitorModel *PvMonitorProject::GetMonitorModel()
{
  return GetModelCount() > 0 ? GetModel<MonitorModel>() : nullptr;
}

}  // namespace sequencergui
