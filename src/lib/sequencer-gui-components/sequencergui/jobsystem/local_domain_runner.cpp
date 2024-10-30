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

#include "local_domain_runner.h"

#include <sup/sequencer/local_job.h>

namespace sequencergui
{

LocalDomainRunner::LocalDomainRunner(std::unique_ptr<procedure_t> procedure,
                                     sup::sequencer::IJobInfoIO *info_io)
    : AbstractDomainRunner(
          std::make_unique<sup::sequencer::LocalJob>(std::move(procedure), *info_io))
{
}

}  // namespace sequencergui
