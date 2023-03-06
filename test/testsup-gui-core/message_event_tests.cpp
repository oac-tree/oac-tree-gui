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

#include "sup/gui/core/message_event.h"

#include <gtest/gtest.h>

using namespace sup::gui;

class MessageEventTest : public ::testing::Test
{
};

TEST_F(MessageEventTest, Equality)
{
  MessageEvent event1{"aa", "bb", "cc", "dd"};
  MessageEvent event2{"aa", "bb", "cc", "dd"};
  MessageEvent event3{"aa3", "bb3", "cc3", "dd3"};

  EXPECT_TRUE(event1 == event2);
  EXPECT_FALSE(event1 != event2);

  EXPECT_FALSE(event1 == event3);
  EXPECT_TRUE(event1 != event3);
}

TEST_F(MessageEventTest, CreateInvalidOperationMessage)
{
  auto message = CreateInvalidOperationMessage("text", "informative", "details");
  EXPECT_EQ(message.text, std::string("text"));
  EXPECT_EQ(message.informative, std::string("informative"));
  EXPECT_EQ(message.detailed, std::string("details"));
}
