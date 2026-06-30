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

#include "oac_tree_gui/model/server_info_items.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for EPICSServerInfoItem class.
 */
class EPICSServerInfoItemTest : public ::testing::Test
{
};

TEST_F(EPICSServerInfoItemTest, InitialState)
{
  const EPICSServerInfoItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<EPICSServerInfoItem>());
  EXPECT_EQ(item.Property<std::string>(EPICSServerInfoItem::kServerName), std::string());

  EXPECT_EQ(item.GetServerInfo(), EPICSServerInfo{});
}

TEST_F(EPICSServerInfoItemTest, SetAndGetServerInfo)
{
  EPICSServerInfoItem item;

  const EPICSServerInfo info{"MyServer"};
  item.SetServerInfo(info);

  EXPECT_EQ(item.Property<std::string>(EPICSServerInfoItem::kServerName), std::string("MyServer"));
  EXPECT_EQ(item.GetServerInfo(), info);
}

TEST_F(EPICSServerInfoItemTest, Clone)
{
  EPICSServerInfoItem item;
  item.SetServerInfo(EPICSServerInfo{"MyServer"});

  auto clone = item.Clone();

  auto clone_ptr = dynamic_cast<EPICSServerInfoItem*>(clone.get());
  ASSERT_NE(clone_ptr, nullptr);
  EXPECT_EQ(clone_ptr->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(clone_ptr->GetServerInfo(), EPICSServerInfo{"MyServer"});
}

/**
 * @brief Tests for WebSocketsServerInfoItem class.
 */
class WebSocketsServerInfoItemTest : public ::testing::Test
{
};

TEST_F(WebSocketsServerInfoItemTest, InitialState)
{
  const WebSocketsServerInfoItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<WebSocketsServerInfoItem>());
  EXPECT_EQ(item.Property<std::string>(WebSocketsServerInfoItem::kServerName), std::string());
  EXPECT_EQ(item.Property<mvvm::uint16>(WebSocketsServerInfoItem::kServerPort), mvvm::uint16{});

  EXPECT_EQ(item.GetServerInfo(), WebSocketsServerInfo{});
}

TEST_F(WebSocketsServerInfoItemTest, SetAndGetServerInfo)
{
  WebSocketsServerInfoItem item;

  const WebSocketsServerInfo info{"localhost", 8080};
  item.SetServerInfo(info);

  EXPECT_EQ(item.Property<std::string>(WebSocketsServerInfoItem::kServerName),
            std::string("localhost"));
  EXPECT_EQ(item.Property<mvvm::uint16>(WebSocketsServerInfoItem::kServerPort), mvvm::uint16{8080});
  EXPECT_EQ(item.GetServerInfo(), info);
}

TEST_F(WebSocketsServerInfoItemTest, Clone)
{
  WebSocketsServerInfoItem item;
  item.SetServerInfo(WebSocketsServerInfo{"localhost", 8080});

  auto clone = item.Clone();

  auto clone_ptr = dynamic_cast<WebSocketsServerInfoItem*>(clone.get());
  ASSERT_NE(clone_ptr, nullptr);
  EXPECT_EQ(clone_ptr->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(clone_ptr->GetServerInfo(), (WebSocketsServerInfo{"localhost", 8080}));
}

/**
 * @brief Tests for ServerGroupItem class.
 */
class ServerGroupItemTest : public ::testing::Test
{
};

TEST_F(ServerGroupItemTest, InitialState)
{
  const ServerGroupItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<ServerGroupItem>());
  ASSERT_EQ(item.GetTotalItemCount(), 2);

  // EPICS server is selected by default (first in the group)
  EXPECT_EQ(item.GetCurrentIndex(), 0);
  EXPECT_EQ(item.GetCurrentType(), mvvm::GetTypeName<EPICSServerInfoItem>());

  EXPECT_EQ(item.GetAutomationServerInfo(), AutomationServerInfo{EPICSServerInfo{}});
}

TEST_F(ServerGroupItemTest, SetAndGetEPICSServerInfo)
{
  ServerGroupItem item;

  const AutomationServerInfo info{EPICSServerInfo{"MyServer"}};
  item.SetAutomationServerInfo(info);

  EXPECT_EQ(item.GetCurrentType(), mvvm::GetTypeName<EPICSServerInfoItem>());
  EXPECT_EQ(item.GetAutomationServerInfo(), info);
}

TEST_F(ServerGroupItemTest, SetAndGetWebSocketsServerInfo)
{
  ServerGroupItem item;

  const AutomationServerInfo info{WebSocketsServerInfo{"localhost", 8080}};
  item.SetAutomationServerInfo(info);

  // setting WebSockets info switches the currently selected item
  EXPECT_EQ(item.GetCurrentIndex(), 1);
  EXPECT_EQ(item.GetCurrentType(), mvvm::GetTypeName<WebSocketsServerInfoItem>());
  EXPECT_EQ(item.GetAutomationServerInfo(), info);
}

TEST_F(ServerGroupItemTest, SwitchBetweenServerTypes)
{
  ServerGroupItem item;

  item.SetAutomationServerInfo(AutomationServerInfo{WebSocketsServerInfo{"localhost", 8080}});
  EXPECT_EQ(item.GetCurrentType(), mvvm::GetTypeName<WebSocketsServerInfoItem>());

  const AutomationServerInfo epics_info{EPICSServerInfo{"MyServer"}};
  item.SetAutomationServerInfo(epics_info);
  EXPECT_EQ(item.GetCurrentType(), mvvm::GetTypeName<EPICSServerInfoItem>());
  EXPECT_EQ(item.GetAutomationServerInfo(), epics_info);
}

TEST_F(ServerGroupItemTest, Clone)
{
  ServerGroupItem item;
  item.SetAutomationServerInfo(AutomationServerInfo{WebSocketsServerInfo{"localhost", 8080}});

  auto clone = item.Clone();

  auto clone_ptr = dynamic_cast<ServerGroupItem*>(clone.get());
  ASSERT_NE(clone_ptr, nullptr);
  EXPECT_EQ(clone_ptr->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(clone_ptr->GetCurrentType(), mvvm::GetTypeName<WebSocketsServerInfoItem>());
  EXPECT_EQ(clone_ptr->GetAutomationServerInfo(),
            (AutomationServerInfo{WebSocketsServerInfo{"localhost", 8080}}));
}

}  // namespace oac_tree_gui::test
