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

#include "oac_tree_gui/components/custom_row_strategies.h"

#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/variable_info_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/providers/viewitem.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for strategies from custom_row_strategies.h.

class CustomRowStrategiesTest : public ::testing::Test
{
};

TEST_F(CustomRowStrategiesTest, VariableRowStrategyForLocalVariable)
{
  LocalVariableItem item;
  item.SetName("abc");

  VariableRowStrategy strategy;
  auto view_items = strategy.ConstructRow(&item);

  // expectind two elements in a row looking at DisplayName and Name
  ASSERT_EQ(view_items.size(), 3);
  EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
  EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
  EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
}

TEST_F(CustomRowStrategiesTest, VariableRowStrategyForUniversalItem)
{
  if (!IsVariableTypeAvailable(domainconstants::kSystemClockVariableType))
  {
    GTEST_SKIP();
  }

  UniversalVariableItem item;
  item.SetDomainType(domainconstants::kSystemClockVariableType);
  item.SetName("abc");

  VariableRowStrategy strategy;
  auto view_items = strategy.ConstructRow(&item);

  // expectind two elements in a row looking at DisplayName and Name
  ASSERT_EQ(view_items.size(), 3);
  EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
  EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
  EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(),
            QString::fromStdString(domainconstants::kSystemClockVariableType));
}

TEST_F(CustomRowStrategiesTest, VariableRowStrategyForVariableInfo)
{
  auto item = CreateVariableInfoItem(domainconstants::kLocalVariableType);
  item->SetName("abc");

  VariableRowStrategy strategy;
  auto view_items = strategy.ConstructRow(item.get());

  // expectind two elements in a row looking at DisplayName and Name
  ASSERT_EQ(view_items.size(), 3);
  EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
  EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
  EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
}

TEST_F(CustomRowStrategiesTest, VariableTableRowStrategyLocalVariable)
{
  {  // single local variable with a scalar
    LocalVariableItem item;
    const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);
    ASSERT_EQ(view_items.size(), 4);
    EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
    EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));  // channel
  }

  {  // single local variable with a scalar (constructed as VariableInfo)
    auto item = CreateVariableInfoItem(domainconstants::kLocalVariableType);
    const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
    SetAnyValue(anyvalue, *item);
    item->SetName("abc");

    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(item.get());
    ASSERT_EQ(view_items.size(), 4);
    EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
    EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));  // channel
  }

  {  // single local variable with a struct
    LocalVariableItem item;
    const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    // row corresponding to the variable itself
    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);
    ASSERT_EQ(view_items.size(), 4);
    EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
    // value column should contain a placeholder
    EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString("---"));
    EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));

    // row corresponding to the structure beneath
    auto struct_view_items = strategy.ConstructRow(item.GetAnyValueItem());
    ASSERT_EQ(struct_view_items.size(), 4);
    EXPECT_EQ(struct_view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("struct"));
    EXPECT_EQ(struct_view_items.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(struct_view_items.at(2)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(struct_view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));

    // row corresponding to the scalar beneath
    auto scalar_view_items = strategy.ConstructRow(item.GetAnyValueItem()->GetChildren().at(0));
    ASSERT_EQ(scalar_view_items.size(), 4);
    EXPECT_EQ(scalar_view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("signed"));
    EXPECT_EQ(scalar_view_items.at(1)->Data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(scalar_view_items.at(2)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(scalar_view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));
  }

  {  // single local variable with a struct (constructed as VariableInfo)
    auto item = CreateVariableInfoItem(domainconstants::kLocalVariableType);

    const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
    SetAnyValue(anyvalue, *item);
    item->SetName("abc");

    // row corresponding to the variable itself
    VariableTableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(item.get());
    ASSERT_EQ(view_items.size(), 4);
    EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
    // value column should contain a placeholder
    EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString("---"));
    EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));

    // row corresponding to the structure beneath
    auto struct_view_items = strategy.ConstructRow(item->GetAnyValueItem());
    ASSERT_EQ(struct_view_items.size(), 4);
    EXPECT_EQ(struct_view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("struct"));
    EXPECT_EQ(struct_view_items.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(struct_view_items.at(2)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(struct_view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));

    // row corresponding to the scalar beneath
    auto scalar_view_items = strategy.ConstructRow(item->GetAnyValueItem()->GetChildren().at(0));
    ASSERT_EQ(scalar_view_items.size(), 4);
    EXPECT_EQ(scalar_view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("signed"));
    EXPECT_EQ(scalar_view_items.at(1)->Data(Qt::DisplayRole).toInt(), 42);
    EXPECT_EQ(scalar_view_items.at(2)->Data(Qt::DisplayRole).toString(), QString(""));
    EXPECT_EQ(scalar_view_items.at(3)->Data(Qt::DisplayRole).toString(), QString(""));
  }
}

TEST_F(CustomRowStrategiesTest, InstructionEditorRowStrategy)
{
  {
    SequenceItem item;
    item.SetName("abc");
    InstructionEditorRowStrategy strategy;

    auto view_items = strategy.ConstructRow(&item);

    ASSERT_EQ(view_items.size(), 2);

    EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString(), QString("Sequence"));
    EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toString(), QString("abc"));
  }
}

}  // namespace oac_tree_gui::test
