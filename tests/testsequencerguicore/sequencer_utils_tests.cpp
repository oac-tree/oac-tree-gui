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

#include "sequencergui/model/sequencer_utils.h"

#include "sequencergui/model/instruction_container_item.h"
#include "sequencergui/model/standard_instruction_items.h"

#include "mvvm/factories/item_catalogue_factory.h"
#include "mvvm/interfaces/item_manager_interface.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class SequencerUtilsTest : public ::testing::Test
{
};

TEST_F(SequencerUtilsTest, IsCompoundInstruction)
{
  SequenceItem sequence;
  EXPECT_TRUE(IsCompoundInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsCompoundInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsCompoundInstruction(&wait));
}

TEST_F(SequencerUtilsTest, IsDecoratorInstruction)
{
  SequenceItem sequence;
  EXPECT_FALSE(IsDecoratorInstruction(&sequence));

  RepeatItem repeat;
  EXPECT_TRUE(IsDecoratorInstruction(&repeat));

  WaitItem wait;
  EXPECT_FALSE(IsDecoratorInstruction(&wait));
}

TEST_F(SequencerUtilsTest, CreateSequencerItemCatalogues)
{
  // checking one sequencer related item in the catalogue
  auto catalogue = CreateSequencerItemCatalogue();

  auto item = catalogue->Create(SequenceItem::Type);
  EXPECT_EQ(item->GetType(), SequenceItem::Type);
}

TEST_F(SequencerUtilsTest, CreateSequencerItemManager)
{
  // checking one sequencer related item in the catalogue
  auto manager = CreateSequencerItemManager();

  auto item = manager->CreateItem(SequenceItem::Type);
  EXPECT_EQ(item->GetType(), SequenceItem::Type);
}
