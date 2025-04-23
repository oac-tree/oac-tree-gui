/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/oac-tree/instruction.h>

#include <memory>
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
  std::vector<const InstructionItem*> indexes;
};

/**
 * @brief Creates InstructionItem from the automation server information.
 *
 * This method ignores all possible children instructions.
 *
 * Method temporarily creates domain object and thus requires that plugins are loaded.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates InstructionInfoItem from the automation server information.
 *
 * The method ignores all possible children's instructions.
 *
 * InstructionInfoItem is a simplified instruction which doesn't have editable attributes and which
 * is used to represent the instruction tree obtained from JobInfo information during procedure
 * execution. The method doesn't need that plugins are properly loaded.
 */
std::unique_ptr<InstructionItem> CreateInstructionInfoItem(
    const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates InstructionItem with all its children from the automation server information.
 *
 * Method requires that all plugins are loaded.
 */
InstructionTree CreateInstructionItemTree(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates simplfied InstructionInfoItem tree from automation server information.
 *
 * Method doesn't require plugin load.
 */
InstructionTree CreateInstructionInfoItemTree(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates InstructionItem with all its children from domain instruction.
 *
 * Method requires that all plugins are loaded.
 */
InstructionTree CreateInstructionItemTree(const sup::oac_tree::Instruction& instruction);

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
