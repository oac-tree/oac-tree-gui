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
 * @brief Creates copy of instruction.
 *
 * Instructions properties are included, children ignored.
 */
std::unique_ptr<QMimeData> CreateInstructionCopyMimeData(const InstructionItem& instruction);

/**
 * @brief Creates copy of instruction, all children included.
 */
std::unique_ptr<QMimeData> CreateInstructionTreeCopyMimeData(const InstructionItem& instruction);

/**
 * @brief Returns vector of instructions from given mime data.
 */
std::vector<std::unique_ptr<mvvm::SessionItem>> CreateInstructions(const QMimeData* mime_data);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_COPY_HELPER_H_
