/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_WORKSPACE_EVENT_HELPER_H_
#define OAC_TREE_GUI_JOBSYSTEM_WORKSPACE_EVENT_HELPER_H_

//! @file
//! Helper functions to update workspace items from domain variable events.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

namespace sup::dto
{
class AnyValue;
}

namespace oac_tree_gui
{
class WorkspaceItem;
class VariableItem;
struct VariableUpdatedEvent;
}  // namespace oac_tree_gui

namespace oac_tree_gui
{

/**
 * @brief Checks if  GUI and domain workspaces match.
 *
 * Will return true, if a number of variables and their names coincide.
 */
bool AreMatchingWorkspaces(const WorkspaceItem& workspace_item,
                           const sup::oac_tree::Workspace& workspace);

/**
 * @brief Updates variable from domain event.
 */
void UpdateVariableFromEvent(const VariableUpdatedEvent& event, VariableItem& item);

/**
 * @brief Updates variable from domain event.
 */
void UpdateVariableFromEvent(const sup::dto::AnyValue& value, bool connected, VariableItem& item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_WORKSPACE_EVENT_HELPER_H_
