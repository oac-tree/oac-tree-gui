/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/monitor_model.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

namespace suppvmonitor
{

MonitorModel::MonitorModel()
{
  RegisterItem<sequencergui::ChannelAccessVariableItem>();
  RegisterItem<sequencergui::LocalVariableItem>();
  RegisterItem<sequencergui::PVClientVariableItem>();
  RegisterItem<sequencergui::PVServerVariableItem>();
  RegisterItem<sequencergui::UnknownVariableItem>();
  RegisterItem<sequencergui::WorkspaceItem>();
}

}  // namespace suppvmonitor
