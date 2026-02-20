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

#include "oac_tree_gui/viewmodel/favorite_instructions_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

namespace
{
const std::int32_t kExpectedColumnCount = 2;  // as defined in LabelDataRowStrategy
}

/**
 * @brief Tests for FavoriteInstructionsViewModel class.
 */
class FavoriteInstructionsViewModelTest : public ::testing::Test
{
public:
  FavoriteInstructionsViewModelTest() = default;

  mvvm::ApplicationModel m_model;
};

TEST_F(FavoriteInstructionsViewModelTest, InitialState)
{
  {  // no model defined
    const FavoriteInstructionsViewModel view_model(nullptr);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }

  {
    const FavoriteInstructionsViewModel view_model(&m_model);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }
}

TEST_F(FavoriteInstructionsViewModelTest, ViewmodelWithItems)
{
  m_model.InsertItem<SequenceItem>();
  m_model.InsertItem<WaitItem>();

  const FavoriteInstructionsViewModel view_model(&m_model);
  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
}

TEST_F(FavoriteInstructionsViewModelTest, FlagsForDragAndDrop)
{
  auto wait0 = m_model.InsertItem<SequenceItem>();
  const FavoriteInstructionsViewModel model(&m_model);

  // Valid index should have drag and drop enabled
  const Qt::ItemFlags valid_flags = model.flags(model.index(0, 0));
  EXPECT_TRUE(valid_flags & Qt::ItemIsEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsSelectable);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDragEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDropEnabled);

  // Invalid index (root) should only have drop enabled
  const Qt::ItemFlags invalid_flags = model.flags(QModelIndex());
  EXPECT_TRUE(invalid_flags & Qt::ItemIsDropEnabled);
  EXPECT_FALSE(invalid_flags & Qt::ItemIsDragEnabled);
}

TEST_F(FavoriteInstructionsViewModelTest, SupportedActions)
{
  const FavoriteInstructionsViewModel model(&m_model);

  EXPECT_TRUE(model.supportedDragActions() & Qt::MoveAction);
  EXPECT_TRUE(model.supportedDragActions() & Qt::CopyAction);
  EXPECT_TRUE(model.supportedDropActions() & Qt::MoveAction);
  EXPECT_TRUE(model.supportedDropActions() & Qt::CopyAction);
}

TEST_F(FavoriteInstructionsViewModelTest, MimeTypes)
{
  const FavoriteInstructionsViewModel model(&m_model);

  QStringList mime_types = model.mimeTypes();
  EXPECT_EQ(mime_types.size(), 1);
  EXPECT_EQ(mime_types[0], kItemIdentifierMimeType);
}

TEST_F(FavoriteInstructionsViewModelTest, MimeDataEncoding)
{
  const FavoriteInstructionsViewModel model(&m_model);

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto display_index = model.index(0, 0);
  auto data_index = model.index(0, 1);

  std::unique_ptr<QMimeData> mime_data(model.mimeData({display_index, data_index}));
  EXPECT_NE(mime_data, nullptr);

  EXPECT_TRUE(mime_data->hasFormat(kItemIdentifierMimeType));

  auto identifiers = GetStringListFromMime(mime_data.get(), kItemIdentifierMimeType);
  EXPECT_EQ(identifiers.size(), 1);
  EXPECT_EQ(identifiers.at(0), sequence_item->GetIdentifier());
}

}  // namespace oac_tree_gui::test
