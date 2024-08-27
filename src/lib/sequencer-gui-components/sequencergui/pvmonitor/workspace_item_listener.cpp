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

#include "workspace_item_listener.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <sup/sequencer/workspace.h>

namespace sequencergui
{

WorkspaceItemListener::WorkspaceItemListener(WorkspaceItem* workspace_item,
                                             sup::sequencer::Workspace* domain_workspace)
    : m_workspace_item(workspace_item), m_domain_workspace(domain_workspace)
{
  if (!m_domain_workspace)
  {
    throw RuntimeException("Not initialised workspace");
  }

  if (!m_workspace_item)
  {
    throw RuntimeException("Not initialised workspace item");
  }

  if (!AreMatchingWorkspaces(*m_workspace_item, *m_domain_workspace))
  {
    throw RuntimeException("Domain and GUI workspace do not match");
  }

  m_listener = std::make_unique<mvvm::ModelListener<>>(workspace_item->GetModel());
  m_listener->Connect<mvvm::DataChangedEvent>(this, &WorkspaceItemListener::OnDataChangedEvent);
}

WorkspaceItemListener::~WorkspaceItemListener() = default;

void WorkspaceItemListener::OnDataChangedEvent(const mvvm::DataChangedEvent& event)
{
  // finding VariableItem which is a parent of the item with changed data
  for (auto variable_item : m_workspace_item->GetVariables())
  {
    if (mvvm::utils::IsItemAncestor(event.m_item, variable_item))
    {
      ProcessEventToDomain(variable_item);
      break;
    }
  }
}

void WorkspaceItemListener::ProcessEventToDomain(VariableItem* variable_item)
{
  if (!m_domain_workspace->IsSuccessfullySetup())
  {
    throw RuntimeException("Attempt to propagate changes to the domain workspace without setup");
  }
  m_domain_workspace->SetValue(variable_item->GetName(), GetAnyValue(*variable_item));
}

}  // namespace sequencergui
