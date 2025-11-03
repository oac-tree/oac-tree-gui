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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_STATUS_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_STATUS_H_

#include <cstdint>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The InstructionStatus enum defines possible execution states that a InstructionItem can
 * have.
 *
 * The first 5 values reproduce the sequencer's domain sup::oac_tree::ExecutionStatus, which are
 * followed by additional states that make sense for the GUI.
 */
enum class InstructionStatus : std::uint8_t
{
  kNotStarted,   //!< not started yet
  kNotFinished,  //!< started and waiting for the next tick
  kRunning,      //!< instruction or one of its descendants is running asynchroniously
  kSuccess,      //!< finished successfully
  kFailure,      //!< finished unsuccessfully
  kUndefined     //!< we do not receive any status updates from the domain
};

/**
 * @brief Returns string representation of the RunerStatus.
 */
std::string ToString(InstructionStatus status);

/**
 * @brief Returns RunnerStatus from string representation.
 */
InstructionStatus GetInstructionStatus(const std::string& status_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_STATUS_H_
