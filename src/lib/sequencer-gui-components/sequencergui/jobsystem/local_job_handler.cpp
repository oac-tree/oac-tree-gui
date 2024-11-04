/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "local_job_handler.h"

#include "local_domain_runner.h"
#include "domain_event_dispatcher_context.h"

#include <sequencergui/model/job_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/pvmonitor/workspace_item_listener.h>
#include <sequencergui/transform/domain_procedure_builder.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

namespace sequencergui
{

LocalJobHandler::LocalJobHandler(JobItem *job_item, const UserContext &user_context)
    : AbstractJobHandler(job_item)
{
  auto domain_procedure = DomainProcedureBuilder::CreateProcedure(*GetJobItem()->GetProcedure());
  auto domain_procedure_ptr = domain_procedure.get();

  // LocalDomainRunner's internals call Setup on the domain procedure
  auto dispatcher_context = CreateContext();
  auto runner = std::make_unique<LocalDomainRunner>(dispatcher_context, user_context,
                                                    std::move(domain_procedure));
  runner->SetTickTimeout(GetJobItem()->GetTickTimeout());

  Setup(std::move(runner));

  m_workspace_item_listener = std::make_unique<WorkspaceItemListener>(
      GetExpandedProcedure()->GetWorkspace(), &domain_procedure_ptr->GetWorkspace());
}

void LocalJobHandler::OnVariableUpdatedEvent(const VariableUpdatedEvent &event)
{
  m_workspace_item_listener->ProcessEventFromDomain(event);
}

LocalJobHandler::~LocalJobHandler() = default;

}  // namespace sequencergui
