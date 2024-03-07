/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_RUNNER_STATUS_H_
#define SEQUENCERGUI_JOBSYSTEM_RUNNER_STATUS_H_

#include <string>

namespace sequencergui
{

/**
 * @brief The RunnerStatus enum defines possible states that runner can have during job execution.
 *
 * For the moment, it matches one-to-one sup::sequencer::JobStatus.
 */
enum class RunnerStatus
{
  kInitial,
  kPaused,
  kStepping,
  kRunning,
  kSucceeded,
  kFailed,
  kHalted
};

/**
 * @brief Returns string representation of the RunerStatus.
 */
std::string RunnerStatusToString(RunnerStatus status);

/**
 * @brief Returns RunnerStatus from string representation.
 */
RunnerStatus GetRunnerStatus(const std::string& status_name);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_TYPES_H_
