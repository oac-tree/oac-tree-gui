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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_BEHAVIOR_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_BEHAVIOR_H_

#include <cstdint>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The InstructionBehavior enum defines possible behaviors of InstructionItem during
 * execution.
 */
enum class InstructionBehavior : std::uint8_t
{
  kNative,   //!< instruction behaves according to the implementation
  kHidden,   //!< instruction is invisible and is not executed
  kSucceed,  //!< instruction always succeeds
  kFail      //!< instruction always fails
};

/**
 * @brief Returns string representation of the InstructionBehavior.
 */
std::string ToString(InstructionBehavior behavior);

/**
 * @brief Returns InstructionBehavior from string representation.
 */
InstructionBehavior GetInstructionBehavior(const std::string& behavior_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_BEHAVIOR_H_
