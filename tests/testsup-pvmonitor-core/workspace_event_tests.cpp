/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/workspace_event.h"

#include <gtest/gtest.h>

using namespace suppvmonitor;

//! Tests for WorkspaceSyncronizer class.

class WorkspaceEventTests : public ::testing::Test
{
};

TEST_F(WorkspaceEventTests, EqualityOperators)
{
  {  // default constructed
    WorkspaceEvent event1;
    WorkspaceEvent event2;
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // same names
    WorkspaceEvent event1{"abc"};
    WorkspaceEvent event2{"abc"};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // same names and values
    WorkspaceEvent event1{"abc", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    WorkspaceEvent event2{"abc", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // different names and same values
    WorkspaceEvent event1{"abc", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    WorkspaceEvent event2{"def", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }

  {  // same names and different values
    WorkspaceEvent event1{"abc", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    WorkspaceEvent event2{"abc", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }
}
