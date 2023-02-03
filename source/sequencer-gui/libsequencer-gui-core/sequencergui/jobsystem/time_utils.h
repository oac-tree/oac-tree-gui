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

#ifndef SEQUENCERGUI_JOBSYSTEM_TIME_UTILS_H_
#define SEQUENCERGUI_JOBSYSTEM_TIME_UTILS_H_

//! Collection of time related utils.

#include <chrono>
#include <functional>

namespace sequencergui
{

const auto duration = [](auto time_interval)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(time_interval).count(); };

//! Will wait a given amount of msec for runner completion. Returns `true` is runner has finished
//! before the timeout, `false` otherwise. Internally has a precision of 10 msec.
bool BusyWaitFor(std::function<bool()> runner, std::chrono::milliseconds timeout);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_UTILS_H_
