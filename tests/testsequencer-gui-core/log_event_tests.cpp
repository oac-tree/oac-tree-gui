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

#include "sequencergui/jobsystem/log_event.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class LogEventTests : public ::testing::Test
{
};

TEST_F(LogEventTests, EqualityOperator)
{
  LogEvent event1;
  LogEvent event2;
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  LogEvent event3{"sup", Severity::kWarning, "2022-12-01", "18:52:01.001", "message"};
  LogEvent event4{"sup", Severity::kWarning, "2022-12-01", "18:52:01.001", "message"};
  EXPECT_TRUE(event3 == event4);
  EXPECT_FALSE(event3 != event4);

  LogEvent event5{"sup", Severity::kWarning, "2022-12-01", "18:52:01.001", "message"};
  LogEvent event6{"sup", Severity::kError, "2022-12-01", "18:52:01.001", "message"};
  EXPECT_FALSE(event5 == event6);
  EXPECT_TRUE(event5 != event6);
}

TEST_F(LogEventTests, CreateLogEvent)
{
  auto event = CreateLogEvent(Severity::kWarning, "abc");
  EXPECT_EQ(event.severity, Severity::kWarning);
  EXPECT_EQ(event.message, std::string("abc"));
}
