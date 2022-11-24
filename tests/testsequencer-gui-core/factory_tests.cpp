/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/core/factory.h"

#include <gtest/gtest.h>

using namespace sequencergui;

class FactoryTests : public ::testing::Test
{
public:
  class TestFactory : public Factory<std::string, int>
  {
  };
};

TEST_F(FactoryTests, DomainTypeConstants)
{
  TestFactory factory;
  EXPECT_FALSE(factory.Contains("abc"));
  factory.Register("abc", 42);
  EXPECT_TRUE(factory.Contains("abc"));

  EXPECT_EQ(factory.GetValue("abc"), 42);
  EXPECT_EQ(factory.GetKeys(), std::vector<std::string>({"abc"}));
}
