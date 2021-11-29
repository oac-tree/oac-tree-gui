/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H
#define SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H

#include "sequencergui/monitor/job_types.h"

#include "mvvm/utils/threadsafestack.h"

namespace sequi
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

private:
  ModelView::threadsafe_stack<int> m_step_request;
  WaitingMode m_waiting_mode{WaitingMode::kProceed};
  int m_wait_msec{0};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_FLOWCONTROLLER_H
