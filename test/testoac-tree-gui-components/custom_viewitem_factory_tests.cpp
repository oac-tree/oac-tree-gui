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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/viewmodel/custom_viewitem_factory.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>

#include <mvvm/viewmodel/viewitem.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for factory functions defined in custom_viewitem_factory.h
 */
class CustomViewItemFactoryTest : public ::testing::Test
{
};

TEST_F(CustomViewItemFactoryTest, ChannelPresentationItemForLocalVariable)
{
  LocalVariableItem item;

  auto viewitem = CreateChannelPresentationViewItem(item);

  // it should be just empty non-editable placeholder.
  EXPECT_FALSE(viewitem->Data(Qt::EditRole).isValid());
  EXPECT_TRUE(viewitem->Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), QString());
  EXPECT_FALSE(viewitem->Data(Qt::DecorationRole).isValid());

  // it is not possible to set any data
  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->SetData(QString("bbb"), Qt::EditRole));
}

TEST_F(CustomViewItemFactoryTest, ChannelPresentationItemForConnectableVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string channel_name("CHANNEL");
  PvAccessClientVariableItem item;
  item.SetChannel(channel_name);
  item.SetIsAvailable(true);

  auto viewitem = CreateChannelPresentationViewItem(item);

  // it should be just empty non-editable placeholder.
  EXPECT_FALSE(viewitem->Data(Qt::EditRole).isValid());
  EXPECT_TRUE(viewitem->Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), QString::fromStdString(channel_name));
  EXPECT_TRUE(viewitem->Data(Qt::DecorationRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DecorationRole).value<QColor>(), GetConnectedVariableColor());

  // it is not possible to set any data
  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->SetData(QString("bbb"), Qt::EditRole));
}

}  // namespace oac_tree_gui::test
