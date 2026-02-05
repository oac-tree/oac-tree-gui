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

#include "oac_tree_gui/viewmodel/flatlist_viewmodel.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{
std::int32_t kExpectedColumnCount = 2;  // as defined in LabelDataRowStrategy
}

/**
 * @brief Tests for FlatListViewModelTest class.
 */
class FlatListViewModelTest : public ::testing::Test
{
public:
  FlatListViewModelTest() = default;

  mvvm::ApplicationModel m_model;
};

TEST_F(FlatListViewModelTest, InitialState)
{
  {  // no model defined
    const FlatListViewModel view_model(nullptr);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }

  {
    const FlatListViewModel view_model(&m_model);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }
}

TEST_F(FlatListViewModelTest, InstructionWithChildren)
{
  const FlatListViewModel view_model(&m_model);

  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait = m_model.InsertItem<WaitItem>(sequence0);
  auto message0 = m_model.InsertItem<RepeatItem>();

  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);

  auto sequence_index = view_model.index(0, 0);
  EXPECT_EQ(view_model.data(sequence_index, Qt::DisplayRole).toString().toStdString(),
            mvvm::GetTypeName<SequenceItem>());

  // children are not seen
  EXPECT_EQ(view_model.rowCount(sequence_index), 0);
  EXPECT_EQ(view_model.columnCount(sequence_index), kExpectedColumnCount);
}

TEST_F(FlatListViewModelTest, MixtureOfItemsOfVeryDifferentType)
{
  const FlatListViewModel view_model(&m_model);

  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait = m_model.InsertItem<WaitItem>(sequence0);
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto job = m_model.InsertItem<LocalJobItem>();

  EXPECT_EQ(view_model.rowCount(), 3); // sequence, procedure, job
  EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
}

TEST_F(FlatListViewModelTest, ContainerChange)
{
  auto container0 = m_model.InsertItem<mvvm::ContainerItem>();
  auto job0 = m_model.InsertItem<LocalJobItem>(container0);
  auto job1 = m_model.InsertItem<LocalJobItem>(container0);
  auto container1 = m_model.InsertItem<mvvm::ContainerItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(container1);
  auto wait1 = m_model.InsertItem<WaitItem>(container1);
  auto wait2 = m_model.InsertItem<WaitItem>(container1);

  FlatListViewModel view_model(nullptr);
  view_model.SetRootSessionItem(container0);
  EXPECT_EQ(view_model.rowCount(), 2);
  view_model.SetRootSessionItem(container1);
  EXPECT_EQ(view_model.rowCount(), 3);
}

}  // namespace oac_tree_gui::test
