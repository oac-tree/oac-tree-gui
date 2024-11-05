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

#ifndef SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem from/to domain variables.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <vector>

namespace sequencergui
{

class InstructionItem;
class VariableItem;
class WorkspaceItem;

/**
 * @brief Creates VariableItem from the automation server information.
 */
std::unique_ptr<VariableItem> CreateVariableItem(const sup::sequencer::VariableInfo& info);

/**
 * @brief Populates empty workspace workspace item with variables from the automation server
 * information.
 *
 * @param info Automation server representation of the domain workspace.
 * @param workspace_item An empty workspace item to populate with the content.
 *
 * @return List of all created variables stored according to their automation indexes
 */
std::vector<const VariableItem*> PopulateWorkspaceItem(const sup::sequencer::WorkspaceInfo& info,
                                                       WorkspaceItem* workspace_item);

/**
 * @brief Populates empty workspace workspace item with variables from the domain workspace.
 *
 * @param info Domain workspace.
 * @param registry Possible registry containing reference to some types.
 * @param workspace_item An empty workspace item to populate with the content.
 *
 * @return List of all created variables stored according to their automation indexes
 */
std::vector<const VariableItem*> PopulateWorkspaceItem(const sup::sequencer::Workspace& workspace,
                                                       const anytype_registry_t* registry,
                                                       WorkspaceItem* workspace_item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_AUTOMATION_TRANSFORM_HELPER_H_
