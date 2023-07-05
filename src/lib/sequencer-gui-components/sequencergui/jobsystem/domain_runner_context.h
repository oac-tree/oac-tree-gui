/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_CONTEXT_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/job_types.h>

#include <functional>

namespace sequencergui
{

/**
 * @brief The DomainRunnerContext structure provides initial parameters for DomainRunnerAdapter to
 * run.
 */
struct DomainRunnerContext
{
  //! domain procedure to execute in a thread, must be after Setup call
  procedure_t* procedure{nullptr};

  //! sequencer user interface
  userinterface_t* user_interface{nullptr};

  //! callback to report when runner has changed status
  std::function<void(RunnerStatus)> runner_status_changed_cb;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_CONTEXT_H_
