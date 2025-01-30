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

#include "remote_job_handler.h"

#include "abstract_domain_runner.h"
#include "domain_event_dispatcher_context.h"
#include "remote_domain_runner.h"

#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/procedure_item_job_info_builder.h>

#include <sup/gui/model/anyvalue_utils.h>

#include <sup/oac-tree/workspace.h>

#include <iostream>

namespace oac_tree_gui
{

RemoteJobHandler::RemoteJobHandler(JobItem *job_item, sup::oac_tree_server::IJobManager &manager,
                                   std::size_t job_index, UserContext user_context)
    : AbstractJobHandler(job_item)
{
  Setup(std::make_unique<RemoteDomainRunner>(CreateEventDispatcherContext(),
                                             std::move(user_context), manager, job_index));
}

RemoteJobHandler::~RemoteJobHandler() = default;

void RemoteJobHandler::OnVariableUpdatedEvent(const VariableUpdatedEvent &event)
{
  if (auto item = GetItemBuilder()->GetVariable(event.index); item)
  {
    if (event.connected && sup::dto::IsEmptyValue(event.value) && !item->IsAvailable())
    {
      item->SetIsAvailable(event.connected);
      return;
    }

    item->SetIsAvailable(event.connected);
    UpdateAnyValue(event.value, *item);
  }
  else
  {
    std::cout << "Can't find variable \n";
  }
}

}  // namespace oac_tree_gui
