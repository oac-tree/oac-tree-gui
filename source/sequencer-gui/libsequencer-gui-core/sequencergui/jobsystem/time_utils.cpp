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

#include "sequencergui/jobsystem/time_utils.h"

#include <thread>

namespace sequencergui
{

bool BusyWaitFor(std::function<bool()> runner, std::chrono::milliseconds timeout)
{
  const std::chrono::milliseconds timeout_precision_msec(10);
  const std::chrono::milliseconds wait_time(timeout);
  auto time_end = std::chrono::system_clock::now() + timeout;
  while (std::chrono::system_clock::now() < time_end)
  {
    if (!runner())
    {
      return true;
    }
    std::this_thread::sleep_for(timeout_precision_msec);
  }
  return false;
}

}  // namespace sequencergui
