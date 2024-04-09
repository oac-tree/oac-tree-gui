/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/procedure_item.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/epics_instruction_items.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing ProcedureItem class.

class ProcedureItemTest : public ::testing::Test
{
};

TEST_F(ProcedureItemTest, InitialState)
{
  ProcedureItem item;

  EXPECT_TRUE(item.GetInstructionContainer());
  EXPECT_TRUE(item.GetWorkspace());
  EXPECT_TRUE(item.GetPreambleItem());
}

TEST_F(ProcedureItemTest, GetterAndSetter)
{
  ProcedureItem item;

  EXPECT_EQ(item.GetName(), std::string());
  EXPECT_EQ(item.GetDescription(), std::string());
  EXPECT_EQ(item.GetFileName(), std::string());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetDescription("def");
  EXPECT_EQ(item.GetDescription(), std::string("def"));

  item.SetFileName("jhk");
  EXPECT_EQ(item.GetFileName(), std::string("jhk"));
}

//! Checking CollectPluginNames when procedure contains only core objects.
TEST_F(ProcedureItemTest, CollectPluginNamesForCoreObjects)
{
  {  // empty procedure
    const ProcedureItem item;

    // no plugin names are necessary
    EXPECT_TRUE(CollectPluginNames(item).empty());
  }

  {  // procedure with local variable
    const ProcedureItem item;
    item.GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());

    // no plugin names are necessary
    EXPECT_TRUE(CollectPluginNames(item).empty());
  }

  {  // procedure with sequence
    const ProcedureItem item;
    item.GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

    // no plugin names are necessary
    EXPECT_TRUE(CollectPluginNames(item).empty());
  }
}

//! Checking CollectPluginNames when procedure contains EPICS instructions and variables.
TEST_F(ProcedureItemTest, CollectPluginNamesForEpicsObjects)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  {  // procedure with ChannelAccessVariable
    const ProcedureItem item;
    item.GetWorkspace()->InsertItem<ChannelAccessVariableItem>(mvvm::TagIndex::Append());

    // no plugin names are necessary
    const std::set<std::string> expected({domainconstants::kEpicsCAPluginName});
    EXPECT_EQ(CollectPluginNames(item), expected);
  }

  {  // procedure with ChannelAccessVariable and PvAccessWriteInstructionItem
    const ProcedureItem item;
    item.GetWorkspace()->InsertItem<ChannelAccessVariableItem>(mvvm::TagIndex::Append());
    item.GetInstructionContainer()->InsertItem<PvAccessWriteInstructionItem>(
        mvvm::TagIndex::Append());

    // no plugin names are necessary
    const std::set<std::string> expected(
        {domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName});
    EXPECT_EQ(CollectPluginNames(item), expected);
  }
}
