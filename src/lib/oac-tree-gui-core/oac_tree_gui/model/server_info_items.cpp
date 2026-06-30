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

#include "server_info_items.h"

#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

EPICSServerInfoItem::EPICSServerInfoItem()
    : mvvm::CompoundItem(mvvm::GetTypeName<EPICSServerInfoItem>())
{
  (void)AddProperty(kServerName, std::string()).SetDisplayName("Server name");
}

std::unique_ptr<mvvm::SessionItem> EPICSServerInfoItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

EPICSServerInfo EPICSServerInfoItem::GetServerInfo() const
{
  return EPICSServerInfo{Property<std::string>(kServerName)};
}

void EPICSServerInfoItem::SetServerInfo(const EPICSServerInfo& server_info)
{
  (void)SetProperty<std::string>(kServerName, server_info.server_name);
}

WebSocketsServerInfoItem::WebSocketsServerInfoItem()
    : mvvm::CompoundItem(mvvm::GetTypeName<WebSocketsServerInfoItem>())
{
  (void)AddProperty(kServerName, std::string()).SetDisplayName("Server address");
  (void)AddProperty(kServerPort, mvvm::int16{}).SetDisplayName("Port");
}

std::unique_ptr<mvvm::SessionItem> WebSocketsServerInfoItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

WebSocketsServerInfo WebSocketsServerInfoItem::GetServerInfo() const
{
  WebSocketsServerInfo result;
  result.server_address = Property<std::string>(kServerName);
  result.server_port = static_cast<std::uint16_t>(Property<mvvm::int16>(kServerPort));
  return result;
}

void WebSocketsServerInfoItem::SetServerInfo(const WebSocketsServerInfo& server_info)
{
  (void)SetProperty<std::string>(kServerName, server_info.server_address);
  (void)SetProperty<mvvm::int16>(kServerPort, static_cast<mvvm::int16>(server_info.server_port));
}

}  // namespace oac_tree_gui
