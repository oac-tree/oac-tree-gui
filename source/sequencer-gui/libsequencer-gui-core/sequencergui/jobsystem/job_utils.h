/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_

//! @file job_utils.h
//! Collection of various utility functions for job execution.

#include <sequencergui/jobsystem/job_types.h>

#include <string>
#include <vector>

namespace sequencergui
{

//! Returns string representation of the RunerStatus.
std::string RunnerStatusToString(RunnerStatus status);

//! Get RunnerStatus from string representation.
RunnerStatus GetRunnerStatus(const std::string& status_name);

//! Returns default tick timeout in msec.
int GetDefaultTickTimeoutMsc();

//! Returns reg-exp pattern for vector with labels.
//! example: {"INFO", "DEBUG"} -> "(INFO|DEBUG)"
std::string GetRegExpPattern(const std::vector<std::string>& data);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
