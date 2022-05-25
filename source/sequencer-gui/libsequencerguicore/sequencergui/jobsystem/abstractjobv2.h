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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOBV2_H
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOBV2_H

#include "sequencergui/monitor/job_types.h"

#include <memory>

namespace sequencergui
{

class JobStateInterface;

//! Abstract job to execute the arbitrary task via RunnerInterface.
//! The main responsibility of a class is to forward JobAction to the runner, preventing
//! contradicting actions.

class AbstractJobV2
{
public:
  AbstractJobV2();
  virtual ~AbstractJobV2();

  RunnerStatus GetStatus() const;

  bool PerformAction(JobAction action);

protected:
  virtual void SetStatus(RunnerStatus state);

private:

  virtual void StartRequest() {}
  virtual void PauseRequest() {}
  virtual void ReleaseRequest() {}
  virtual void StepRequest() {}
  virtual void StopRequest() {}

  RunnerStatus m_status{RunnerStatus::kIdle};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
