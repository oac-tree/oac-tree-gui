/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_JOBUTILS_H
#define SEQUENCERGUI_MONITOR_JOBUTILS_H

//! @file jobutils.h
//! Collection of various utility functions for job execution.

#include "sequencergui/monitor/job_types.h"

#include <string>

namespace sequencergui
{

//! Returns string representation of the RunerStatus.
std::string RunnerStatusToString(RunnerStatus status);

//! Get RunnerStatus from string representation.
RunnerStatus GetRunnerStatus(const std::string& status_name);

//! Returns default tick timeout in msec.
int GetDefaultTickTimeoutMsc();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOBUTILS_H
