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

#include "oac_tree_gui/jobsystem/request_types.h"

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Test of classes in request_types.h
 */
class RequestTypesTest : public ::testing::Test
{
};

TEST_F(RequestTypesTest, UserChoiceArgs)
{
  {  // default constructed
    const UserChoiceArgs arg1{};
    const UserChoiceArgs arg2{};
    EXPECT_TRUE(arg1 == arg2);
    EXPECT_FALSE(arg1 != arg1);
  }

  {  // same values
    const UserChoiceArgs arg1{std::vector<std::string>({"abc"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const UserChoiceArgs arg2{std::vector<std::string>({"abc"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_TRUE(arg1 == arg2);
    EXPECT_FALSE(arg1 != arg2);
  }

  {  // different values
    const UserChoiceArgs arg1{std::vector<std::string>({"abc"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const UserChoiceArgs arg2{std::vector<std::string>({"abc"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}};
    EXPECT_FALSE(arg1 == arg2);
    EXPECT_TRUE(arg1 != arg2);
  }

  {  // different options
    const UserChoiceArgs arg1{std::vector<std::string>({"abc"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const UserChoiceArgs arg2{std::vector<std::string>({"def"}),
                              sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_FALSE(arg1 == arg2);
    EXPECT_TRUE(arg1 != arg2);
  }
}

TEST_F(RequestTypesTest, UserChoiceResult)
{
  {  // default constructed
    const UserChoiceResult res1{};
    const UserChoiceResult res2{};
    EXPECT_TRUE(res1 == res2);
    EXPECT_FALSE(res1 != res2);
  }

  {  // same values
    const UserChoiceResult res1{42, true};
    const UserChoiceResult res2{42, true};
    EXPECT_TRUE(res1 == res2);
    EXPECT_FALSE(res1 != res2);
  }

  {  // different index
    const UserChoiceResult res1{42, true};
    const UserChoiceResult res2{43, true};
    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
  }

  {  // different processed
    const UserChoiceResult res1{42, true};
    const UserChoiceResult res2{42, false};
    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
  }
}

TEST_F(RequestTypesTest, UserInputArgs)
{
  {  // default constructed
    const UserInputArgs arg1{};
    const UserInputArgs arg2{};
    EXPECT_TRUE(arg1 == arg2);
    EXPECT_FALSE(arg1 != arg1);
  }

  {  // same values
    const UserInputArgs arg1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, "abs"};
    const UserInputArgs arg2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, "abs"};
    EXPECT_TRUE(arg1 == arg2);
    EXPECT_FALSE(arg1 != arg2);
  }

  {  // different values
    const UserInputArgs arg1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, "abs"};
    const UserInputArgs arg2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}, "abs"};
    EXPECT_FALSE(arg1 == arg2);
    EXPECT_TRUE(arg1 != arg2);
  }

  {  // different description
    const UserInputArgs arg1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, "abs"};
    const UserInputArgs arg2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, "def"};
    EXPECT_FALSE(arg1 == arg2);
    EXPECT_TRUE(arg1 != arg2);
  }
}

TEST_F(RequestTypesTest, UserInputResult)
{
  {  // default constructed
    const UserInputResult res1{};
    const UserInputResult res2{};
    EXPECT_TRUE(res1 == res2);
    EXPECT_FALSE(res1 != res2);
  }

  {  // same values
    const UserInputResult res1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, true};
    const UserInputResult res2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, true};
    EXPECT_TRUE(res1 == res2);
    EXPECT_FALSE(res1 != res2);
  }

  {  // different values
    const UserInputResult res1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, true};
    const UserInputResult res2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}, true};
    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
  }

  {  // different processed flag
    const UserInputResult res1{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, true};
    const UserInputResult res2{sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, false};
    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
  }
}

}  // namespace oac_tree_gui::test
