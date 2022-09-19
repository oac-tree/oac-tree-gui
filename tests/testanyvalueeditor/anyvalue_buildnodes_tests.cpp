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

  EXPECT_EQ(node.GetNodeType(), AbstractAnyValueBuildNode::NodeType::kValue);

  // processing empty stack
  std::stack<AbstractAnyValueBuildNode::node_t> stack;
  EXPECT_TRUE(node.Process(stack));

  // processing stack containing another value
  stack.push(
      std::make_unique<AnyValueBuildNode>(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}));
  EXPECT_THROW(node.Process(stack), std::runtime_error);

  // processing stack containing a field
  stack.push(std::make_unique<StartFieldBuildNode>());
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
  EXPECT_TRUE(node.IsStartStructNode());

  EXPECT_EQ(node.GetNodeType(), AbstractAnyValueBuildNode::NodeType::kStartStruct);

  // processing empty stack
  std::stack<AbstractAnyValueBuildNode::node_t> stack;
  EXPECT_TRUE(node.Process(stack));

  // processing stack containing another struct
  stack.push(std::make_unique<StartStructBuildNode>(std::string()));
  EXPECT_THROW(node.Process(stack), std::runtime_error);

  // processing stack containing a field
  stack.push(std::make_unique<StartFieldBuildNode>());
  EXPECT_TRUE(node.Process(stack));

  // expected value
  auto expected = ::sup::dto::EmptyStruct("struct_name");
  auto result = node.MoveAnyValue();
  EXPECT_EQ(result, expected);
}

TEST_F(AnyValueBuildNodesTests, StartStructBuildNodeProcessAddField)
{
  StartStructBuildNode node("struct_name");

  // adding a field to struct
  EXPECT_NO_THROW(
      node.AddMember("field_name", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}));

  // expected value
  sup::dto::AnyValue expected_anyvalue = {{{"field_name", {sup::dto::SignedInteger32Type, 42}}},
                                          "struct_name"};
  auto result = node.MoveAnyValue();
  EXPECT_EQ(result, expected_anyvalue);
}

TEST_F(AnyValueBuildNodesTests, StartFieldBuildNodeProcess)
{
  StartFieldBuildNode node;
  node.SetFieldName("field_name");

  EXPECT_EQ(node.GetFieldName(), std::string("field_name"));

  EXPECT_FALSE(node.IsStartElementNode());
  EXPECT_TRUE(node.IsStartFieldNode());
  EXPECT_FALSE(node.IsStartStructNode());

  EXPECT_EQ(node.GetNodeType(), AbstractAnyValueBuildNode::NodeType::kStartField);

  // processing of empty stack is not allowed
  std::stack<AbstractAnyValueBuildNode::node_t> stack;
  EXPECT_THROW(node.Process(stack), std::runtime_error);

  // stack is possible to process if it contains StartStructBuildNode
  stack.push(std::make_unique<StartStructBuildNode>(std::string()));
  EXPECT_TRUE(node.Process(stack));

  // field name should be set
  node.SetFieldName("");
  EXPECT_THROW(node.Process(stack), std::runtime_error);
}

TEST_F(AnyValueBuildNodesTests, EndFieldBuildNodeProcess)
{
  EndFieldBuildNode node;
  EXPECT_EQ(node.GetNodeType(), AbstractAnyValueBuildNode::NodeType::kEndField);
}
