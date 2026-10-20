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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_HELPER_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_HELPER_H_

//! @file
//! Helper functions for InstructionItem.

#include <mvvm/model/tagindex.h>

#include <vector>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

class InstructionItem;
class InstructionContainerItem;

/**
 * @brief Convenience function to insert instruction in another instruction.
 *
 * @param domain_type The domain instruction type.
 * @param parent The parent to get a new child
 * @param parent Child position
 *
 * @return Inserted item.
 */
InstructionItem* InsertInstruction(const std::string& domain_type, mvvm::SessionItem* parent,
                                   const mvvm::TagIndex& tag_index = mvvm::TagIndex::Append());

/**
 * @brief Checks if item can be collapsed or expanded.
 */
bool IsCollapsible(const InstructionItem& item);

/**
 * @brief Returns true if the children branch of the instruction is shown collapsed.
 *
 * @details Takes showCollapsed. If it is not set, Include and IncludeProcedure are collapsed, other
 * instructions are expanded.
 */
bool IsCollapsed(const InstructionItem& item);

/**
 * @brief Set collapsed status of instruction to a given value.
 *
 * Will do nothing, if  instruction is not collapsible.
 */
void SetCollapsed(bool value, InstructionItem& item);

/**
 * @brief Toggles collapsed status of instruction.
 */
void ToggleCollapsed(InstructionItem& item);

/**
 * @brief Returns vector of instructions which wants their children's branch shown as collapsed.
 */
std::vector<const InstructionItem*> GetCollapsedItems(const InstructionContainerItem& container);

/**
 * @brief Returns AnyValueItem associated with given instruction, or nullptr if no such item exist.
 *
 * @details This method checks if corresponding tag has been registered and contain AnyValueItem.
 * For the moment PvAccessWriteItem, ChannelAccessWriteItem and RPCClientInstructionItem
 * might cary AnyValueItem representing json type and value attributes.
 */
const sup::gui::AnyValueItem* GetAnyValueItem(const InstructionItem& item);

sup::gui::AnyValueItem* GetAnyValueItem(InstructionItem& item);

/**
 * @brief Checks if the given instruction represents a compound with multiple children.
 */
bool IsCompoundInstruction(const InstructionItem* instruction);

/**
 * @brief Checks if the given instruction represents a decorator instruction.
 */
bool IsDecoratorInstruction(const InstructionItem* instruction);

/**
 * @brief Returns an item representing is_root property, or nullptr if the given instruction doesn't
 * have such property registered.
 */
const mvvm::SessionItem* GetIsRootItem(const InstructionItem& item);

mvvm::SessionItem* GetIsRootItem(InstructionItem& item);

/**
 * @brief Returns the instruction owning the given property item, or nullptr.
 *
 * @param property A property item of the instruction, or an item inside its attribute container.
 * @return The instruction above the property.
 */
const InstructionItem* GetInstructionFromProperty(const mvvm::SessionItem& property);

InstructionItem* GetInstructionFromProperty(mvvm::SessionItem& property);

/**
 * @brief Checks if the instruction can be considered as potential root of the procedure.
 *
 * The instruction is considered as potential root it is a top instruction in instruction container.
 */
bool IsPotentialRootInstruction(const InstructionItem& instruction);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_HELPER_H_
