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

#include "transform_from_domain.h"

#include <oac_tree_gui/core/exceptions.h>

#include <map>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Creates mapping between domain JobState and GUI RunnerStatus.
 */
std::map<sup::oac_tree::JobState, RunnerStatus> CreateRunnerStatusMap()
{
  using sup::oac_tree::JobState;
  const std::map<sup::oac_tree::JobState, RunnerStatus> result = {
      {JobState::kInitial, RunnerStatus::kInitial},
      {JobState::kPaused, RunnerStatus::kPaused},
      {JobState::kStepping, RunnerStatus::kStepping},
      {JobState::kRunning, RunnerStatus::kRunning},
      {JobState::kSucceeded, RunnerStatus::kSucceeded},
      {JobState::kFailed, RunnerStatus::kFailed},
      {JobState::kHalted, RunnerStatus::kHalted}};
  return result;
}

/**
 * @brief Creates mapping between domain ExecutionStatus and GUI InstructionStatus.
 */
std::map<sup::oac_tree::ExecutionStatus, InstructionStatus> CreateInstructionStatusMap()
{
  using sup::oac_tree::ExecutionStatus;
  const std::map<sup::oac_tree::ExecutionStatus, InstructionStatus> result = {
      {ExecutionStatus::NOT_STARTED, InstructionStatus::kNotStarted},
      {ExecutionStatus::NOT_FINISHED, InstructionStatus::kNotFinished},
      {ExecutionStatus::RUNNING, InstructionStatus::kRunning},
      {ExecutionStatus::SUCCESS, InstructionStatus::kSuccess},
      {ExecutionStatus::FAILURE, InstructionStatus::kFailure},
      {ExecutionStatus::HALTED, InstructionStatus::kHalted}};
  return result;
}

}  // namespace

RunnerStatus GetRunnerStatusFromDomain(sup::oac_tree::JobState job_state)
{
  static const auto kRunnerStatusMap = CreateRunnerStatusMap();
  auto iter = kRunnerStatusMap.find(job_state);
  if (iter == kRunnerStatusMap.end())
  {
    throw RuntimeException("Can't convert sup::oac_tree::JobState");
  }
  return iter->second;
}

InstructionStatus GetInstructionStatusFromDomain(sup::oac_tree::ExecutionStatus exec_status)
{
  static const auto kInstructionStatusMap = CreateInstructionStatusMap();
  auto iter = kInstructionStatusMap.find(exec_status);
  if (iter == kInstructionStatusMap.end())
  {
    throw RuntimeException("Can't convert sup::oac_tree::ExecutionStatus");
  }
  return iter->second;
}

}  // namespace oac_tree_gui
