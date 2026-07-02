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

#include "oac_tree_gui/jobsystem/automation_client.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/oac-tree-server/epics_utils.h>
#include <sup/oac-tree-server/ws_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui
{

/**
 * @brief Tests for AutomationClientTest class.
 */
class AutomationClientTest : public ::testing::Test
{
};

TEST_F(AutomationClientTest, CreateConnectionManager)
{
  {  // for EPICS server
    const EPICSServerInfo info{"server_name"};
    auto manager = CreateConnectionManager(info);
    EXPECT_NE(CreateConnectionManager(info).get(), nullptr);
  }

  {  // for WebSockets
    const WebSocketsServerInfo info{"localhost", 8080};
    auto manager = CreateConnectionManager(info);
    EXPECT_NE(CreateConnectionManager(info).get(), nullptr);
  }

  {  // undefined
    const AutomationServerInfo info;
    EXPECT_THROW(CreateConnectionManager(info), RuntimeException);
  }
}

}  // namespace oac_tree_gui
