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

#ifndef OAC_TREE_GUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem from/to domain variables.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/domain/variable_descriptor.h>

#include <memory>
#include <string>
#include <vector>

namespace oac_tree_gui
{

class VariableItem;
class WorkspaceItem;

/**
 * @brief Creates a variable item for the given node.
 *
 * Attributes of the descriptor become attribute items in the attribute container. Attribute
 * values of the node are set; attributes unknown to the descriptor become string attribute items.
 * The JSON type and value attributes give the AnyValueItem; without type it is empty.
 *
 * @param node The variable node.
 * @param registry Optional type registry containing already existing domain types.
 * @return The variable item.
 */
std::unique_ptr<VariableItem> CreateVariableItem(const VariableNode& node,
                                                 const anytype_registry_t* registry = nullptr);

/**
 * @brief Creates a variable item for the given domain type, with the attributes of the type.
 *
 * @throw sup::oac_tree::InvalidOperationException if the type is not registered.
 */
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

/**
 * @brief Creates a domain variable from the given item.
 *
 * Attributes with a value, the name, and the JSON type and value if the AnyValue is not empty.
 *
 * @param item The variable item.
 * @return The domain variable.
 * @throw sup::oac_tree::InvalidOperationException if the domain type is not registered.
 */
std::unique_ptr<variable_t> CreateDomainVariable(const VariableItem& item);

/**
 * @brief Populates empty workspace workspace item with variables from the automation server
 * information.
 *
 * @param info Automation server representation of the domain workspace.
 * @param workspace_item An empty workspace item to populate with the content.
 *
 * @return List of all created variables stored according to their automation indexes
 */
std::vector<VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::WorkspaceInfo& info,
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
std::vector<VariableItem*> PopulateWorkspaceItem(const sup::oac_tree::Workspace& workspace,
                                                       const anytype_registry_t* registry,
                                                       WorkspaceItem* workspace_item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_VARIABLE_ITEM_TRANSFORM_HELPER_H_
