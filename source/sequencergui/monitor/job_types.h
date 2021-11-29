/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef MVVM_MONITOR_JOB_TYPES_H
#define MVVM_MONITOR_JOB_TYPES_H

//! @file job_types.h
//! Provide conveniece types for procedure execution in the JobManager.

namespace sequi
{

enum class WaitingMode
{
  kProceed,         //! proceed without waiting
  kWaitForRelease,  //! wait for external "step" command
  kSleepFor         //! sleep certain amount of time
};

enum class RunnerStatus
{
  kIdle,       //! wasn't started yet
  kRunning,    //! running
  kCompleted,  //! completed
  kCanceling,  //! in the process of canceling (performed Halt of procedure)
  kCanceled,   //! was canceled
  kFailed      //! something went wrong in the domain runner
};

}  // namespace sequi

#endif  // MVVM_MONITOR_JOB_TYPES_H
