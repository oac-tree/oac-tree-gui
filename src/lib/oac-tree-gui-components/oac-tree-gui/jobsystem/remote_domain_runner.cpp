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

#include "remote_domain_runner.h"

#include "domain_event_dispatcher_context.h"
#include "user_context.h"

#include <sup/auto-server/client_job.h>
#include <sup/auto-server/epics_config_utils.h>

namespace sequencergui
{

RemoteDomainRunner::RemoteDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                       UserContext user_context,
                                       sup::auto_server::IJobManager &manager,
                                       std::size_t job_index)
    : AbstractDomainRunner(std::move(dispatcher_context), std::move(user_context))
{
  auto remote_job = sup::auto_server::CreateClientJob(
      manager, job_index, sup::auto_server::utils::CreateEPICSIOClient, *GetJobInfoIO());

  SetDomainJob(std::move(remote_job));
}

}  // namespace sequencergui
