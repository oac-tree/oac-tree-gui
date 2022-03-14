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
