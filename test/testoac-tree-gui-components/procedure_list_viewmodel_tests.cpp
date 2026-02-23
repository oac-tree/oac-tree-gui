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

#include "oac_tree_gui/viewmodel/procedure_list_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

namespace
{
const std::int32_t kExpectedColumnCount = 2;  // as defined in LabelDataRowStrategy
}

/**
 * @brief Tests for ProcedureListViewModel class.
 */
class ProcedureListViewModelTest : public ::testing::Test
{
public:
  ProcedureListViewModelTest() = default;

  mvvm::ApplicationModel m_model;
};

TEST_F(ProcedureListViewModelTest, InitialState)
{
  {  // no model defined
    const ProcedureListViewModel view_model(nullptr);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }

  {
    const ProcedureListViewModel view_model(&m_model);
    EXPECT_EQ(view_model.rowCount(), 0);
    EXPECT_EQ(view_model.columnCount(), kExpectedColumnCount);
  }
}

TEST_F(ProcedureListViewModelTest, ContainerChange)
{
  auto container0 = m_model.InsertItem<mvvm::ContainerItem>();

  auto procedure0 = m_model.InsertItem<ProcedureItem>(container0);
  auto procedure1 = m_model.InsertItem<ProcedureItem>(container0);

  auto container1 = m_model.InsertItem<mvvm::ContainerItem>();

  auto procedure2 = m_model.InsertItem<ProcedureItem>(container1);
  auto procedure3 = m_model.InsertItem<ProcedureItem>(container1);
  auto procedure4 = m_model.InsertItem<ProcedureItem>(container1);

  ProcedureListViewModel view_model(nullptr);
  view_model.SetRootSessionItem(container0);
  EXPECT_EQ(view_model.rowCount(), 2);
  view_model.SetRootSessionItem(container1);
  EXPECT_EQ(view_model.rowCount(), 3);
}

TEST_F(ProcedureListViewModelTest, FlagsForDragAndDrop)
{
  auto wait0 = m_model.InsertItem<ProcedureItem>();
  const ProcedureListViewModel model(&m_model);

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

TEST_F(ProcedureListViewModelTest, SupportedActions)
{
  const ProcedureListViewModel model(&m_model);

  EXPECT_TRUE(model.supportedDragActions() & Qt::MoveAction);
  EXPECT_TRUE(model.supportedDragActions() & Qt::CopyAction);
  EXPECT_TRUE(model.supportedDropActions() & Qt::MoveAction);
  EXPECT_FALSE(model.supportedDropActions() & Qt::CopyAction);
}

TEST_F(ProcedureListViewModelTest, MimeTypes)
{
  const ProcedureListViewModel model(&m_model);

  QStringList mime_types = model.mimeTypes();
  EXPECT_EQ(mime_types.size(), 1);
  EXPECT_EQ(mime_types[0], kProcedureIdentifierMimeType);
}

TEST_F(ProcedureListViewModelTest, MimeDataEncoding)
{
  const ProcedureListViewModel model(&m_model);

  auto procedure_item = m_model.InsertItem<ProcedureItem>();
  auto display_index = model.index(0, 0);
  auto data_index = model.index(0, 1);

  std::unique_ptr<QMimeData> mime_data(model.mimeData({display_index, data_index}));
  EXPECT_NE(mime_data, nullptr);

  EXPECT_TRUE(mime_data->hasFormat(kProcedureIdentifierMimeType));

  auto identifiers = GetStringListFromMime(mime_data.get(), kProcedureIdentifierMimeType);
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
TEST_F(ProcedureListViewModelTest, CanDropMimeData)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();

  const ProcedureListViewModel view_model(&m_model);

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
    mime_data->setData(kProcedureIdentifierMimeType, QByteArray());

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

TEST_F(ProcedureListViewModelTest, DragProcedureFromFirstPositionToLast)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  ProcedureListViewModel view_model(&m_model);

  auto procedure0_index = view_model.index(0, 0);

  const std::unique_ptr<QMimeData> mime_data(view_model.mimeData({procedure0_index}));

  // pretending to drop after the last item
  const std::int32_t drop_indicator_row = 3;
  const QModelIndex parent_index = QModelIndex();  // invalid

  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({procedure1, procedure2, procedure0}));
}

TEST_F(ProcedureListViewModelTest, DragLastProcedureOnTopOfFirst)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  ProcedureListViewModel view_model(&m_model);

  auto procedure0_index = view_model.index(0, 0);
  auto procedure2_index = view_model.index(2, 0);

  // pretending to drop on top of the first item
  const std::int32_t drop_indicator_row = -1;
  const QModelIndex parent_index = procedure0_index;

  const std::unique_ptr<QMimeData> mime_data(view_model.mimeData({procedure2_index}));
  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));

  // procedure2 should appear on the first place
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({procedure2, procedure0, procedure1}));
}

}  // namespace oac_tree_gui::test
