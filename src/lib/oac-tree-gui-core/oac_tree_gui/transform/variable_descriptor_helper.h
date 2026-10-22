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

#ifndef OAC_TREE_GUI_TRANSFORM_VARIABLE_DESCRIPTOR_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_VARIABLE_DESCRIPTOR_HELPER_H_

//! @file
//! Collection of functions to create variable descriptors and nodes from domain objects.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/domain/variable_descriptor.h>

#include <string>

namespace oac_tree_gui
{

/**
 * @brief Creates a typed descriptor from the attribute definitions of a domain variable.
 *
 * No attribute accepts text.
 *
 * @param variable The domain variable.
 * @return The variable descriptor.
 * @throw RuntimeException if an attribute definition has a non-scalar type.
 */
VariableDescriptor CreateVariableDescriptor(const variable_t& variable);

/**
 * @brief Creates an info descriptor from the automation server information.
 *
 * Holds the domain type only. No plugin is needed.
 *
 * @param info The variable information.
 * @return The variable descriptor.
 */
VariableDescriptor CreateVariableDescriptor(const sup::oac_tree::VariableInfo& info);

/**
 * @brief Creates a node for the given domain type, without values.
 *
 * A temporary domain variable provides the typed descriptor.
 *
 * @param domain_type The domain type of the variable.
 * @return The variable node.
 * @throw sup::oac_tree::InvalidOperationException if the type is not registered.
 */
VariableNode CreateVariableNode(const std::string& domain_type);

/**
 * @brief Creates a node from a domain variable: typed descriptor and string attributes.
 *
 * @param variable The domain variable.
 * @return The variable node.
 * @throw RuntimeException if an attribute definition has a non-scalar type.
 */
VariableNode CreateVariableNode(const variable_t& variable);

/**
 * @brief Creates a node from the automation server information.
 *
 * The node gets an info descriptor, the attributes and the index of the info. The JSON type and
 * value attributes are kept only if kProvideVariableInfoInitialValue is set. No plugin is needed.
 *
 * @param info The variable information.
 * @return The variable node.
 */
VariableNode CreateVariableNode(const sup::oac_tree::VariableInfo& info);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_VARIABLE_DESCRIPTOR_HELPER_H_
