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

#ifndef OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform InstructionItem from/to domain instructions.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <memory>
#include <vector>

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The InstructionTree class contains a root instruction with all its children, and the
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
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates InstructionItem with all its children from the automation server information.
 */
InstructionTree CreateInstructionItemTree(const sup::oac_tree::InstructionInfo& info);

/**
 * @brief Creates InstructionItem with all its children from domain instruction.
 */
InstructionTree CreateInstructionItemTree(const sup::oac_tree::Instruction& instruction);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_INSTRUCTION_ITEM_TRANSFORM_HELPER_H_
