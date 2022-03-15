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

  // Creates testing tree
  std::unique_ptr<AlignNode> CreateGreenTree()
  {
    auto result = std::make_unique<AlignNode>();
    result->SetPos(0.5, 1.0);
    auto node_A = result->Add<AlignNode>();
    node_A->SetPos(0.0, 2.0);
    auto node_D = result->Add<AlignNode>();
    node_D->SetPos(1.0, 2.0);

    node_D->Add<AlignNode>()->SetPos(0.5, 3.0);
    node_D->Add<AlignNode>()->SetPos(1.5, 3.0);

    return result;
  }

  // Creates testing tree with `mod` parameter accumulated.
  std::unique_ptr<AlignNode> CreateGreenTreeWithMod()
  {
    auto result = std::make_unique<AlignNode>();
    result->SetPos(0.5, 1.0);
    result->SetMod(1.0);
    auto node_A = result->Add<AlignNode>();
    node_A->SetPos(0.0, 2.0);
    auto node_D = result->Add<AlignNode>();
    node_D->SetPos(1.0, 2.0);
    node_D->SetMod(2.0);

    node_D->Add<AlignNode>()->SetPos(0.5, 3.0);
    node_D->Add<AlignNode>()->SetPos(1.5, 3.0);

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

TEST_F(AlignUtilsTest, GetRightContour)
{
  auto node = CreateGreenTree();
  auto contour = GetRightCountour(*node);

  // expected right countour of the tree (level .vs. x-pos)
  std::map<int, double> expected = {{1, 0.5}, {2, 1.0}, {3, 1.5}};
  EXPECT_EQ(contour, expected);
}

TEST_F(AlignUtilsTest, GetRightContourV2)
{
  auto node = CreateGreenTreeWithMod();
  auto contour = GetRightCountour(*node);

  EXPECT_EQ(contour.size(), 3);
  EXPECT_DOUBLE_EQ(contour[1], 0.5);
  EXPECT_DOUBLE_EQ(contour[2], 2.0);  // parent mod + own position
  EXPECT_DOUBLE_EQ(contour[3], 4.5);  // sum of parent mods + own position
}

TEST_F(AlignUtilsTest, CalculateInitialXSingleChild)
{
  AlignNode node;
  node.SetX(42);

  auto child = node.Add<AlignNode>();
  child->SetX(42);

  CalculateInitialX(node);

  EXPECT_EQ(child->GetX(), 0.0);
  EXPECT_EQ(child->GetMod(), 0.0);
  EXPECT_EQ(node.GetX(), 0.0);
  EXPECT_EQ(node.GetMod(), 0.0);
}

TEST_F(AlignUtilsTest, CalculateInitialXTwoChildren)
{
  AlignNode node;
  node.SetX(42);

  auto child0 = node.Add<AlignNode>();
  auto child1 = node.Add<AlignNode>();

  CalculateInitialX(node);

  EXPECT_EQ(child0->GetX(), 0.0);
  EXPECT_EQ(child0->GetMod(), 0.0);
  EXPECT_EQ(child1->GetX(), 1.0);
  EXPECT_EQ(child1->GetMod(), 0.0);
  EXPECT_DOUBLE_EQ(node.GetX(), 0.5);
  EXPECT_EQ(node.GetMod(), 0.0);
}

TEST_F(AlignUtilsTest, CalculateInitialXTwoGrandChildren)
{
  AlignNode node;
  node.SetX(42);

  auto child0 = node.Add<AlignNode>();
  auto grandchild0 = child0->Add<AlignNode>();
  auto grandchild1 = child0->Add<AlignNode>();

  auto child1 = node.Add<AlignNode>();
  auto grandchild2 = child1->Add<AlignNode>();
  auto grandchild3 = child1->Add<AlignNode>();

  CalculateInitialX(node);

  EXPECT_EQ(grandchild0->GetX(), 0.0);
  EXPECT_EQ(grandchild1->GetX(), 1.0);
  EXPECT_EQ(grandchild2->GetX(), 0.0);
  EXPECT_EQ(grandchild3->GetX(), 1.0);

  EXPECT_EQ(grandchild0->GetMod(), 0.0);
  EXPECT_EQ(grandchild1->GetMod(), 0.0);
  EXPECT_EQ(grandchild2->GetMod(), 0.0);
  EXPECT_EQ(grandchild3->GetMod(), 0.0);

  EXPECT_EQ(child0->GetX(), 0.5);
  EXPECT_EQ(child1->GetX(), 1.5);

  EXPECT_EQ(child0->GetMod(), 0.0);
  EXPECT_EQ(child1->GetMod(), 1.0);
}
