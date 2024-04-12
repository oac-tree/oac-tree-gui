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

#include "sequencergui/viewmodel/toolkit_viewmodel.h"

#include <sequencergui/viewmodel/drag_and_drop_helper.h>

#include <gtest/gtest.h>

#include <QMimeData>

using namespace sequencergui;

//! Tests for InstructionOperationViewModel class.

class InstructionOperationViewModelTest : public ::testing::Test
{
};

TEST_F(InstructionOperationViewModelTest, InitialState)
{
  const ToolKitViewModel model;

  EXPECT_EQ(model.rowCount(), 0);
  EXPECT_EQ(model.columnCount(), 0);
}

//! Validating model composition when it is initialized with the list.

TEST_F(InstructionOperationViewModelTest, PopulateFromList)
{
  ToolKitViewModel model;

  model.PopulateModel(std::vector<std::string>({"a1", "a2"}));

  EXPECT_EQ(model.rowCount(), 2);
  EXPECT_EQ(model.columnCount(), 1);

  auto index0 = model.index(0, 0, QModelIndex());
  auto index1 = model.index(1, 0, QModelIndex());

  EXPECT_EQ(model.data(index0).toString(), QString("a1"));
  EXPECT_EQ(model.data(index1).toString(), QString("a2"));
}

//! Validating mime data obtained from the model.

TEST_F(InstructionOperationViewModelTest, MimeFromList)
{
  ToolKitViewModel model;

  model.PopulateModel(std::vector<std::string>({"a1", "a2"}));

  auto index0 = model.index(0, 0, QModelIndex());

  std::unique_ptr<QMimeData> mime_data(model.mimeData({index0}));
  EXPECT_NE(mime_data.get(), nullptr);
  EXPECT_TRUE(mime_data->hasFormat(kNewInstructionMimeType));
  EXPECT_EQ(GetNewInstructionType(mime_data.get()), std::string("a1"));
}

//! Validating model composition when it is initialized from the group.

TEST_F(InstructionOperationViewModelTest, PopulateFromTree)
{
  ToolKitViewModel model;

  const std::vector<ObjectGroupInfo> group_info{
      {"group1", {"a1", "a2"}},
      {"group2", {"b1"}},
  };

  model.PopulateModel(group_info);

  EXPECT_EQ(model.rowCount(), 2);
  EXPECT_EQ(model.columnCount(), 1);

  auto index_group0 = model.index(0, 0, QModelIndex());
  auto index_group1 = model.index(1, 0, QModelIndex());

  EXPECT_EQ(model.rowCount(index_group0), 2);
  EXPECT_EQ(model.columnCount(index_group0), 1);

  auto index_a1 = model.index(0, 0, index_group0);
  auto index_a2 = model.index(1, 0, index_group0);
  auto index_b1 = model.index(0, 0, index_group1);

  EXPECT_EQ(model.data(index_group0).toString(), QString("group1"));
  EXPECT_EQ(model.data(index_a1).toString(), QString("a1"));
  EXPECT_EQ(model.data(index_a2).toString(), QString("a2"));

  EXPECT_EQ(model.data(index_group0).toString(), QString("group1"));
  EXPECT_EQ(model.data(index_a1).toString(), QString("a1"));
  EXPECT_EQ(model.data(index_a2).toString(), QString("a2"));
}

//! Validating mime data obtained from the model, when the model represents groups.

TEST_F(InstructionOperationViewModelTest, MimeFromTree)
{
  ToolKitViewModel model;

  std::vector<ObjectGroupInfo> group_info{
      {"group1", {"a1", "a2"}},
      {"group2", {"b1"}},
  };

  model.PopulateModel(group_info);

  EXPECT_EQ(model.rowCount(), 2);
  EXPECT_EQ(model.columnCount(), 1);

  auto index_group0 = model.index(0, 0, QModelIndex());
  auto index_group1 = model.index(1, 0, QModelIndex());

  EXPECT_EQ(model.rowCount(index_group0), 2);
  EXPECT_EQ(model.columnCount(index_group0), 1);

  auto index_a1 = model.index(0, 0, index_group0);
  auto index_a2 = model.index(1, 0, index_group0);
  auto index_b1 = model.index(0, 0, index_group1);

  // our groups are not draggable
  EXPECT_EQ(model.mimeData({index_group0}), nullptr);
  EXPECT_EQ(model.mimeData({index_group1}), nullptr);

  // the rest is draggable, and should report correct object type
  const std::unique_ptr<QMimeData> mime_a1(model.mimeData({index_a1}));
  EXPECT_EQ(GetNewInstructionType(mime_a1.get()), std::string("a1"));

  const std::unique_ptr<QMimeData> mime_a2(model.mimeData({index_a2}));
  EXPECT_EQ(GetNewInstructionType(mime_a2.get()), std::string("a2"));

  const std::unique_ptr<QMimeData> mime_b1(model.mimeData({index_b1}));
  EXPECT_EQ(GetNewInstructionType(mime_b1.get()), std::string("b1"));
}
