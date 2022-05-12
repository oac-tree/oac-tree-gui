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

#include "sequencergui/model/workspaceitem.h"

#include "sequencergui/model/standardvariableitems.h"

#include <gtest/gtest.h>

using namespace sequencergui;

class WorkspaceItemTest : public ::testing::Test
{
};

TEST_F(WorkspaceItemTest, WorkspaceItem)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariables().empty());

  auto var0 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto var1 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(item.GetVariables(), std::vector<VariableItem*>({var0, var1}));
}
