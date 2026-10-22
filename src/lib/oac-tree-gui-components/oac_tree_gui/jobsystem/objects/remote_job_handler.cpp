/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "remote_job_handler.h"

#include <oac_tree_gui/jobsystem/domain_event_dispatcher_context.h>  // IWYU pragma: keep
#include <oac_tree_gui/jobsystem/remote_domain_runner.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/procedure_item_job_info_builder.h>

namespace oac_tree_gui
{

RemoteJobHandler::RemoteJobHandler(JobItem* job_item,
                                   sup::oac_tree_server::IClientJobManager& manager,
                                   std::size_t job_index, UserContext user_context)
    : AbstractJobHandler(job_item)
{
  Setup(std::make_unique<RemoteDomainRunner>(CreateEventDispatcherContext(),
                                             std::move(user_context), manager, job_index));
}

RemoteJobHandler::~RemoteJobHandler() = default;

void RemoteJobHandler::OnVariableUpdatedEvent(const VariableUpdatedEvent& event)
{
  if (auto item = GetItemBuilder()->GetVariable(event.index); item)
  {
    UpdateVariableFromEvent(event, *item);
  }
}

}  // namespace oac_tree_gui
