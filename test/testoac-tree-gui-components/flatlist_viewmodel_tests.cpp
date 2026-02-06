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

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QMimeData>

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

  // two children, sequence0 and message0, at top level
  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);

  auto sequence_index = view_model.index(0, 0);
  EXPECT_EQ(view_model.data(sequence_index, Qt::DisplayRole).toString().toStdString(),
            mvvm::GetTypeName<SequenceItem>());

  // children of sequence are hidden
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

  EXPECT_EQ(view_model.rowCount(), 3);  // sequence, procedure, job
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

TEST_F(FlatListViewModelTest, FlagsForDragAndDrop)
{
  auto wait0 = m_model.InsertItem<WaitItem>();
  const FlatListViewModel model(&m_model);

  // Valid index should have drag and drop enabled
  Qt::ItemFlags valid_flags = model.flags(model.index(0, 0));
  EXPECT_TRUE(valid_flags & Qt::ItemIsEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsSelectable);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDragEnabled);
  EXPECT_TRUE(valid_flags & Qt::ItemIsDropEnabled);

  // Invalid index (root) should only have drop enabled
  const Qt::ItemFlags invalid_flags = model.flags(QModelIndex());
  EXPECT_TRUE(invalid_flags & Qt::ItemIsDropEnabled);
  EXPECT_FALSE(invalid_flags & Qt::ItemIsDragEnabled);
}

TEST_F(FlatListViewModelTest, SupportedActions)
{
  const FlatListViewModel model(&m_model);

  EXPECT_TRUE(model.supportedDragActions() & Qt::MoveAction);
  EXPECT_TRUE(model.supportedDragActions() & Qt::CopyAction);
  EXPECT_TRUE(model.supportedDropActions() & Qt::MoveAction);
  EXPECT_TRUE(model.supportedDropActions() & Qt::CopyAction);
}

TEST_F(FlatListViewModelTest, MimeTypes)
{
  const FlatListViewModel model(&m_model);

  QStringList mime_types = model.mimeTypes();
  EXPECT_EQ(mime_types.size(), 1);
  EXPECT_EQ(mime_types[0], kItemIdentifierMimeType);
}

TEST_F(FlatListViewModelTest, MimeDataEncoding)
{
  const FlatListViewModel model(&m_model);

  auto procedure_item = m_model.InsertItem<ProcedureItem>();
  auto display_index = model.index(0, 0);
  auto data_index = model.index(0, 1);

  std::unique_ptr<QMimeData> mime_data(model.mimeData({display_index, data_index}));
  EXPECT_NE(mime_data, nullptr);

  EXPECT_TRUE(mime_data->hasFormat(kItemIdentifierMimeType));

  auto identifiers = GetStringListFromMime(mime_data.get(), kItemIdentifierMimeType);
  EXPECT_EQ(identifiers.size(), 1);
  EXPECT_EQ(identifiers.at(0), procedure_item->GetIdentifier());
}

/*
 This is what canDropMimeData reports when we drag an item

[0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
[1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
[2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
[3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
[4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
*/
TEST_F(FlatListViewModelTest, CanDropMimeData)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();

  const FlatListViewModel view_model(&m_model);

  {  // nullptr data
    EXPECT_FALSE(view_model.canDropMimeData(nullptr, Qt::MoveAction, -1, -1, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(nullptr, Qt::CopyAction, -1, -1, QModelIndex()));
  }

  {  // mime data of wrong type
    auto mime_data = std::make_unique<QMimeData>();
    mime_data->setData("wrong/type", QByteArray());
    EXPECT_FALSE(
        view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1, QModelIndex()));
    EXPECT_FALSE(
        view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1, QModelIndex()));
  }

  {  // all combinations of QModelIndex and row from the table above should be accepted
    auto mime_data = std::make_unique<QMimeData>();
    mime_data->setData(kItemIdentifierMimeType, QByteArray());

    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(0, 0)));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(1, 0)));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 2, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(2, 0)));

    // copy is not yet implemented, so it should be rejected
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 0, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(0, 0)));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 1, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(1, 0)));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 2, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(2, 0)));
  }
}


}  // namespace oac_tree_gui::test
