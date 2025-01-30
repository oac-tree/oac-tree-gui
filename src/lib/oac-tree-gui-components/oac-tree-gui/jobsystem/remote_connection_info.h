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

#ifndef OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
#define OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_

#include <set>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The RemoteConnectionInfo struct holds information about remote jobs that should be
 * imported into the job system.
 */
struct RemoteConnectionInfo
{
  std::string server_name;       //!< the name of the server to connect
  std::set<std::size_t> job_indexes;  //!< list of job indices to import into the job system
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
