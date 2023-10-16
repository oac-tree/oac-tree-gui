/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/viewmodel/standard_row_strategies.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/viewmodelbase/viewitem.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for strategies from standard_row_strategies.h.

class StandardRowStrategiesTest : public ::testing::Test
{
};

TEST_F(StandardRowStrategiesTest, VariableRowStrategy)
{
  {  // single local variable
    LocalVariableItem item;
    item.SetName("abc");

    VariableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);

    // expectind two elements in a row looking at DisplayName and Name
    ASSERT_EQ(view_items.size(), 2);
    EXPECT_EQ(view_items.at(0)->data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(1)->data(Qt::DisplayRole).toString(), QString("abc"));
  }
}

TEST_F(StandardRowStrategiesTest, VariableTableRowStrategyLocalVariable)
{
  {  // single local variable with a scalar
    LocalVariableItem item;
    const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);
    ASSERT_EQ(view_items.size(), 3);
    EXPECT_EQ(view_items.at(0)->data(Qt::DisplayRole).toString(), QString("abc"));
    EXPECT_EQ(view_items.at(1)->data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(view_items.at(2)->data(Qt::DisplayRole).toString(), QString("Local"));
  }

  {  // single local variable with a struct
    LocalVariableItem item;
    const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    // row corresponding to the variable itself
    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);
    ASSERT_EQ(view_items.size(), 3);
    EXPECT_EQ(view_items.at(0)->data(Qt::DisplayRole).toString(), QString("abc"));
    // value column should contain a placeholder
    EXPECT_EQ(view_items.at(1)->data(Qt::DisplayRole).toString(), QString("---"));
    EXPECT_EQ(view_items.at(2)->data(Qt::DisplayRole).toString(), QString("Local"));

    // row corresponding to the structure beneath
    auto struct_view_items = strategy.ConstructRow(item.GetAnyValueItem());
    ASSERT_EQ(struct_view_items.size(), 3);
    EXPECT_EQ(struct_view_items.at(0)->data(Qt::DisplayRole).toString(), QString("struct"));
    EXPECT_EQ(struct_view_items.at(1)->data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(struct_view_items.at(2)->data(Qt::DisplayRole).toString(), QString(""));

    // row corresponding to the scalar beneath
    auto scalar_view_items = strategy.ConstructRow(item.GetAnyValueItem()->GetChildren().at(0));
    ASSERT_EQ(scalar_view_items.size(), 3);
    EXPECT_EQ(scalar_view_items.at(0)->data(Qt::DisplayRole).toString(), QString("signed"));
    EXPECT_EQ(scalar_view_items.at(1)->data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(scalar_view_items.at(2)->data(Qt::DisplayRole).toString(), QString(""));
  }
}
