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

#include "sequencergui/viewmodel/custom_viewitem_factory.h"

#include <sequencergui/components/component_helper.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/standard_variable_items.h>

#include <mvvm/viewmodelbase/viewitem.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for factory functions defined in custom_viewitem_factory.h

class CustomViewItemFactoryTest : public ::testing::Test
{
public:
  class TestVariableItem : public ConnectableVariableItem
  {
  public:
    TestVariableItem() : ConnectableVariableItem("TestVariableItem")
    {
      AddProperty(domainconstants::kChannelAttribute, std::string(""));
    }
  };
};

//! Validating view item presenting channel and is_available status for LocalVariable.
//! It should be just empty non-editable placeholder.
TEST_F(CustomViewItemFactoryTest, ChannelPresentationItemForLocalVariable)
{
  LocalVariableItem item;

  auto viewitem = CreateChannelPresentationViewItem(item);

  EXPECT_FALSE(viewitem->Data(Qt::EditRole).isValid());
  EXPECT_TRUE(viewitem->Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), QString());
  EXPECT_FALSE(viewitem->Data(Qt::DecorationRole).isValid());

  // it is not possible to set any data
  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->SetData(QString("bbb"), Qt::EditRole));
}

//! Validating view item presenting channel and is_available status for LocalVariable.
//! It should be just empty non-editable placeholder.
TEST_F(CustomViewItemFactoryTest, ChannelPresentationItemForConnectableVariable)
{
  const std::string channel_name("CHANNEL");
  TestVariableItem item;
  item.SetChannel(channel_name);
  item.SetIsAvailable(true);

  auto viewitem = CreateChannelPresentationViewItem(item);

  EXPECT_FALSE(viewitem->Data(Qt::EditRole).isValid());
  EXPECT_TRUE(viewitem->Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), QString::fromStdString(channel_name));
  EXPECT_TRUE(viewitem->Data(Qt::DecorationRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DecorationRole).value<QColor>(), GetConnectedVariableColor());

  // it is not possible to set any data
  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->SetData(QString("bbb"), Qt::EditRole));
}
