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

#ifndef OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_
#define OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_

//! @file
//! Collection of items to store automation server information.

#include <oac_tree_gui/model/remote_connection_info.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/group_item.h>

namespace oac_tree_gui
{

/**
 * @brief The EPICSServerInfoItem class represents an EPICS-based automation server in the model.
 */
class EPICSServerInfoItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kServerName = "kServerName";

  EPICSServerInfoItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns server info constructed from item's properties.
   */
  EPICSServerInfo GetServerInfo() const;

  /**
   * @brief Saves server info into item's properties.
   */
  void SetServerInfo(const EPICSServerInfo& server_info);
};

/**
 * @brief The WebSocketsServerInfoItem class represents a WebSocket-based automation server in the
 * model.
 */
class WebSocketsServerInfoItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kServerName = "kServerName";
  static constexpr auto kServerPort = "kServerPort";

  WebSocketsServerInfoItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns server info constructed from item's properties.
   */
  WebSocketsServerInfo GetServerInfo() const;

  /**
   * @brief Saves server info into item's properties.
   */
  void SetServerInfo(const WebSocketsServerInfo& server_info);
};

/**
 * @brief The ServerGroupItem class represents a group of possible automation servers with only one
 * of them being current at a time.
 *
 * The group holds an EPICSServerInfoItem and a WebSocketsServerInfoItem; the currently selected one
 * defines the type of the automation server.
 */
class ServerGroupItem : public mvvm::GroupItem
{
public:
  ServerGroupItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns automation server info constructed from the currently selected item.
   */
  AutomationServerInfo GetAutomationServerInfo() const;

  /**
   * @brief Selects the item matching the given server info and saves the info into it.
   */
  void SetAutomationServerInfo(const AutomationServerInfo& server_info);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::EPICSServerInfoItem>
{
  static constexpr std::string_view type_name() noexcept { return "EPICSServerInfoItem"; }
};

template <>
struct item_traits<oac_tree_gui::WebSocketsServerInfoItem>
{
  static constexpr std::string_view type_name() noexcept { return "WebSocketsServerInfoItem"; }
};

template <>
struct item_traits<oac_tree_gui::ServerGroupItem>
{
  static constexpr std::string_view type_name() noexcept { return "ServerGroupItem"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_