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
  //  auto adapter = [this](const mvvm::event_variant_t& event)
  //  {
  //    auto concrete_event = std::get<mvvm::DataChangedEvent>(event);
  //    OnDataChangedEvent(concrete_event.m_item, concrete_event.m_data_role);
  //  };

  //  m_model->GetEventHandler()->Connect<mvvm::DataChangedEvent>(adapter, m_slot.get());

  m_model->GetEventHandler()->Connect<mvvm::ItemInsertedEvent>(
      this, &WorkspaceItemController::OnModelEvent, m_slot.get());
}

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
void WorkspaceItemController::SetCallback(
    const std::function<void(const WorkspaceEvent&)>& callback)
{
  m_report_callback = callback;
}

void WorkspaceItemController::OnModelEvent(const mvvm::event_variant_t& event)
{
  if (m_report_callback)
  {
    auto concrete_event = std::get<mvvm::ItemInsertedEvent>(event);
    if (auto variable_item = dynamic_cast<sequencergui::VariableItem*>(concrete_event.m_parent))
    {
      auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
      m_report_callback({variable_item->GetName(), stored_anyvalue});
    }
  }
}

}  // namespace suppvmonitor
