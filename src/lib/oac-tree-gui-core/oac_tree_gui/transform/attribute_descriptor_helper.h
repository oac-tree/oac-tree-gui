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

#ifndef OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_DESCRIPTOR_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_DESCRIPTOR_HELPER_H_

//! @file
//! Collection of functions to create attribute descriptors of instructions and variables.

#include <oac_tree_gui/domain/attribute_descriptor.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <string>

namespace oac_tree_gui
{

/**
 * @brief Creates an attribute descriptor and applies the preset.
 *
 * Mandatory attributes, isRoot and strings are not nullable. GUI attributes (isRoot,
 * showCollapsed) and strings take no text. isRoot is not visible.
 *
 * @param domain_type The domain type of the instruction or variable.
 * @param name The name of the attribute.
 * @param default_value The scalar default value; its type defines the attribute type.
 * @param mandatory The mandatory flag of the attribute.
 * @param category The category of the attribute.
 * @return The attribute descriptor.
 */
AttributeDescriptor CreateAttributeDescriptor(const std::string& domain_type,
                                              const std::string& name,
                                              const mvvm::variant_t& default_value, bool mandatory,
                                              sup::oac_tree::AttributeCategory category);

/**
 * @brief Creates an attribute descriptor from the domain attribute definition.
 *
 * The default value is the default-initialized scalar of the definition type.
 *
 * @param domain_type The domain type of the instruction or variable.
 * @param definition The domain attribute definition.
 * @return The attribute descriptor.
 * @throw RuntimeException if the definition has a non-scalar type.
 */
AttributeDescriptor CreateAttributeDescriptor(const std::string& domain_type,
                                              const attribute_definition_t& definition);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_DESCRIPTOR_HELPER_H_
