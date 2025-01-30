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

#include "local_domain_runner.h"

#include "domain_event_dispatcher_context.h"
#include "user_context.h"

#include <sup/oac-tree/local_job.h>

namespace oac_tree_gui
{

LocalDomainRunner::LocalDomainRunner(DomainEventDispatcherContext dispatcher_context,
                                     UserContext user_context,
                                     std::unique_ptr<procedure_t> procedure)
    : AbstractDomainRunner(std::move(dispatcher_context), std::move(user_context))
{
  SetDomainJob(std::make_unique<sup::oac_tree::LocalJob>(std::move(procedure), *GetJobInfoIO()));
}

}  // namespace oac_tree_gui
