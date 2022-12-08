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

#include "suppvmonitor/workspace_item_controller.h"

#include <mvvm/model/model_utils.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_event.h>
#include <mvvm/signals/model_event_handler.h>
#include <mvvm/signals/model_event_handler.h>

#include <stdexcept>

namespace suppvmonitor
{

WorkspaceItemController::WorkspaceItemController(MonitorModel* model) : m_model(model) {}

void WorkspaceItemController::ProcessDomainEvent(const WorkspaceEvent& event)
{
  if (auto item = GeVariableItemForName(event.m_variable_name); item)
  {
    sequencergui::UpdateAnyValue(event.m_value, *item);
  }


}

sequencergui::VariableItem* WorkspaceItemController::GeVariableItemForName(const std::string& name)
{
  if (!GetWorkspaceItem())
  {
    throw std::logic_error("No WOrkspaceItem exists");
  }

  for (auto variable_item : GetWorkspaceItem()->GetVariables())
  {
    if (variable_item->GetName() == name)
    {
      return variable_item;
    }
  }
  return nullptr;
}

sequencergui::WorkspaceItem* WorkspaceItemController::GetWorkspaceItem()
{
  return mvvm::utils::GetTopItem<sequencergui::WorkspaceItem>(m_model);
}

//! Sets the callback to report GUI events.
void WorkspaceItemController::SetCallback(const std::function<void (const WorkspaceEvent &)> &callback)
{
  m_report_callback = callback;
}

}  // namespace suppvmonitor
