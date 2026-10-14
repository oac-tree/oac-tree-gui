/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/sequencer_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_includes.h>
#include <oac_tree_gui/model/universal_instruction_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/model/universal_variable_item.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

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
      AddProperty(itemconstants::kStatus, "");
      AddProperty(itemconstants::kBreakpoint, "");
      AddProperty(itemconstants::kIsAvailable, "");
      AddProperty(domainconstants::kIsRootAttribute, "");
    }
  };
};

TEST_F(SequencerItemHelperTest, IsCompoundInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsCompoundInstruction(wait.get()));
}

TEST_F(SequencerItemHelperTest, IsDecoratorInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsDecoratorInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(wait.get()));
}

TEST_F(SequencerItemHelperTest, Clone)
{
  using mvvm::test::IsCloneImplemented;

  // instructions
  EXPECT_TRUE(IsCloneImplemented<UniversalInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<InstructionInfoItem>());

  // variables
  EXPECT_TRUE(IsCloneImplemented<FileVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<LocalVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<UniversalVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<VariableInfoItem>());

  // other
  EXPECT_TRUE(IsCloneImplemented<InstructionContainerItem>());
  EXPECT_TRUE(IsCloneImplemented<ProcedureItem>());
  EXPECT_TRUE(IsCloneImplemented<TypeRegistrationItem>());
  EXPECT_TRUE(IsCloneImplemented<ProcedurePreambleItem>());
  EXPECT_TRUE(IsCloneImplemented<WorkspaceItem>());

  EXPECT_TRUE(IsCloneImplemented<LocalJobItem>());
  EXPECT_TRUE(IsCloneImplemented<ImportedJobItem>());
  EXPECT_TRUE(IsCloneImplemented<RemoteJobItem>());
  EXPECT_TRUE(IsCloneImplemented<FileBasedJobItem>());

  EXPECT_TRUE(IsCloneImplemented<PluginSettingsItem>());

  EXPECT_TRUE(IsCloneImplemented<EPICSServerInfoItem>());
  EXPECT_TRUE(IsCloneImplemented<WebSocketsServerInfoItem>());
  EXPECT_TRUE(IsCloneImplemented<ServerGroupItem>());
}

TEST_F(SequencerItemHelperTest, ClonePlugin)
{
  using mvvm::test::IsCloneImplemented;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // variables
  EXPECT_TRUE(IsCloneImplemented<ChannelAccessVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessClientVariableItem>());
  EXPECT_TRUE(IsCloneImplemented<PvAccessServerVariableItem>());
}

TEST_F(SequencerItemHelperTest, GetPropertyItem)
{
  // item doesn't have property items
  const mvvm::SessionItem item;
  EXPECT_EQ(GetStatusItem(item), nullptr);
  EXPECT_EQ(GetBreakpointItem(item), nullptr);
  EXPECT_EQ(GetIsAvailableItem(item), nullptr);
  EXPECT_EQ(GetIsRootItem(item), nullptr);

  // test item has property items
  const TestItem test_item;
  EXPECT_NE(GetStatusItem(test_item), nullptr);
  EXPECT_NE(GetBreakpointItem(test_item), nullptr);
  EXPECT_NE(GetIsAvailableItem(test_item), nullptr);
  EXPECT_NE(GetIsRootItem(test_item), nullptr);
}

TEST_F(SequencerItemHelperTest, IsPotentialRootInstruction)
{
  {  // instruction outside container
    const auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
    EXPECT_FALSE(IsPotentialRootInstruction(*sequence));
  }

  {  // two instructions in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence1));
  }

  {  // instruction with child in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_FALSE(IsPotentialRootInstruction(*wait0));
  }
}

}  // namespace oac_tree_gui::test
