/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
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

#include "anyvalueeditor/anyvalue_buildnodes.h"

#include <gtest/gtest.h>

using namespace anyvalueeditor;

//! Tests for classes in anyvalue_

class AnyValueBuildNodesTests : public ::testing::Test
{
};

TEST_F(AnyValueBuildNodesTests, AnyValueBuildNodeProcess)
{
  AnyValueBuildNode node(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  EXPECT_FALSE(node.IsStartElementNode());
  EXPECT_FALSE(node.IsStartFieldNode());

  // processing empty stack
  std::stack<AbstractAnyValueBuildNode::node_t> stack;
  EXPECT_TRUE(node.Process(stack));

  // processing stack containing another value
  stack.push(
      std::make_unique<AnyValueBuildNode>(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}));
  EXPECT_THROW(node.Process(stack), std::runtime_error);

  // processing stack containing a field
  stack.push(std::make_unique<StartFieldBuildNode>("field_name"));
  EXPECT_TRUE(node.Process(stack));

  // expected value
  sup::dto::AnyValue expected{sup::dto::SignedInteger32Type, 42};
  auto result = node.MoveAnyValue();
  EXPECT_EQ(result, expected);
}

TEST_F(AnyValueBuildNodesTests, StartStructBuildNodeProcess)
{
  StartStructBuildNode node("struct_name");

  EXPECT_FALSE(node.IsStartElementNode());
  EXPECT_FALSE(node.IsStartFieldNode());

  // processing empty stack
  std::stack<AbstractAnyValueBuildNode::node_t> stack;
  EXPECT_TRUE(node.Process(stack));

  // processing stack containing another struct
  stack.push(std::make_unique<StartStructBuildNode>(std::string()));
  EXPECT_THROW(node.Process(stack), std::runtime_error);

  // processing stack containing a field
  stack.push(std::make_unique<StartFieldBuildNode>("field_name"));
  EXPECT_TRUE(node.Process(stack));

  // expected value
  auto expected = ::sup::dto::EmptyStruct("struct_name");
  auto result = node.MoveAnyValue();
  EXPECT_EQ(result, expected);
}
