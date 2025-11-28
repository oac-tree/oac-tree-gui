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

#include "local_job_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/domain_event_dispatcher_context.h>  // IWYU pragma: keep
#include <oac_tree_gui/jobsystem/local_domain_runner.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/operation/operation_action_helper.h>
#include <oac_tree_gui/pvmonitor/workspace_item_listener.h>

#include <mvvm/signals/item_listener.h>

#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

LocalJobHandler::LocalJobHandler(JobItem* job_item, UserContext user_context)
    : AbstractJobHandler(job_item)
{
  SetupPropertyListener();

  auto domain_procedure = CreateDomainProcedure(*GetJobItem());
  auto domain_procedure_ptr = domain_procedure.get();

  auto runner = CreateDomainRunner(std::move(user_context), std::move(domain_procedure));
  Setup(std::move(runner));

  m_workspace_item_listener = std::make_unique<WorkspaceItemListener>(
      GetExpandedProcedure()->GetWorkspace(), &domain_procedure_ptr->GetWorkspace());
}

LocalJobHandler::~LocalJobHandler() = default;

void LocalJobHandler::OnVariableUpdatedEvent(const VariableUpdatedEvent& event)
{
  m_workspace_item_listener->ProcessEventFromDomain(event);
}

std::unique_ptr<AbstractDomainRunner> LocalJobHandler::CreateDomainRunner(
    UserContext user_context, std::unique_ptr<procedure_t> procedure)
{
  // LocalDomainRunner's internals call Setup on the domain procedure
  auto runner = std::make_unique<LocalDomainRunner>(CreateEventDispatcherContext(),
                                                    std::move(user_context), std::move(procedure));
  runner->SetTickTimeout(GetJobItem()->GetTickTimeout());
  return runner;
}

void LocalJobHandler::SetupPropertyListener()
{
  m_property_listener = std::make_unique<mvvm::ItemListener>(GetJobItem());
  auto on_event = [this](const mvvm::PropertyChangedEvent& event)
  {
    if (event.name == itemconstants::kTickTimeout)
    {
      GetDomainRunner()->SetTickTimeout(GetJobItem()->GetTickTimeout());
    }
  };
  m_property_listener->Connect<mvvm::PropertyChangedEvent>(on_event);
}

}  // namespace oac_tree_gui
