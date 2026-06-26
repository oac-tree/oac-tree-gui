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

#include "oac_tree_gui/jobsystem/remote_connection_info.h"

#include <gtest/gtest.h>

namespace oac_tree_gui
{

/**
 * @brief Tests for free functions defined in remote_connection_info.h
 */
class RemoteConnectionInfoTest : public ::testing::Test
{
};

TEST_F(RemoteConnectionInfoTest, GetServerNameForEPICS)
{
  const AutomationServerInfo info{EPICSServerInfo{"MyEPICSServer"}};
  EXPECT_EQ(GetServerName(info), std::string("MyEPICSServer"));
}

TEST_F(RemoteConnectionInfoTest, GetServerNameForWebSockets)
{
  const AutomationServerInfo info{WebSocketsServerInfo{"localhost", 8080}};
  EXPECT_EQ(GetServerName(info), std::string("localhost:8080"));
}

}  // namespace oac_tree_gui
