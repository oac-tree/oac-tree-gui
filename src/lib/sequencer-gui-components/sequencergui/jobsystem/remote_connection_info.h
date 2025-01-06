/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
#define SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_

#include <set>
#include <string>

namespace sequencergui
{

/**
 * @brief The RemoteConnectionInfo struct holds information about remote jobs that should be
 * imported into the job system.
 */
struct RemoteConnectionInfo
{
  std::string server_name;       //!< the name of the server to connect
  std::set<size_t> job_indexes;  //!< list of job indices to import into the job system
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
