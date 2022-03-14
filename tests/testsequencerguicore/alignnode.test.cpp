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

#include "sequencergui/algorithm/alignnode.h"

#include <gtest/gtest.h>

namespace
{
template <typename T>
auto CreateTestData()
{
  auto uptr = std::make_unique<T>();
  auto raw_ptr = uptr.get();
  return std::make_pair(std::move(uptr), raw_ptr);
}
}  // namespace

using namespace sequencergui::algorithm;

class AlignNodeTest : public ::testing::Test
{
};

TEST_F(AlignNodeTest, InitialState)
{
  AlignNode node;
  EXPECT_EQ(node.GetParent(), nullptr);
  EXPECT_EQ(node.GetSize(), 0);
  EXPECT_TRUE(node.GetChildren().empty());
  EXPECT_EQ(node.GetX(), 0.0);
  EXPECT_EQ(node.GetY(), 0.0);
  EXPECT_EQ(node.GetMod(), 0.0);
}

TEST_F(AlignNodeTest, GetAndSet)
{
  AlignNode parent;
  AlignNode node;

  node.SetParent(&parent);
  EXPECT_EQ(node.GetParent(), &parent);

  node.SetX(42.1);
  EXPECT_EQ(node.GetX(), 42.1);

  node.SetY(42.2);
  EXPECT_EQ(node.GetY(), 42.2);

  node.SetMod(42.3);
  EXPECT_EQ(node.GetMod(), 42.3);
}

TEST_F(AlignNodeTest, Add)
{
  auto [child0, child0_ptr] = CreateTestData<AlignNode>();
  auto [child1, child1_ptr] = CreateTestData<AlignNode>();

  AlignNode node;
  node.Add(std::move(child0));
  node.Add(std::move(child1));
  EXPECT_EQ(node.GetSize(), 2);
  EXPECT_EQ(node.GetChildren(), std::vector<AlignNode*>({child0_ptr, child1_ptr}));
  EXPECT_EQ(child0_ptr->GetParent(), &node);
  EXPECT_EQ(child1_ptr->GetParent(), &node);
}
