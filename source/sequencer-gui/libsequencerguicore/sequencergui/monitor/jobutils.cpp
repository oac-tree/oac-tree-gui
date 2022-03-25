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

#include "sequencergui/monitor/jobutils.h"

#include <map>

namespace
{

const std::map<sequencergui::RunnerStatus, std::string> kRunnerStatusMap = {
    {sequencergui::RunnerStatus::kIdle, "Idle"},
    {sequencergui::RunnerStatus::kRunning, "Running"},
    {sequencergui::RunnerStatus::kCompleted, "Completed"},
    {sequencergui::RunnerStatus::kCanceling, "Canceling"},
    {sequencergui::RunnerStatus::kCanceled, "Canceled"},
    {sequencergui::RunnerStatus::kFailed, "Failed"}};
} // namespace

namespace sequencergui
{

std::string RunnerStatusToString(RunnerStatus status)
{
  auto it = kRunnerStatusMap.find(status);
  return it == kRunnerStatusMap.end() ? std::string("Unknown") : it->second;
}

}  // namespace sequencergui
