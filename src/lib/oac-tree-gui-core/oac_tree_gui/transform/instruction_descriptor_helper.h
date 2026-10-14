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

#ifndef OAC_TREE_GUI_TRANSFORM_INSTRUCTION_DESCRIPTOR_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_INSTRUCTION_DESCRIPTOR_HELPER_H_

//! @file
//! Collection of functions to create instruction descriptors and nodes from domain objects.
//! The main idea is to handle all peculiarities of the domain (differences in domain attributes,
//! lack of default values, lack of info from automation server) here, and provide InstructionItem
//! with the unified source of information.

#include <oac_tree_gui/domain/instruction_descriptor.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <string>

namespace oac_tree_gui
{

/**
 * @brief Creates a typed descriptor from the attribute definitions of a domain instruction.
 *
 * Attribute presets are applied.
 *
 * @param instruction The domain instruction.
 * @return The instruction descriptor.
 * @throw RuntimeException if an attribute definition has a non-scalar type.
 */
InstructionDescriptor CreateInstructionDescriptor(const instruction_t& instruction);

/**
 * @brief Creates an info descriptor from the automation server information.
 *
 * Holds the attributes of the oac-tree base instruction classes only. Attribute presets are
 * applied. No plugin is needed.
 *
 * @param info The instruction information.
 * @return The instruction descriptor.
 */
InstructionDescriptor CreateInstructionDescriptor(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates a node for the given domain type, without values and children.
 *
 * A temporary domain instruction provides the typed descriptor.
 *
 * @param domain_type The domain type of the instruction.
 * @return The instruction node.
 * @throw sup::oac_tree::InvalidOperationException if the type is not registered.
 */
InstructionNode CreateInstructionNode(const std::string& domain_type);

/**
 * @brief Creates a node tree from a domain instruction and its children.
 *
 * Each node gets a typed descriptor and the string attributes of its instruction. Indexes follow
 * depth-first order, starting from 0 at the root.
 *
 * @param instruction The domain instruction.
 * @return The root node.
 * @throw RuntimeException if an attribute definition has a non-scalar type.
 */
InstructionNode CreateInstructionNodeTree(const instruction_t& instruction);

/**
 * @brief Creates a node tree from the automation server information.
 *
 * Each node gets an info descriptor, the attributes and the index of its info. No plugin is
 * needed.
 *
 * @param info The instruction information.
 * @return The root node.
 */
InstructionNode CreateInstructionNodeTree(const sup::oac_tree::InstructionInfo& info);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_INSTRUCTION_DESCRIPTOR_HELPER_H_
