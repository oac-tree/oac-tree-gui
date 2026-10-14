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
//! Collection of functions to create instruction descriptors from domain objects.

#include <oac_tree_gui/domain/instruction_descriptor.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_INSTRUCTION_DESCRIPTOR_HELPER_H_
