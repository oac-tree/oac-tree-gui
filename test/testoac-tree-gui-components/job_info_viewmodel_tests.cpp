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

#include "oac_tree_gui/viewmodel/job_info_viewmodel.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for JobInfoViewModel class.
 */
class JobInfoViewModelTest : public ::testing::Test
{
};

TEST_F(JobInfoViewModelTest, InitialState)
{
  const JobInfoViewModel model;

  EXPECT_EQ(model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(model.columnCount(QModelIndex()), 0);
}

TEST_F(JobInfoViewModelTest, PopulateModel)
{
  JobInfoViewModel model;

  model.PopulateModel(std::vector<std::string>({"job1", "job2"}));

  EXPECT_EQ(model.rowCount(QModelIndex()), 2);
  EXPECT_EQ(model.columnCount(QModelIndex()), 2);

  // first column contains the job index, second column contains the job name
  EXPECT_EQ(model.data(model.index(0, 0, QModelIndex())).toString(), QString("0"));
  EXPECT_EQ(model.data(model.index(0, 1, QModelIndex())).toString(), QString("job1"));
  EXPECT_EQ(model.data(model.index(1, 0, QModelIndex())).toString(), QString("1"));
  EXPECT_EQ(model.data(model.index(1, 1, QModelIndex())).toString(), QString("job2"));
}

TEST_F(JobInfoViewModelTest, PopulateModelClearsPreviousContent)
{
  JobInfoViewModel model;

  model.PopulateModel(std::vector<std::string>({"job1", "job2"}));
  model.PopulateModel(std::vector<std::string>({"job3"}));

  EXPECT_EQ(model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(model.columnCount(QModelIndex()), 2);

  EXPECT_EQ(model.data(model.index(0, 0, QModelIndex())).toString(), QString("0"));
  EXPECT_EQ(model.data(model.index(0, 1, QModelIndex())).toString(), QString("job3"));
}

}  // namespace oac_tree_gui::test
