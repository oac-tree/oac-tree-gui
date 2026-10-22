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

#ifndef OAC_TREE_GUI_DOMAIN_VARIABLE_DESCRIPTOR_H_
#define OAC_TREE_GUI_DOMAIN_VARIABLE_DESCRIPTOR_H_

//! @file
//! Descriptions of variable types and instances, used to build variable items.

#include <oac_tree_gui/domain/attribute_descriptor.h>

#include <sup/oac-tree/attribute_info.h>

#include <cstdint>
#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The VariableDescriptor struct describes a variable type.
 */
struct VariableDescriptor
{
  std::string domain_type;
  std::vector<AttributeDescriptor> attributes;
};

/**
 * @brief The VariableNode struct describes a variable instance.
 */
struct VariableNode
{
  VariableDescriptor descriptor;
  std::vector<sup::oac_tree::AttributeInfo> attributes;  //!< name/value strings
  std::uint32_t index{0};                                //!< automation index
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_VARIABLE_DESCRIPTOR_H_
