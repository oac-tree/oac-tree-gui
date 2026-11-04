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

#ifndef OAC_TREE_GUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
#define OAC_TREE_GUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

namespace oac_tree_gui
{
class WorkspaceItem;
}  // namespace oac_tree_gui

namespace oac_tree_gui
{

/**
 * @brief Populate empty domain workspace with the content in WorkspaceItem.
 *
 * @param item The GUI workspace item
 * @param workspace oac-tree domain workspace
 *
 * @note Domain workspace should be empty, will throw otherwise.
 */
void PopulateDomainWorkspace(const WorkspaceItem& item, workspace_t& workspace);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
