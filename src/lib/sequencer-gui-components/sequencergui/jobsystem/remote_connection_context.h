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

#ifndef SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_CONTEXT_H_

#include <string>
#include <vector>

namespace sequencergui
{

/**
 * @brief The RemoteConnectionContext struct holds information about automation jobs that should be
 * imported into the job system.
 */
struct RemoteConnectionContext
{
  std::string server_name;        //!< the name of the server to connect
  std::vector<size_t> m_job_ids;  //!< list of automation jobs to import into the job system
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_CONTEXT_H_
