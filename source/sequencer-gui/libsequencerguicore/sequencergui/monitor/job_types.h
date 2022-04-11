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

#ifndef SEQUENCERGUI_MONITOR_JOB_TYPES_H
#define SEQUENCERGUI_MONITOR_JOB_TYPES_H

//! @file job_types.h
//! Provide conveniece types for procedure execution in the JobManager.

namespace sequencergui
{

//! Defines possible execution modes of ProcedureRunner.
enum class WaitingMode
{
  kProceed,         //! proceed without waiting
  kWaitForRelease,  //! wait for external "step" command
  kSleepFor         //! sleep certain amount of time
};

//! Defines possible states that runner can have during job execution.
enum class RunnerStatus
{
  kIdle,       //! wasn't started yet
  kRunning,    //! running
  kPaused,     //! paused
  kCompleted,  //! completed
  kCanceling,  //! in the process of canceling (performed Halt of procedure)
  kCanceled,   //! was canceled
  kFailed      //! something went wrong in the domain runner
};

//! Defines message types that JobLog is aware of.
enum class JobMessageType
{
  kDefault,
  kSuccess,
  kHighlight,
  kWarning,
  kError
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOB_TYPES_H
