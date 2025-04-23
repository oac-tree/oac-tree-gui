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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_REMOTE_DOMAIN_RUNNER_H_
#define OAC_TREE_GUI_JOBSYSTEM_REMOTE_DOMAIN_RUNNER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/jobsystem/abstract_domain_runner.h>

#include <sup/oac-tree-server/i_job_manager.h>

namespace oac_tree_gui
{

/**
 * @brief The RemoteDomainRunner class runs remotely the sequencer domain procedure.
 */
class RemoteDomainRunner : public AbstractDomainRunner
{
public:
  RemoteDomainRunner(DomainEventDispatcherContext dispatcher_context, UserContext user_context,
                     sup::oac_tree_server::IJobManager &manager, std::size_t job_index);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REMOTE_DOMAIN_RUNNER_H_
