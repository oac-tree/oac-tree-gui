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

#include "sequencergui/nodeeditor/connectableviewmap.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/connectableviewfactory.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for InstructionViewItemMap class.

class ConnectableViewMapTest : public ::testing::Test
{
};

//! Testing Insert method.
TEST_F(ConnectableViewMapTest, InsertThenFind)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // map is empty at the beginning
  EXPECT_FALSE(map.FindView(&instruction));

  // inserting to the map
  map.Insert(&instruction, view.get());

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view.get());

  // it is not possible to insert another view for given instruction
  auto another_view = ConnectableViewFactory().CreateView(&instruction);
  EXPECT_THROW(map.Insert(&instruction, another_view.get()), ExistingKeyException);

  map.Clear();
  EXPECT_FALSE(map.FindView(&instruction));
}

TEST_F(ConnectableViewMapTest, InsertThenRemove)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // inserting to the map
  map.Insert(&instruction, view.get());

  // removing
  map.Remove(&instruction);

  EXPECT_FALSE(map.FindView(&instruction));

  // it is not possible to remove what was already removed
  EXPECT_THROW(map.Remove(&instruction), NotFoundKeyException);
}

//! Testing InsertOrAssign method.
TEST_F(ConnectableViewMapTest, InsertOrAssignThenFind)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // map is empty at the beginning
  EXPECT_FALSE(map.FindView(&instruction));

  // updating the map
  map.InsertOrAssign(&instruction, view.get());

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view.get());

  map.Clear();
  EXPECT_FALSE(map.FindView(&instruction));
}

//! Testing InsertOrAssign method when
TEST_F(ConnectableViewMapTest, TakeView)
{
  WaitItem instruction0;
  auto view0 = ConnectableViewFactory().CreateView(&instruction0);
  WaitItem instruction1;
  auto view1 = ConnectableViewFactory().CreateView(&instruction1);
  ConnectableViewMap map;

  // updating the map
  map.InsertOrAssign(&instruction0, view0.get());
  map.InsertOrAssign(&instruction1, view1.get());

  auto taken = map.TakeView(&instruction0);
  EXPECT_EQ(taken, view0.get());

  // the view is not in the map anymore
  EXPECT_EQ(map.FindView(&instruction0), nullptr);

  // it is not possible to take a view again
  EXPECT_THROW(map.TakeView(&instruction0), NotFoundKeyException);

  // another view is in the map
  EXPECT_EQ(map.FindView(&instruction1), view1.get());
}

//! Testing InsertOrAssign method when
TEST_F(ConnectableViewMapTest, InsertOrAssignTwice)
{
  WaitItem instruction;
  auto view0 = ConnectableViewFactory().CreateView(&instruction);
  auto view1 = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // updating the map
  map.InsertOrAssign(&instruction, view0.get());

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view0.get());

  // updating the map
  map.InsertOrAssign(&instruction, view1.get());

  // another view is in the map
  EXPECT_EQ(map.FindView(&instruction), view1.get());
}
