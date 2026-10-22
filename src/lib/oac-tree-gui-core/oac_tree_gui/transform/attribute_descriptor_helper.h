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
//! Collection of functions to create and find attribute descriptors of instructions and
//! variables.

#include <oac_tree_gui/domain/attribute_descriptor.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <algorithm>
#include <string>
#include <vector>

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

/**
 * @brief Finds the attribute descriptor with the given name.
 *
 * @tparam DescriptorT InstructionDescriptor or VariableDescriptor.
 * @param descriptor The descriptor with the list of attributes.
 * @param name The name of the attribute.
 * @return The attribute descriptor, or nullptr if not found.
 */
template <typename DescriptorT>
const AttributeDescriptor* FindAttributeDescriptor(const DescriptorT& descriptor,
                                                   const std::string& name)
{
  auto iter = std::find_if(descriptor.attributes.begin(), descriptor.attributes.end(),
                           [&name](const auto& attribute) { return attribute.name == name; });
  return iter == descriptor.attributes.end() ? nullptr : &*iter;
}

/**
 * @brief Returns the names of the attributes of the descriptor, in their order.
 *
 * @tparam DescriptorT InstructionDescriptor or VariableDescriptor.
 * @param descriptor The descriptor with the list of attributes.
 * @return The attribute names.
 */
template <typename DescriptorT>
std::vector<std::string> GetAttributeNames(const DescriptorT& descriptor)
{
  std::vector<std::string> result;
  result.reserve(descriptor.attributes.size());
  for (const auto& attribute : descriptor.attributes)
  {
    result.push_back(attribute.name);
  }
  return result;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_DESCRIPTOR_HELPER_H_
