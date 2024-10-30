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

#ifndef SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/abstract_domain_runner.h>

namespace sequencergui
{

/**
 * @brief The LocalDomainRunner class runs locally the sequencer domain procedure.
 */
class LocalDomainRunner : public AbstractDomainRunner
{
public:
  explicit LocalDomainRunner(std::unique_ptr<procedure_t> procedure,
                             sup::sequencer::IJobInfoIO* info_io);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_LOCAL_DOMAIN_RUNNER_H_
