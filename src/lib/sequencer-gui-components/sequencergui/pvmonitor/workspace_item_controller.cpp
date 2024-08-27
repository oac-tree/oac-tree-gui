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

#include "workspace_item_controller.h"

#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/signals/model_listener.h>

#include <iostream>
#include <stdexcept>

namespace sequencergui
{

WorkspaceItemController::WorkspaceItemController(WorkspaceItem* item)
    : m_workspace_item(item), m_listener(std::make_unique<mvvm::ModelListener<>>(item->GetModel()))
{
  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &WorkspaceItemController::OnItemInsertedEvent);
  m_listener->Connect<mvvm::DataChangedEvent>(this, &WorkspaceItemController::OnDataChangedEvent);
}

WorkspaceItemController::~WorkspaceItemController() = default;

//! Processes an event coming from sequencer workspace.

void WorkspaceItemController::ProcessEventFromDomain(const WorkspaceEvent& event)
{
  m_block_update_to_domain[event.variable_name] = true;

  if (auto item = GeVariableItemForName(event.variable_name); item)
  {
    if (event.connected && sup::dto::IsEmptyValue(event.value) && !item->IsAvailable())
    {
      item->SetIsAvailable(event.connected);
      return;
    }

    item->SetIsAvailable(event.connected);
    UpdateAnyValue(event.value, *item);
  }
  else
  {
    std::cout << "Can't find variable [" << event.variable_name << "]" << std::endl;
  }

  m_block_update_to_domain[event.variable_name] = false;
}

VariableItem* WorkspaceItemController::GeVariableItemForName(const std::string& name)
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

WorkspaceItem* WorkspaceItemController::GetWorkspaceItem()
{
  return m_workspace_item;
}

//! Sets the callback to report GUI events.

void WorkspaceItemController::SetCallback(
    const std::function<void(const WorkspaceEvent&)>& callback)
{
  m_report_callback = callback;
}

//! Process data changed event.
void WorkspaceItemController::OnDataChangedEvent(const mvvm::DataChangedEvent& event)
{
  // finding VariableItem which is a parent of the item with changed data
  for (auto variable_item : GetWorkspaceItem()->GetVariables())
  {
    if (mvvm::utils::IsItemAncestor(event.m_item, variable_item))
    {
      ProcessEventToDomain(variable_item);
      break;
    }
  }
}

//! Process model insert event.
void WorkspaceItemController::OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event)
{
  if (!m_report_callback)
  {
    return;
  }

  // If parent is VariableItem, then insert event denotes that AnyValueItem has been regenerated.
  if (auto variable_item = dynamic_cast<VariableItem*>(event.m_item))
  {
    ProcessEventToDomain(variable_item);
  }
}

//! Processes an event in WorkspaceItem, and, if necessary, send it to the domain via callback
//! provided.

void WorkspaceItemController::ProcessEventToDomain(VariableItem* variable_item)
{
  if (!m_report_callback)
  {
    return;
  }

  // do not send an event if it was initially triggered from the domain
  if (m_block_update_to_domain[variable_item->GetName()])
  {
    return;
  }

  // generate AnyValue from current AnyValueItem

  if (variable_item->GetAnyValueItem())
  {
    m_report_callback({variable_item->GetName(), GetAnyValue(*variable_item)});
  }
}

}  // namespace sequencergui
