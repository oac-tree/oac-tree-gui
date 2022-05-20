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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H
#define SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H

#include "sequencergui/monitor/job_types.h"
#include <memory>

namespace sequencergui
{

class AbstractJob;

class JobStateInterface
{
public:
  virtual ~JobStateInterface();

  virtual RunnerStatus GetStatus() = 0;

  virtual std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) = 0;
};

class IdleState : public JobStateInterface
{
public:

  RunnerStatus GetStatus() override;

  std::unique_ptr<JobStateInterface> Handle(JobAction action, AbstractJob *job) override;
};


}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOBSTATE_H
