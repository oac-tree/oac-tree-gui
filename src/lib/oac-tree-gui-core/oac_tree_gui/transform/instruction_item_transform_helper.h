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

#ifndef OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform InstructionItem from/to domain instructions.

#include <oac_tree_gui/domain/instruction_descriptor.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/oac-tree/instruction.h>

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class CompoundItem;
}  // namespace mvvm

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The InstructionTree struct contains a root instruction with all its children, and the
 * correspondence of automation index to instruction pointer.
 *
 * It is obtained from sup::oac_tree_server::InstructionInfo.
 */
struct InstructionTree
{
  std::unique_ptr<InstructionItem> root;  //!< root instruction

  //!< all instructions stored according their automation indexes
  std::vector<InstructionItem*> indexes;
};

/**
 * @brief Creates an instruction item tree from the given node tree.
 *
 * No domain objects are created and no plugin is needed.
 *
 * @param node The root node.
 * @return The root item and the items by automation index.
 * @throw RuntimeException if a node index is out of range.
 */
InstructionTree CreateInstructionItemTree(const InstructionNode& node);

/**
 * @brief Creates an instruction item for the given domain type.
 *
 * A temporary domain instruction provides the attribute definitions.
 *
 * @param domain_type The domain type of the instruction.
 * @return The instruction item.
 * @throw sup::oac_tree::InvalidOperationException if the type is not registered.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

/**
 * @brief Creates a domain instruction from the given item, without children.
 *
 * Exposed attributes go to the domain. The display name becomes the domain name, unless it is the
 * domain type.
 *
 * @param item The instruction item.
 * @return The domain instruction.
 * @throw sup::oac_tree::InvalidOperationException if the domain type is not registered.
 */
std::unique_ptr<instruction_t> CreateDomainInstruction(const InstructionItem& item);

/**
 * @brief Register tag in instruction item to be able to add other instructions as children.
 *
 * @param instruction The domain instruction is used to deduce category (compound, decorator).
 * @param item An item where tag will be registered.
 */
void RegisterChildrenTag(const instruction_t& instruction, mvvm::CompoundItem& item);

/**
 * @brief Register tag in instruction item to be able to add other instructions as children.
 *
 * @param category The category of the domain instruction.
 * @param item An item where tag will be registered.
 */
void RegisterChildrenTag(instruction_t::Category category, mvvm::CompoundItem& item);

/**
 * @brief Registers show collapsed property.
 *
 * Domain category is used to decide if show_collapsed property is necessary. Domain type is used to
 * guess what can be the default value.
 *
 * @param category Domain instruction category.
 * @param domain_type The type of the domain instruction
 * @param item An item where to register property.
 */
void RegisterShowCollapsedProperty(instruction_t::Category category, const std::string& domain_type,
                                   mvvm::CompoundItem& item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_
