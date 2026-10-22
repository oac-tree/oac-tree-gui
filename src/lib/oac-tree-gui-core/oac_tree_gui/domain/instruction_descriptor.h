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

#ifndef OAC_TREE_GUI_DOMAIN_INSTRUCTION_DESCRIPTOR_H_
#define OAC_TREE_GUI_DOMAIN_INSTRUCTION_DESCRIPTOR_H_

//! @file
//! Descriptions of instruction types and instances, used to build instruction items.

#include <oac_tree_gui/domain/attribute_descriptor.h>

#include <sup/oac-tree/attribute_info.h>
#include <sup/oac-tree/instruction.h>

#include <cstdint>
#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The InstructionDescriptor struct describes an instruction type.
 */
struct InstructionDescriptor
{
  std::string domain_type;
  sup::oac_tree::Instruction::Category category{sup::oac_tree::Instruction::kAction};
  std::vector<AttributeDescriptor> attributes;
};

/**
 * @brief The InstructionNode struct describes an instruction instance with its children.
 */
struct InstructionNode
{
  InstructionDescriptor descriptor;
  std::vector<sup::oac_tree::AttributeInfo> attributes;  //!< name/value strings
  std::uint32_t index{0};                                //!< automation index
  std::vector<InstructionNode> children;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_INSTRUCTION_DESCRIPTOR_H_
