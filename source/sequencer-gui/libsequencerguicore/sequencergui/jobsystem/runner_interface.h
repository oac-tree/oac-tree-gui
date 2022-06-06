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
  virtual bool Start() = 0;

  //! Stops task execution.
  virtual bool Stop() = 0;

  //! Pause task execution at the next break point.
  virtual bool Pause() = 0;

  //! Step paused task forward till the next break point.
  virtual bool Step() = 0;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_RUNNERINTERFACE_H
