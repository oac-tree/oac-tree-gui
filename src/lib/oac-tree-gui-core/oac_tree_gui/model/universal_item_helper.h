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

#ifndef OAC_TREE_GUI_MODEL_UNIVERSAL_ITEM_HELPER_H_
#define OAC_TREE_GUI_MODEL_UNIVERSAL_ITEM_HELPER_H_

//! Helper functions to simplify construction of UniversalInstructionItem and UniversalVariableItem.

#include <mvvm/model/tagindex.h>

#include <memory>
#include <vector>

namespace mvvm
{
class ISessionModel;
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
 * @brief Convenience function to insert instruction aggregate.
 *
 * @param domain_type The name of ergistered aggregate type.
 * @param parent The parent to get a new child
 * @param parent Child position
 *
 * @return Inserted item.
 */
InstructionItem* InsertAggregate(const std::string& domain_type, mvvm::SessionItem* parent,
                                 const mvvm::TagIndex& tag_index = mvvm::TagIndex::Append());

/**
 * @brief Convenience function to insert instruction in another instruction.
 *
 * @param item Child to insert.
 * @param parent The parent to get a new child
 * @param parent Child position
 *
 * @return Inserted item.
 */
InstructionItem* InsertInstruction(std::unique_ptr<oac_tree_gui::InstructionItem> item,
                                   mvvm::SessionItem* parent,
                                   const mvvm::TagIndex& tag_index = mvvm::TagIndex::Append());

/**
 * @brief Returns true if given name correspond to the name of registered aggregate.
 */
bool IsAggregateName(const std::string& name);

/**
 * @brief Set input property to instruction item.
 */
void SetInput(const std::string& value, InstructionItem* item);

/**
 * @brief Set output property to instruction item.
 */
void SetOutput(const std::string& value, InstructionItem* item);

/**
 * @brief Set description property to instruction item.
 */
void SetDescription(const std::string& value, InstructionItem* item);

/**
 * @brief Set text attribute property to instruction item.
 */
void SetText(const std::string& value, InstructionItem* item);

/**
 * @brief Set variable name property to instruction item.
 */
void SetVariableName(const std::string& value, InstructionItem* item);

/**
 * @brief Checks if item can be collapsed or expanded.
 */
bool IsCollapsible(const InstructionItem& item);

/**
 * @brief Add property to show children as collapsed branch.
 *
 * @details This property will be added to all instructions which can have children. For
 * IncludeProcedure and Include instructions the value of this property will be put to true by
 * default.
 */
void AddShowCollapsedProperty(InstructionItem& item);

/**
 * @brief Returns true if item has is_collapsed property and it is set to true.
 *
 * This will be taken into account during operation to show instruction tree with certain children
 * branches collapsed.
 */
bool IsCollapsed(const InstructionItem& item);

/**
 * @brief Set collapsed status of instruction to a given value.
 *
 * Will do nothing, if  instruction is not collapsible.
 */
void SetCollapsed(bool value, InstructionItem& item);

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
sup::gui::AnyValueItem* GetAnyValueItem(const InstructionItem& item);

/**
 * @brief Creates a single instruction or instruction aggregate.
 *
 * @param name The type of single instruction, or the name of registered aggregate.
 * @return Constructed single instruction or instruction tree.
 */
std::unique_ptr<InstructionItem> CreateInstructionTree(const std::string& name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_UNIVERSAL_ITEM_HELPER_H_
