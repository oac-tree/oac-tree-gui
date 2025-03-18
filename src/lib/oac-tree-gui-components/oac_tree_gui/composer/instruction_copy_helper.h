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

#ifndef OAC_TREE_GUI_COMPOSER_INSTRUCTION_COPY_HELPER_H_
#define OAC_TREE_GUI_COMPOSER_INSTRUCTION_COPY_HELPER_H_

#include <memory>
#include <vector>

class QMimeData;

namespace mvvm
{
class SessionItem;
}

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief Creates copy object for single instruction, children ignored.
 *
 * Instructions properties are included, children instructions are ignored.
 */
std::unique_ptr<QMimeData> CreateInstructionCopyMimeData(const InstructionItem& instruction);

/**
 * @brief Creates copy objecrt for single instruction, all children included.
 */
std::unique_ptr<QMimeData> CreateInstructionTreeCopyMimeData(const InstructionItem& instruction);

/**
 * @brief Creates copy object for selection tree.
 *
 * Selection tree is a list of instructions obtained when the user manually picks parent and child
 * instructions from a large instruction tree.
 *
 * Example:
 * Sequence  <- selected
 *   Wait
 *   Message <- selected
 *
 * Selection list [sequence, message] will generate copy object for top level "sequence" containing
 * a single child "message".
 */
std::unique_ptr<QMimeData> CreateInstructionSelectionCopyMimeData(
    const std::vector<InstructionItem*>& selection);

/**
 * @brief Returns vector of instructions from given mime data.
 */
std::vector<std::unique_ptr<mvvm::SessionItem>> CreateInstructions(const QMimeData* mime_data);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_COPY_HELPER_H_
