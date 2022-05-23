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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACTRUNNER_H
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACTRUNNER_H

#include "sequencergui/jobsystem/runnerinterface.h"
#include "sequencergui/monitor/job_types.h"

#include <memory>

namespace sequencergui
{

class AbstractRunner : public RunnerInterface
{
public:
  AbstractRunner();
  ~AbstractRunner() override;

  void Start() override;

  void Stop() override;

  void Pause() override;

  void Step() override;

  bool IsInTransition() const override;

  RunnerStatus GetRunnerStatus() const;

private:
  struct AbstractRunnerImpl;
  std::unique_ptr<AbstractRunnerImpl> p_impl;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_RUNNERINTERFACE_H
