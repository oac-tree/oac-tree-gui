/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <QDate>
#include <QDateTime>
#include <QTime>

namespace sequencergui::test
{

//! Tests for LogEvent class.

class LogEventTest : public ::testing::Test
{
};

TEST_F(LogEventTest, EqualityOperator)
{
  const LogEvent event1;
  const LogEvent event2;
  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  const LogEvent event3{"2022-12-01", "18:52:01.001", Severity::kWarning, "sup", "message"};
  const LogEvent event4{"2022-12-01", "18:52:01.001", Severity::kWarning, "sup", "message"};
  EXPECT_TRUE(event3 == event4);
  EXPECT_FALSE(event3 != event4);

  const LogEvent event5{"2022-12-01", "18:52:01.001", Severity::kWarning, "sup", "message"};
  const LogEvent event6{"2022-12-01", "18:52:01.001", Severity::kError, "sup", "message"};
  EXPECT_FALSE(event5 == event6);
  EXPECT_TRUE(event5 != event6);
}

TEST_F(LogEventTest, CreateLogEvent)
{
  auto event = CreateLogEvent(Severity::kWarning, "abc");
  EXPECT_EQ(event.severity, Severity::kWarning);
  EXPECT_EQ(event.message, std::string("abc"));

  auto date =
      QDate::fromString(QString::fromStdString(event.date), GetLogEventDateFormat().c_str());
  auto time =
      QTime::fromString(QString::fromStdString(event.time), GetLogEventTimeFormat().c_str());
  EXPECT_TRUE(QDateTime(date, time).isValid());
}

}  // namespace sequencergui::test
