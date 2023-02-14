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

#include "sequencergui/jobsystem/flow_controller.h"

#include <iostream>

namespace
{
//! Flag to put into multi-threading stack to trigger next step. Can be any value.
const int kMakeStepFlag{42};
}  // namespace

namespace sequencergui
{

void FlowController::SetWaitingMode(WaitingMode waiting_mode)
{
  if (waiting_mode == m_waiting_mode)
  {
    return;
  }
  auto prev_mode = m_waiting_mode;
  m_waiting_mode = waiting_mode;

  if (prev_mode == WaitingMode::kWaitForRelease)
  {
    StepRequest();
  }
}

//! Perform conditional waiting (blocking). Expecting to be called from worker thread.
//! Depending on m_waiting_mode can a) proceed without waiting b) wait certain amount of msec
//! 3) wait for StepRequest() call.

void FlowController::WaitIfNecessary()
{
  if (m_waiting_mode == WaitingMode::kProceed)
  {
    return;  // proceed without waiting
  }

  if (m_waiting_mode == WaitingMode::kSleepFor & m_wait_msec > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait_msec));
  }

  if (m_waiting_mode == WaitingMode::kWaitForRelease)
  {
    try
    {
      m_step_request.wait_and_pop();
    }
    catch (std::exception &ex)
    {
      std::cout << "Waiting for step request has been terminated\n";
    }
  }
}

//! Set flag to wait in WaitIfNecessary at the next occasion.
void FlowController::StepRequest()
{
  m_step_request.update_top(kMakeStepFlag);
}

void FlowController::Interrupt()
{
  SetWaitingMode(WaitingMode::kProceed);
  m_step_request.stop();  // stops waiting in m_step_request.wait_and_pop()
}

void FlowController::SetSleepTime(int msec)
{
  m_wait_msec = msec;
}

bool FlowController::IsPaused() const
{
  return m_waiting_mode == WaitingMode::kWaitForRelease;
}

}  // namespace sequencergui
