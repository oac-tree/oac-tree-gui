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

#include "oac_tree_gui/components/custom_presentation_items.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/model/compound_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for presentation items defined in custom_presentation_items.h
 */
class CustomPresentationItemTest : public ::testing::Test
{
public:
  class TestItem : public mvvm::CompoundItem
  {
  public:
    static inline const std::string kIsAvailable = "is_available";

    TestItem() : CompoundItem("TestItem") { AddProperty("is_available", true); }
  };
};

TEST_F(CustomPresentationItemTest, ChannelPresentationItem)
{
  TestItem item;
  const ChannelPresentationItem presentation(item.GetItem(TestItem::kIsAvailable), "channel_name");

  // it should looke like non-editable label with colored rectangle

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DecorationRole).isValid());

  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("channel_name"));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetConnectedVariableColor());

  // changing IsAvailable status should change the color of the rectangle
  EXPECT_TRUE(item.SetProperty(TestItem::kIsAvailable, false));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetDisonnectedVariableColor());

  EXPECT_EQ(presentation.GetQtRolesToEmit(mvvm::DataRole::kData),
            QVector<int>({Qt::DecorationRole}));
}

TEST_F(CustomPresentationItemTest, ExclusiveCheckStatePresentationItem)
{
  InstructionContainerItem container;
  auto sequence0 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto sequence1 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto sequence2 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  // all three unchecked
  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  auto root_property1 = GetIsRootItem(*sequence1);

  ExclusiveCheckStatePresentationItem presentation1(root_property1);

  EXPECT_EQ(presentation1.GetInstructionContainer(), &container);
  EXPECT_EQ(presentation1.GetInstructionItem(), sequence1);
  EXPECT_EQ(presentation1.GetSiblings(), std::vector<InstructionItem*>({sequence0, sequence2}));

  EXPECT_FALSE(presentation1.Data(Qt::DisplayRole).isValid());  // no text
  EXPECT_TRUE(presentation1.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  // checking middle item
  EXPECT_TRUE(presentation1.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_TRUE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  // checking first item
  auto root_property0 = GetIsRootItem(*sequence0);
  ExclusiveCheckStatePresentationItem presentation0(root_property0);
  EXPECT_TRUE(presentation0.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_TRUE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_FALSE(sequence2->IsRoot());

  // checking last item
  auto root_property2 = GetIsRootItem(*sequence2);
  ExclusiveCheckStatePresentationItem presentation2(root_property2);
  EXPECT_TRUE(presentation2.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));

  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_TRUE(sequence2->IsRoot());

  EXPECT_EQ(presentation0.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation1.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Checked);

  // checking same item again
  EXPECT_FALSE(presentation2.SetData(QVariant(Qt::Checked), Qt::CheckStateRole));
  EXPECT_FALSE(sequence0->IsRoot());
  EXPECT_FALSE(sequence1->IsRoot());
  EXPECT_TRUE(sequence2->IsRoot());
}

}  // namespace oac_tree_gui::test
