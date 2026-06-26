/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
#define OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_

#include <set>
#include <string>
#include <cstdint>
#include <variant>

namespace oac_tree_gui
{

/**
 * @brief The AutomationServerType enum defines types of the remote automation server.
 */
enum class AutomationServerType : std::uint8_t
{
  kUnknown,    //!< unknown automation server type
  kEPICS,      //!< EPICS-based automation server
  kWebSockets,  //!< WebSocket-based automation server
};

/**
 * @brief The EPICSServerInfo struct holds info related to the EPICS automation server.
 */
struct EPICSServerInfo
{
  std::string server_name;
};

bool operator==(const EPICSServerInfo& lhs, const EPICSServerInfo& rhs);
bool operator!=(const EPICSServerInfo& lhs, const EPICSServerInfo& rhs);

/**
 * @brief The WebSocketsServerInfo class
 */
struct WebSocketsServerInfo
{
  std::string server_address;
  std::uint16_t server_port{};
};

bool operator==(const WebSocketsServerInfo& lhs, const WebSocketsServerInfo& rhs);
bool operator!=(const WebSocketsServerInfo& lhs, const WebSocketsServerInfo& rhs);

// Comparison of AutomationServerInfo is provided automatically by std::variant via the operators
// defined above for its alternatives.
using AutomationServerInfo = std::variant<EPICSServerInfo, WebSocketsServerInfo>;

/**
 * @brief Returns the name of the server used to establish a connection.
 *
 * For an EPICS server it is the server name as provided. For a WebSockets server it is the server
 * address combined with the port in the form "address:port".
 */
std::string GetServerName(const AutomationServerInfo& server_info);

/**
 * @brief The RemoteConnectionInfo struct holds information about remote jobs that should be
 * imported into the job system.
 */
struct RemoteConnectionInfo
{
  std::string server_name;            //!< the name of the server to connect
  std::set<std::size_t> job_indexes;  //!< list of job indices to import into the job system
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_INFO_H_
