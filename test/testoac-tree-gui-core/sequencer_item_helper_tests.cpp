/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/sequencer_item_includes.h>
#include <oac-tree-gui/model/universal_instruction_item.h>
#include <oac-tree-gui/model/universal_variable_item.h>

#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include "oac-tree-gui/model/sequencer_item_helper.h"

namespace oac_tree_gui::test
{

//! Tests for utility functions from sequencer_utils.h

class SequencerItemHelperTest : public ::testing::Test
{
public:
  class TestItem : public mvvm::CompoundItem
  {
  public:
    TestItem() : CompoundItem("TestItem")
    {
      AddProperty(itemconstants::kName, "");
      AddProperty(itemconstants::kStatus, "");
      AddProperty(itemconstants::kBreakpoint, "");
    }
  };
};

TEST_F(SequencerItemHelperTest, IsCompoundInstruction)
{
  SequenceItem sequence;
  EXPECT_TRUE(IsCompoundInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsCompoundInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsCompoundInstruction(&wait));
}

TEST_F(SequencerItemHelperTest, IsDecoratorInstruction)
{
  SequenceItem sequence;
  EXPECT_FALSE(IsDecoratorInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsDecoratorInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsDecoratorInstruction(&wait));
}

TEST_F(SequencerItemHelperTest, Clone)
{
  using mvvm::test::IsCloneImplemented;

  // instructions
  EXPECT_TRUE(IsCloneImplemented<IncludeItem>());
  EXPECT_TRUE(IsCloneImplemented<ParallelSequenceItem>());
  EXPECT_TRUE(IsCloneImplemented<RepeatItem>());
  EXPECT_TRUE(IsCloneImplemented<SequenceItem>());
  EXPECT_TRUE(IsCloneImplemented<WaitItem>());
  EXPECT_TRUE(IsCloneImplemented<UniversalInstructionItem>());

  // variables
  EXPECT_TRUE(IsCloneImplemented<FileVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<LocalVariableItem>());

  // other
  EXPECT_TRUE(IsCloneImplemented<InstructionContainerItem>());
  EXPECT_TRUE(IsCloneImplemented<ProcedureItem>());
  EXPECT_TRUE(IsCloneImplemented<TypeRegistrationItem>());
  EXPECT_TRUE(IsCloneImplemented<ProcedurePreambleItem>());
  EXPECT_TRUE(IsCloneImplemented<WorkspaceItem>());

  EXPECT_TRUE(IsCloneImplemented<LocalJobItem>());
  EXPECT_TRUE(IsCloneImplemented<ImportedJobItem>());
  EXPECT_TRUE(IsCloneImplemented<RemoteJobItem>());
}

TEST_F(SequencerItemHelperTest, ClonePlugin)
{
  using mvvm::test::IsCloneImplemented;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // instructions from sup-sequencer-plugin-epics
  EXPECT_TRUE(IsCloneImplemented<ChannelAccessReadInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<ChannelAccessWriteInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessReadInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessWriteInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<RPCClientInstruction>());
  EXPECT_TRUE(IsCloneImplemented<SystemCallInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<LogInstructionItem>());

  // variables
  EXPECT_TRUE(IsCloneImplemented<ChannelAccessVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessClientVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessServerVariableItem>());
}

TEST_F(SequencerItemHelperTest, GetPropertyItem)
{
  // item doesn't have property items
  mvvm::SessionItem item;
  EXPECT_EQ(GetNameItem(item), nullptr);
  EXPECT_EQ(GetStatusItem(item), nullptr);
  EXPECT_EQ(GetBreakpointItem(item), nullptr);

  // test item has property items
  TestItem test_item;
  EXPECT_NE(GetNameItem(test_item), nullptr);
  EXPECT_NE(GetStatusItem(test_item), nullptr);
  EXPECT_NE(GetBreakpointItem(test_item), nullptr);
}

}  // namespace oac_tree_gui::test
