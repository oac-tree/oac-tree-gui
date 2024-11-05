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

#include "remote_job_handler.h"

#include "abstract_domain_runner.h"
#include "domain_event_dispatcher_context.h"
#include "remote_domain_runner.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/transform/anyvalue_item_transform_helper.h>
#include <sequencergui/transform/procedure_item_job_info_builder.h>

#include <mvvm/signals/item_listener.h>

#include <sup/sequencer/i_job.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <iostream>

namespace sequencergui
{

RemoteJobHandler::RemoteJobHandler(JobItem *job_item, const UserContext &user_context)
    : AbstractJobHandler(job_item)
{
  Setup(std::make_unique<RemoteDomainRunner>(CreateContext(), user_context));
}

void RemoteJobHandler::SetRemoteJob(std::unique_ptr<sup::sequencer::IJob> job)
{
  GetDomainRunner()->SetJob(std::move(job));
}

RemoteJobHandler::~RemoteJobHandler() = default;

sup::sequencer::IJobInfoIO *RemoteJobHandler::GetJobInfoIO()
{
  return GetDomainRunner() ? GetDomainRunner()->GetJobInfoIO() : nullptr;
}

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

}  // namespace sequencergui
