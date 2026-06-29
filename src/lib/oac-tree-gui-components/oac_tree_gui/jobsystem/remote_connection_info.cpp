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

#include "remote_connection_info.h"

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{

bool operator==(const EPICSServerInfo& lhs, const EPICSServerInfo& rhs)
{
  return lhs.server_name == rhs.server_name;
}

bool operator!=(const EPICSServerInfo& lhs, const EPICSServerInfo& rhs)
{
  return !(lhs == rhs);
}

bool operator==(const WebSocketsServerInfo& lhs, const WebSocketsServerInfo& rhs)
{
  return lhs.server_address == rhs.server_address && lhs.server_port == rhs.server_port;
}

bool operator!=(const WebSocketsServerInfo& lhs, const WebSocketsServerInfo& rhs)
{
  return !(lhs == rhs);
}

std::string GetServerName(const AutomationServerInfo& server_info)
{
  if (std::holds_alternative<EPICSServerInfo>(server_info))
  {
    return std::get<EPICSServerInfo>(server_info).server_name;
  }

  const auto& info = std::get<WebSocketsServerInfo>(server_info);
  return info.server_address + ":" + std::to_string(info.server_port);
}

AutomationServerInfo GetAutomationServerInfo(const std::string& server_name)
{
  const auto parts = mvvm::utils::SplitString(server_name, ":");
  if (parts.size() == 1)
  {
    return EPICSServerInfo{parts.at(0)};
  }

  if (parts.size() == 2)
  {
    const auto port = mvvm::utils::StringToInteger(parts.at(1));
    if (port.has_value())
    {
      return WebSocketsServerInfo{parts.at(0), static_cast<std::uint16_t>(port.value())};
    }
  }

  throw RuntimeException("Can't create server info from server string");
}

}  // namespace oac_tree_gui
