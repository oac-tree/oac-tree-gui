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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <mvvm/model/session_item.h>

namespace mvvm
{
class SessionItem;
}

namespace sequencergui
{
class WorkspaceItem;
class DomainWorkspaceBuilder;
class VariableItem;
}  // namespace sequencergui

namespace sequencergui
{

/**
 * @brief Populate empty domain workspace with the content in WorkspaceItem.
 *
 * @param item The GUI workspace item
 * @param workspace Sequencer domain workspace
 *
 * @note Domain workspace should be empty, will throw otherwise.
 */
void PopulateDomainWorkspace(const WorkspaceItem& item, workspace_t& workspace);

/**
 * @brief Mark variable properties as enabled or disabled depending on if workspace is idle or
 * running.
 *
 * @param is_running Flag denoting if workspace is running.
 * @param item Workspace item to update variables.
 *
 * @details When workspace is running (i.e. both domain and GUI workspaces are up and listen each
 * other), certain properties (i.e. name, channel) should be marked as readonly.
 */
void UpdateVariableEditableProperty(bool is_running, WorkspaceItem& item);

/**
 * @brief Provide initial setup of just created variable.
 *
 * @param item Variable item to setup.
 * @param total_variable_count Number of variables already existing in a workspace.
 *
 * @details Provide reasonable default name, and initial AnyValueItem if necessary.
 */
void SetupNewVariable(VariableItem* item, int total_variable_count = 0);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
