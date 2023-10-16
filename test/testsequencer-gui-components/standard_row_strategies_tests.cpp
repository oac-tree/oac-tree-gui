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

TEST_F(StandardRowStrategiesTest, LocalVariable)
{
  {  // single local variable
    LocalVariableItem item;
    item.SetName("abc");

    VariableRowStrategy strategy;
    auto view_items = strategy.ConstructRow(&item);
    ASSERT_EQ(view_items.size(), 2);

    EXPECT_EQ(view_items.at(0)->data(Qt::DisplayRole).toString(), QString("Local"));
    EXPECT_EQ(view_items.at(1)->data(Qt::DisplayRole).toString(), QString("abc"));
  }
}
