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

#include "oac_tree_gui/viewmodel/attribute_editor_viewmodel.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for AttributeEditorViewModel class.
 */
class AttributeEditorViewModelTest : public ::testing::Test
{
public:
  //! Returns the number of rows looking at the given item.
  static int CountRowsOf(const AttributeEditorViewModel& viewmodel, const mvvm::SessionItem* item)
  {
    int result{0};
    for (int row = 0; row < viewmodel.rowCount(QModelIndex()); ++row)
    {
      if (viewmodel.GetSessionItemFromIndex(viewmodel.index(row, 0, QModelIndex())) == item)
      {
        ++result;
      }
    }
    return result;
  }
};

//! Replacing AnyValueItem of the instruction, the way the instruction editor does after editing.
TEST_F(AttributeEditorViewModelTest, ReplaceAnyValueItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  mvvm::ApplicationModel model;
  auto instruction = InsertInstruction(domainconstants::kPvAccessWriteInstructionType,
                                       model.GetRootItem());

  AttributeEditorViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(instruction);

  auto previous_anyvalue = GetAnyValueItem(*instruction);
  ASSERT_NE(previous_anyvalue, nullptr);
  EXPECT_EQ(CountRowsOf(viewmodel, previous_anyvalue), 1);
  const int row_count = viewmodel.rowCount(QModelIndex());

  model.RemoveItem(previous_anyvalue);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), row_count - 1);

  auto new_anyvalue = model.InsertItem(CreateEmptyAnyValueItem(), instruction->GetAttributes(),
                                       {itemconstants::kAnyValueTag, 0});

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), row_count);
  EXPECT_EQ(CountRowsOf(viewmodel, new_anyvalue), 1);
}

}  // namespace oac_tree_gui::test
