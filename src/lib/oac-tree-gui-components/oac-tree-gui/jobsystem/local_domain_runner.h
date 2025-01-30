/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_

#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <oac-tree-gui/jobsystem/abstract_domain_runner.h>

namespace oac_tree_gui
{

/**
 * @brief The LocalDomainRunner class runs locally the sequencer domain procedure.
 *
 * Sequencer's IJob has to be created outside and moved here via SetJob method.
 */
class LocalDomainRunner : public AbstractDomainRunner
{
public:
  LocalDomainRunner(DomainEventDispatcherContext dispatcher_context, UserContext user_context,
                    std::unique_ptr<procedure_t> procedure);
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_
