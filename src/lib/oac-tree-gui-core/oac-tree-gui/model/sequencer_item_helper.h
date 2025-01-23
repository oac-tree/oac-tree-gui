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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_OAC_TREE_ITEM_HELPER_H_
#define SEQUENCERGUI_MODEL_OAC_TREE_ITEM_HELPER_H_

//! @file
//! Various auxiliary functions for sequencer items.

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

class InstructionItem;
class SequencerModel;

bool IsCompoundInstruction(const InstructionItem* instruction);

bool IsDecoratorInstruction(const InstructionItem* instruction);

/**
 * @brief Returns an item representing a name property, or nullptr if the given parent doesn't have
 * such property registered.
 */
mvvm::SessionItem* GetNameItem(const mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing a status property, or nullptr if the given parent doesn't
 * have such property registered.
 */
mvvm::SessionItem* GetStatusItem(const mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing a breakpoint property, or nullptr if the given parent doesn't
 * have such property registered.
 */
mvvm::SessionItem* GetBreakpointItem(const mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing is_available property, or nullptr if the given parent doesn't
 * have such property registered.
 */
mvvm::SessionItem* GetIsAvailableItem(const mvvm::SessionItem& parent);

/**
 * @brief Returns an item representing channel property, or nullptr if the given parent doesn't
 * have such property registered.
 */
mvvm::SessionItem* GetChannelItem(const mvvm::SessionItem& parent);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_OAC_TREE_ITEM_HELPER_H_
