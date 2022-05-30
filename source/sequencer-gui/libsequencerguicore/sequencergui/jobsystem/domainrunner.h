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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAINRUNNER_H
#define SEQUENCERGUI_JOBSYSTEM_DOMAINRUNNER_H

#include "sequencergui/jobsystem/runnerinterface.h"
#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/monitor/job_types.h"

#include <memory>
#include <functional>

namespace sequencergui
{

class FunctionRunner;

//! Runner to execute sequencer procedure in event loop.

class DomainRunner : public RunnerInterface
{
public:
  DomainRunner(procedure_t* procedure, userinterface_t* user_interface,
               std::function<void(RunnerStatus)> status_changed_callback);
  ~DomainRunner() override;

  bool Start() override;

  bool Stop() override;

  bool Pause() override;

  bool Step() override;

private:
  bool ExecuteSingle();

  procedure_t* m_procedure{nullptr};
  userinterface_t* m_user_interface{nullptr};
  std::unique_ptr<runner_t> m_domain_runner;
  std::unique_ptr<FunctionRunner> m_function_runner;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
