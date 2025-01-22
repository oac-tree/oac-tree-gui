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

#include "sequencergui/nodeeditor/align_node.h"

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

namespace sequencergui::test
{

class AlignNodeTest : public ::testing::Test
{
};

TEST_F(AlignNodeTest, InitialState)
{
  using algorithm::AlignNode;

  AlignNode node;
  EXPECT_EQ(node.GetParent(), nullptr);
  EXPECT_EQ(node.GetSize(), 0);
  EXPECT_TRUE(node.GetChildren().empty());
  EXPECT_EQ(node.GetX(), 0.0);
  EXPECT_EQ(node.GetY(), 0.0);
  EXPECT_EQ(node.GetMod(), 0.0);

  EXPECT_TRUE(node.IsLeaf());
  EXPECT_TRUE(node.IsLeftMost());
  EXPECT_TRUE(node.IsRightMost());

  EXPECT_EQ(node.GetPreviousSibling(), nullptr);
  EXPECT_EQ(node.GetNextSibling(), nullptr);

  EXPECT_EQ(node.GetLeftMostSibling(), nullptr);

  EXPECT_EQ(node.GetLeftMostChild(), nullptr);
  EXPECT_EQ(node.GetRightMostChild(), nullptr);

  EXPECT_EQ(node.GetIndex(), -1);
}

TEST_F(AlignNodeTest, GetAndSet)
{
  using algorithm::AlignNode;

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

  node.SetPos(1.0, 2.0);
  EXPECT_EQ(node.GetX(), 1.0);
  EXPECT_EQ(node.GetY(), 2.0);
}

TEST_F(AlignNodeTest, Add)
{
  using algorithm::AlignNode;

  auto [child0, child0_ptr] = CreateTestData<AlignNode>();
  auto [child1, child1_ptr] = CreateTestData<AlignNode>();

  AlignNode node;
  auto ret0 = node.Add(std::move(child0));
  auto ret1 = node.Add(std::move(child1));
  EXPECT_EQ(ret0, child0_ptr);
  EXPECT_EQ(ret1, child1_ptr);

  EXPECT_EQ(node.GetSize(), 2);
  EXPECT_EQ(node.GetChildren(), std::vector<AlignNode*>({child0_ptr, child1_ptr}));
  EXPECT_EQ(child0_ptr->GetParent(), &node);
  EXPECT_EQ(child1_ptr->GetParent(), &node);
}

TEST_F(AlignNodeTest, LeftMostRightMost)
{
  using algorithm::AlignNode;

  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto child2 = node.Add<AlignNode>();

  EXPECT_TRUE(child0->IsLeftMost());
  EXPECT_FALSE(child0->IsRightMost());

  EXPECT_FALSE(child1->IsLeftMost());
  EXPECT_FALSE(child1->IsRightMost());

  EXPECT_FALSE(child2->IsLeftMost());
  EXPECT_TRUE(child2->IsRightMost());
}

TEST_F(AlignNodeTest, PreviousSiblingNextSibling)
{
  using algorithm::AlignNode;

  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto child2 = node.Add<AlignNode>();

  EXPECT_EQ(child0->GetPreviousSibling(), nullptr);
  EXPECT_EQ(child0->GetNextSibling(), child1);

  EXPECT_EQ(child1->GetPreviousSibling(), child0);
  EXPECT_EQ(child1->GetNextSibling(), child2);

  EXPECT_EQ(child2->GetPreviousSibling(), child1);
  EXPECT_EQ(child2->GetNextSibling(), nullptr);
}

TEST_F(AlignNodeTest, GetLeftMostSibling)
{
  using algorithm::AlignNode;

  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto child2 = node.Add<AlignNode>();

  EXPECT_EQ(child0->GetLeftMostSibling(), child0);
  EXPECT_EQ(child1->GetLeftMostSibling(), child0);
  EXPECT_EQ(child2->GetLeftMostSibling(), child0);
}

TEST_F(AlignNodeTest, GetLeftMostChildGetRightMostChild)
{
  using algorithm::AlignNode;

  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto child2 = node.Add<AlignNode>();

  EXPECT_EQ(node.GetLeftMostChild(), child0);
  EXPECT_EQ(node.GetRightMostChild(), child2);
}

TEST_F(AlignNodeTest, GetIndex)
{
  using algorithm::AlignNode;

  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto child2 = node.Add<AlignNode>();

  EXPECT_EQ(node.GetIndex(), -1);
  EXPECT_EQ(child0->GetIndex(), 0);
  EXPECT_EQ(child1->GetIndex(), 1);
  EXPECT_EQ(child2->GetIndex(), 2);
}

}
