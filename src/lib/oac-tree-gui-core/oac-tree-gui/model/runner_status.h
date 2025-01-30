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

#ifndef OAC_TREE_GUI_MODEL_RUNNER_STATUS_H_
#define OAC_TREE_GUI_MODEL_RUNNER_STATUS_H_

#include <cstdint>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The RunnerStatus enum defines possible states that a runner can have during job execution.
 *
 * It reproduces the sequencer's domain JobStatus, except kUndefined when the domain doesn't exist
 * yet, or its status wasn't yet reported.
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
  kUndefined
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
