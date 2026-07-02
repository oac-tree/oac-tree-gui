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

#include "oac_tree_gui/model/remote_connection_info.h"

#include <oac_tree_gui/core/exceptions.h>

#include <gtest/gtest.h>

namespace oac_tree_gui
{

/**
 * @brief Tests for free functions defined in remote_connection_info.h
 */
class RemoteConnectionInfoTest : public ::testing::Test
{
};

TEST_F(RemoteConnectionInfoTest, GetServerName)
{
  {  // EPICS
    const AutomationServerInfo info{EPICSServerInfo{"MyEPICSServer"}};
    EXPECT_EQ(GetAutomationServerName(info), std::string("MyEPICSServer"));
  }

  {  // websockets
    const AutomationServerInfo info{WebSocketsServerInfo{"localhost", 8080}};
    EXPECT_EQ(GetAutomationServerName(info), std::string("localhost:8080"));
  }

  {  // undefined
    const AutomationServerInfo info;
    EXPECT_THROW(GetAutomationServerName(info), RuntimeException);
  }
}

TEST_F(RemoteConnectionInfoTest, EPICSServerInfoComparison)
{
  EXPECT_TRUE(EPICSServerInfo{"server"} == EPICSServerInfo{"server"});
  EXPECT_FALSE(EPICSServerInfo{"server"} != EPICSServerInfo{"server"});

  EXPECT_TRUE(EPICSServerInfo{"server"} != EPICSServerInfo{"other"});
  EXPECT_FALSE(EPICSServerInfo{"server"} == EPICSServerInfo{"other"});
}

TEST_F(RemoteConnectionInfoTest, WebSocketsServerInfoComparison)
{
  const WebSocketsServerInfo info{"localhost", 8080};

  EXPECT_TRUE(info == (WebSocketsServerInfo{"localhost", 8080}));
  EXPECT_FALSE(info != (WebSocketsServerInfo{"localhost", 8080}));

  // different address
  EXPECT_TRUE(info != (WebSocketsServerInfo{"remote", 8080}));
  // different port
  EXPECT_TRUE(info != (WebSocketsServerInfo{"localhost", 9090}));
}

TEST_F(RemoteConnectionInfoTest, AutomationServerInfoComparison)
{
  const AutomationServerInfo epics{EPICSServerInfo{"server"}};
  const AutomationServerInfo websockets{WebSocketsServerInfo{"localhost", 8080}};

  EXPECT_TRUE(epics == AutomationServerInfo{EPICSServerInfo{"server"}});
  EXPECT_TRUE(websockets == (AutomationServerInfo{WebSocketsServerInfo{"localhost", 8080}}));

  // same value, different alternative
  EXPECT_TRUE(epics != websockets);
  // same alternative, different value
  EXPECT_TRUE(epics != AutomationServerInfo{EPICSServerInfo{"other"}});
}

}  // namespace oac_tree_gui
