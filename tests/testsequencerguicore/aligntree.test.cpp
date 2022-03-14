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

#include "sequencergui/algorithm/aligntree.h"

#include "sequencergui/algorithm/alignnode.h"

#include <gtest/gtest.h>

using namespace sequencergui::algorithm;

class AlignTreeTest : public ::testing::Test
{
};

TEST_F(AlignTreeTest, InitialState)
{
  AlignTree tree;
  EXPECT_NE(tree.GetRootNode(), nullptr);
}

TEST_F(AlignTreeTest, Add)
{
  AlignTree tree;

  // adding children
  auto child0 = tree.Add(nullptr);
  auto child1 = tree.Add(nullptr);
  EXPECT_EQ(tree.GetRootNode()->GetChildren(), std::vector<AlignNode*>({child0, child1}));
  EXPECT_EQ(child0->GetParent(), tree.GetRootNode());
  EXPECT_EQ(child1->GetParent(), tree.GetRootNode());

  // adding grandchild
  auto grandchild0 = tree.Add(child0);
  EXPECT_EQ(child0->GetChildren(), std::vector<AlignNode*>({grandchild0}));
  EXPECT_EQ(grandchild0->GetParent(), child0);
}
