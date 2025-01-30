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

#include "oac-tree-gui/viewmodel/custom_presentation_items.h"

#include <oac-tree-gui/components/component_helper.h>

#include <mvvm/model/compound_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for presentation items defined in custom_presentation_items.h

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
  ChannelPresentationItem presentation(item.GetItem(TestItem::kIsAvailable), "channel_name");

  // it should looke like non-editable label with colored rectangle

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DecorationRole).isValid());

  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("channel_name"));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetConnectedVariableColor());

  // changing IsAvailable status should change the color of the rectangle
  item.SetProperty(TestItem::kIsAvailable, false);
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), GetDisonnectedVariableColor());

  EXPECT_EQ(presentation.GetQtRoles(mvvm::DataRole::kDisplay),
            QVector<int>({Qt::DisplayRole, Qt::EditRole, Qt::DecorationRole}));
}

}  // namespace oac_tree_gui::test
