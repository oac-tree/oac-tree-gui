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

#include "workspace_monitor_helper.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/workspace.h>

namespace
{
//! Returns vector of attributes which should be marked as readonly when workspace is running.
std::vector<std::string> GetVariableReadonlyAttributesWhenRunning()
{
  return {sequencergui::domainconstants::kChannelAttribute,
          sequencergui::domainconstants::kNameAttribute};
}

std::string ProposeVariableName(const sequencergui::VariableItem &item)
{
  if (auto workspace = dynamic_cast<sequencergui::WorkspaceItem *>(item.GetParent()); workspace)
  {
    return "var" + std::to_string(workspace->GetVariableCount() - 1);
  }
  return {};
}

}  // namespace

namespace sequencergui
{

void PopulateDomainWorkspace(const WorkspaceItem &item, workspace_t &workspace)
{
  DomainWorkspaceBuilder builder;
  builder.PopulateDomainWorkspace(&item, &workspace);
}

void UpdateVariableEditableProperty(bool is_running, WorkspaceItem &item)
{
  static const auto attributes = GetVariableReadonlyAttributesWhenRunning();

  for (const auto &attr : attributes)
  {
    for (auto variable : item.GetVariables())
    {
      if (mvvm::utils::HasTag(*variable, attr))
      {
        variable->GetItem(attr)->SetEditable(!is_running);
      }
    }
  }
}

void SetupNewVariable(VariableItem *item)
{
  if (!item)
  {
    return;
  }

  item->SetName(ProposeVariableName(*item));
  // By default we always set scalar anyvalue to any VariableItem added to the WorkspaceItem.
  // If user wants something else, he has to start AnyValueEditor.
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *item);
}

}  // namespace sequencergui
