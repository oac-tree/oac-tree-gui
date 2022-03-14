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

#include "sequencergui/algorithm/alignutils.h"

#include "sequencergui/algorithm/alignnode.h"

#include <gtest/gtest.h>

using namespace sequencergui::algorithm;

class AlignUtilsTest : public ::testing::Test
{
public:
  // Creates testing tree
  std::unique_ptr<AlignNode> CreateBlueTree()
  {
    auto result = std::make_unique<AlignNode>();
    result->SetPos(1.5, 1.0);
    auto node_G = result->Add<AlignNode>();
    node_G->SetPos(1.0, 2.0);
    auto node_M = result->Add<AlignNode>();
    node_M->SetPos(2.0, 2.0);

    node_M->Add<AlignNode>()->SetPos(0.0, 3.0);
    node_M->Add<AlignNode>()->SetPos(1.0, 3.0);
    node_M->Add<AlignNode>()->SetPos(2.0, 3.0);
    node_M->Add<AlignNode>()->SetPos(3.0, 3.0);

    return result;
  }
};

TEST_F(AlignUtilsTest, InitializeNodes)
{
  AlignNode node;
  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();
  auto grandchild0 = child0->Add<AlignNode>();

  InitializeNodes(node);

  EXPECT_EQ(node.GetX(), -1.0);
  EXPECT_EQ(node.GetY(), 0.0);
  EXPECT_EQ(node.GetMod(), 0.0);

  EXPECT_EQ(child0->GetX(), -1.0);
  EXPECT_EQ(child0->GetY(), 1.0);
  EXPECT_EQ(child0->GetMod(), 0.0);

  EXPECT_EQ(child1->GetX(), -1.0);
  EXPECT_EQ(child1->GetY(), 1.0);
  EXPECT_EQ(child1->GetMod(), 0.0);

  EXPECT_EQ(grandchild0->GetX(), -1.0);
  EXPECT_EQ(grandchild0->GetY(), 2.0);
  EXPECT_EQ(grandchild0->GetMod(), 0.0);
}

TEST_F(AlignUtilsTest, GetLeftContour)
{
  auto node = CreateBlueTree();
  auto contour = GetLeftCountour(*node);

  // expected left countour of the tree (level .vs. x-pos)
  std::map<int, double> expected = {{1, 1.5}, {2, 1.0}, {3, 0.0}};
  EXPECT_EQ(contour, expected);
}
