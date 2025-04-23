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

#ifndef OAC_TREE_GUI_MODEL_RUNNER_STATUS_H_
#define OAC_TREE_GUI_MODEL_RUNNER_STATUS_H_

#include <cstdint>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The RunnerStatus enum defines possible states that a JobItem can have.
 *
 * The first 7 values reproduce the sequencer's domain sup::oac_tree::JobState, which are followed
 * by additional states that make sense for the GUI.
 */
enum class RunnerStatus : std::uint8_t
{
  kInitial,
  kPaused,
  kStepping,
  kRunning,
  kSucceeded,
  kFailed,
  kHalted,
  kUndefined,     //!< job either wasn't submitted, or does not have domain counterpart yet
  kSubmitFailure  //!< job submission has failed due to malformed procedure
};

/**
 * @brief Returns string representation of the RunerStatus.
 */
std::string ToString(RunnerStatus status);

/**
 * @brief Returns RunnerStatus from string representation.
 */
RunnerStatus GetRunnerStatus(const std::string& status_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_RUNNER_STATUS_H_
