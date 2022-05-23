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

#ifndef SEQUENCERGUI_JOBSYSTEM_RUNNERINTERFACE_H
#define SEQUENCERGUI_JOBSYSTEM_RUNNERINTERFACE_H

namespace sequencergui
{

//! Interface class to run arbitrary task in a thread.
//! Provide possibility to start, pause, stop job and make step-by-step execution.
class RunnerInterface
{
public:
  virtual ~RunnerInterface() = default;

  //! Starts task execution.
  virtual void Start() = 0;

  //! Stops task execution.
  virtual void Stop() = 0;

  //! Pause task execution at the next break point.
  virtual void Pause() = 0;

  //! Step paused task forward till the next break point.
  virtual void Step() = 0;

  //! The runner is in the transition stage (i.e. `stopping`, after the stop was called till the
  //! moment it is actually stopped.
  virtual bool IsInTransition() const = 0;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_RUNNERINTERFACE_H
