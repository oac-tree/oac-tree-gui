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

#include "oac_tree_gui/settings/scoped_callback.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for ScopedCallback class.
 */
class ScopedCallbackTest : public ::testing::Test
{
public:
  ::testing::MockFunction<void()> m_mock_callback;
};

TEST_F(ScopedCallbackTest, DefaultConstructed)
{
  ScopedCallback connection;
  EXPECT_FALSE(connection.IsConnected());
  EXPECT_NO_FATAL_FAILURE(connection.Disconnect());
}

TEST_F(ScopedCallbackTest, DisconnectOnDestruction)
{
  EXPECT_CALL(m_mock_callback, Call()).Times(1);
  {
    const ScopedCallback connection(m_mock_callback.AsStdFunction());
    EXPECT_TRUE(connection.IsConnected());
  }
}

TEST_F(ScopedCallbackTest, ExplicitDisconnect)
{
  EXPECT_CALL(m_mock_callback, Call()).Times(1);

  ScopedCallback connection(m_mock_callback.AsStdFunction());
  connection.Disconnect();
  EXPECT_FALSE(connection.IsConnected());

  // second call and destruction do nothing
  connection.Disconnect();
}

TEST_F(ScopedCallbackTest, MoveConstruction)
{
  EXPECT_CALL(m_mock_callback, Call()).Times(1);

  ScopedCallback connection(m_mock_callback.AsStdFunction());
  const ScopedCallback moved(std::move(connection));

  EXPECT_FALSE(connection.IsConnected());  // NOLINT(bugprone-use-after-move)
  EXPECT_TRUE(moved.IsConnected());
}

TEST_F(ScopedCallbackTest, MoveAssignmentDisconnectsPrevious)
{
  ::testing::MockFunction<void()> mock_callback_previous;

  ScopedCallback connection(mock_callback_previous.AsStdFunction());
  ScopedCallback other(m_mock_callback.AsStdFunction());

  EXPECT_CALL(mock_callback_previous, Call()).Times(1);
  connection = std::move(other);
  ::testing::Mock::VerifyAndClearExpectations(&mock_callback_previous);

  EXPECT_TRUE(connection.IsConnected());
  EXPECT_CALL(m_mock_callback, Call()).Times(1);
}

}  // namespace oac_tree_gui::test
