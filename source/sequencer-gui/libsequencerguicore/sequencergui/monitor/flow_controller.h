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

#ifndef SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H
#define SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H

#include "sequencergui/jobsystem/job_types.h"

#include "mvvm/utils/threadsafe_stack.h"

namespace sequencergui
{

//! Provides waiting logic for ProcedureRunner.
//! The ::SetWaitingMode allows to switch between 3 different modes:
//! a) Proceed without waiting
//! b) Wait certain amount of msec
//! c) Wait for extra call StepRequest()
class FlowController
{
public:
  void SetWaitingMode(WaitingMode waiting_mode);

  void WaitIfNecessary();

  void StepRequest();

  void Interrupt();

  void SetSleepTime(int msec);

  bool IsPaused() const;

private:
  mvvm::threadsafe_stack<int> m_step_request;
  WaitingMode m_waiting_mode{WaitingMode::kProceed};
  int m_wait_msec{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H
