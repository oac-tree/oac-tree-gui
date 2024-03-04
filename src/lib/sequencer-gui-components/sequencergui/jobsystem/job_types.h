/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_TYPES_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_TYPES_H_

//! Provide conveniece types for procedure execution in the JobManager.

namespace sequencergui
{

//! Defines possible states that runner can have during job execution.
enum class RunnerStatus
{
  kIdle,       //! wasn't started yet
  kRunning,    //! running
  kPaused,     //! paused
  kCompleted,  //! completed
  kStopping,   //! in the process of stopping (performed Halt of procedure)
  kStopped,    //! was stopped
  kFailed      //! something went wrong in the domain runner
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_TYPES_H_
