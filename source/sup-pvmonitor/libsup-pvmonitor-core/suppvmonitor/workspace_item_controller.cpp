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
#include <mvvm/signals/model_event_handler.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_event.h>

#include <QDebug>
#include <stdexcept>

namespace suppvmonitor
{

WorkspaceItemController::WorkspaceItemController(MonitorModel* model)
    : m_model(model), m_slot(std::make_unique<mvvm::Slot>())
{
  m_model->GetEventHandler()->Connect<mvvm::ItemInsertedEvent>(
      this, &WorkspaceItemController::OnModelEvent, m_slot.get());
}

//! Process an event coming from sequencer workspace

void WorkspaceItemController::ProcessEventFromDomain(const WorkspaceEvent& event)
{
  m_block_update_to_domain[event.m_variable_name] = true;

  if (auto item = GeVariableItemForName(event.m_variable_name); item)
  {
    sequencergui::UpdateAnyValue(event.m_value, *item);
  }

  m_block_update_to_domain[event.m_variable_name] = false;
}

sequencergui::VariableItem* WorkspaceItemController::GeVariableItemForName(const std::string& name)
{
  if (!GetWorkspaceItem())
  {
    throw std::logic_error("No WorkspaceItem exists");
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

//! Returns WorkspaceItem from the model.

sequencergui::WorkspaceItem* WorkspaceItemController::GetWorkspaceItem()
{
  return mvvm::utils::GetTopItem<sequencergui::WorkspaceItem>(m_model);
}

//! Sets the callback to report GUI events.

void WorkspaceItemController::SetCallback(
    const std::function<void(const WorkspaceEvent&)>& callback)
{
  m_report_callback = callback;
}

//! Process event in MonitorModel. Currently handles ItemInsertedEvents only.

void WorkspaceItemController::OnModelEvent(const mvvm::event_variant_t& event)
{
  if (m_report_callback)
  {
    auto concrete_event = std::get<mvvm::ItemInsertedEvent>(event);

    // ItemInsertedEvents is a sign that AnyValueItem has been regenerated.
    if (auto variable_item = dynamic_cast<sequencergui::VariableItem*>(concrete_event.m_parent))
    {
      ProcessEventToDomain(variable_item);
    }
  }
}

//! Processes an event in WorkspaceItem, and, if necessary, send it to the domain via callback
//! provided.

void WorkspaceItemController::ProcessEventToDomain(sequencergui::VariableItem* variable_item)
{
  // do not send an event if it was initially triggered from the domain
  if (m_block_update_to_domain[variable_item->GetName()])
  {
    return;
  }

  // generate AnyValue from current AnyValueItem
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
  m_report_callback({variable_item->GetName(), stored_anyvalue});
}

}  // namespace suppvmonitor
