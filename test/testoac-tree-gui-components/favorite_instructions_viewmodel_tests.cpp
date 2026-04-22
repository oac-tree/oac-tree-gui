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
#include <oac_tree_gui/composer/instruction_copy_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/instruction_editor_viewmodel.h>
#include <oac_tree_gui/model/item_factory.h>

#include <sup/gui/components/mime_conversion_helper.h>

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
    EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
    EXPECT_EQ(view_model.columnCount(QModelIndex()), kExpectedColumnCount);
  }

  {
    const FavoriteInstructionsViewModel view_model(&m_model);
    EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
    EXPECT_EQ(view_model.columnCount(QModelIndex()), kExpectedColumnCount);
  }
}

TEST_F(FavoriteInstructionsViewModelTest, ViewmodelWithItems)
{
  m_model.InsertItem<SequenceItem>();
  m_model.InsertItem<WaitItem>();

  const FavoriteInstructionsViewModel view_model(&m_model);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 2);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), kExpectedColumnCount);
}

TEST_F(FavoriteInstructionsViewModelTest, FlagsForDragAndDrop)
{
  auto wait0 = m_model.InsertItem<SequenceItem>();
  const FavoriteInstructionsViewModel model(&m_model);

  // Valid index should have drag and drop enabled
  const Qt::ItemFlags valid_flags = model.flags(model.index(0, 0, QModelIndex()));
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

  const QStringList expected_mime_types = {kNewInstructionMimeType, kInstructionIdentifierMimeType,
                                           kCopyInstructionMimeType};
  EXPECT_EQ(expected_mime_types, model.mimeTypes());
}

TEST_F(FavoriteInstructionsViewModelTest, MimeDataEncoding)
{
  const FavoriteInstructionsViewModel model(&m_model);

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto display_index = model.index(0, 0, QModelIndex());
  auto data_index = model.index(0, 1, QModelIndex());

  std::unique_ptr<QMimeData> mime_data(model.mimeData({display_index, data_index}));
  EXPECT_NE(mime_data, nullptr);

  // instruction identifier
  EXPECT_TRUE(mime_data->hasFormat(kInstructionIdentifierMimeType));
  auto identifiers = sup::gui::GetStringListFromMime(*mime_data, kInstructionIdentifierMimeType);
  EXPECT_EQ(identifiers.size(), 1);
  EXPECT_EQ(identifiers.at(0), sequence_item->GetIdentifier());

  // and full copy of instruction
  EXPECT_TRUE(mime_data->hasFormat(kCopyInstructionMimeType));
  auto copied_items = sup::gui::CreateSessionItems(*mime_data, kCopyInstructionMimeType);
  EXPECT_EQ(copied_items.size(), 1);
  EXPECT_EQ(copied_items.at(0)->GetType(), sequence_item->GetDomainType());
}

/*
 This is what canDropMimeData reports when we drag an item

[0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
[1]  sequence0   row_col=(-1, -1)  QModelIndex(0, 0)
[2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
[3]  sequence1   row_col=(-1, -1)  QModelIndex(1, 0)
[4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
*/
TEST_F(FavoriteInstructionsViewModelTest, CanDropMimeData)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto sequence1 = m_model.InsertItem<SequenceItem>();

  const FavoriteInstructionsViewModel view_model(&m_model);

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
    mime_data->setData(kInstructionIdentifierMimeType, QByteArray());

    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 0, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(0, 0, QModelIndex())));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 1, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(1, 0, QModelIndex())));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, 2, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::MoveAction, -1, -1,
                                           view_model.index(2, 0, QModelIndex())));

    // copy while own reordering
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 0, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(0, 0, QModelIndex())));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 1, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(1, 0, QModelIndex())));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, 2, 0, QModelIndex()));
    EXPECT_FALSE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, -1, -1,
                                            view_model.index(2, 0, QModelIndex())));

    auto external_mime_data = std::make_unique<QMimeData>();
    external_mime_data->setData(kCopyInstructionMimeType, QByteArray());
    EXPECT_TRUE(
        view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, 0, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, -1, -1,
                                           view_model.index(0, 0, QModelIndex())));
    EXPECT_TRUE(
        view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, 1, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, -1, -1,
                                           view_model.index(1, 0, QModelIndex())));
    EXPECT_TRUE(
        view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, 2, 0, QModelIndex()));
    EXPECT_TRUE(view_model.canDropMimeData(external_mime_data.get(), Qt::CopyAction, -1, -1,
                                           view_model.index(2, 0, QModelIndex())));
  }
}

TEST_F(FavoriteInstructionsViewModelTest, DragSequenceFromFirstPositionToLast)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto sequence1 = m_model.InsertItem<SequenceItem>();
  auto sequence2 = m_model.InsertItem<SequenceItem>();

  FavoriteInstructionsViewModel view_model(&m_model);

  auto sequence0_index = view_model.index(0, 0, QModelIndex());

  const std::unique_ptr<QMimeData> mime_data(view_model.mimeData({sequence0_index}));

  // pretending to drop after the last item
  const std::int32_t drop_indicator_row = 3;
  const QModelIndex parent_index = QModelIndex();  // invalid

  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({sequence1, sequence2, sequence0}));
}

TEST_F(FavoriteInstructionsViewModelTest, DragSequenceFromFirstPositionOnTopOfLast)
{
  // Droping on top for FavoriteInstructionsViewModel doesn't lead to insert into, as in
  // InstructionEditorViewModel. It leads to insert before, as in previous test

  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto sequence1 = m_model.InsertItem<SequenceItem>();
  auto sequence2 = m_model.InsertItem<SequenceItem>();

  FavoriteInstructionsViewModel view_model(&m_model);

  auto sequence0_index = view_model.index(0, 0, QModelIndex());
  auto sequence2_index = view_model.index(2, 0, QModelIndex());

  const std::unique_ptr<QMimeData> mime_data(view_model.mimeData({sequence0_index}));

  // pretending to drop on top of the last item
  const std::int32_t drop_indicator_row = -1;
  const QModelIndex parent_index = sequence2_index;

  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({sequence1, sequence0, sequence2}));
}

TEST_F(FavoriteInstructionsViewModelTest, DropNewType)
{
  FavoriteInstructionsViewModel view_model(&m_model);

  {  // drop on empty viewport
    auto mime_data =
        CreateNewInstructionMimeData(QString::fromStdString(domainconstants::kWaitInstructionType));
    const std::int32_t drop_indicator_row = -1;
    const QModelIndex parent_index = QModelIndex();  // invalid

    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                           parent_index));
    EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                        parent_index));

    EXPECT_EQ(m_model.GetRootItem()->GetAllItems().size(), 1);
    auto inserted_item = m_model.GetRootItem()->GetAllItems().front();
    EXPECT_EQ(inserted_item->GetType(), mvvm::GetTypeName<WaitItem>());
  }

  {  // drop after the last item
    auto mime_data = CreateNewInstructionMimeData(
        QString::fromStdString(domainconstants::kSequenceInstructionType));
    const std::int32_t drop_indicator_row = 1;       // after the only existing item
    const QModelIndex parent_index = QModelIndex();  // invalid

    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                           parent_index));
    EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                        parent_index));

    EXPECT_EQ(m_model.GetRootItem()->GetAllItems().size(), 2);
    auto inserted_item = m_model.GetRootItem()->GetAllItems().back();
    EXPECT_EQ(inserted_item->GetType(), mvvm::GetTypeName<SequenceItem>());
  }

  {  // drop on top of sequence should add item before (instead of insert)
    auto mime_data = CreateNewInstructionMimeData(
        QString::fromStdString(domainconstants::kIncludeInstructionType));
    const std::int32_t drop_indicator_row = -1;               // on top of the last item
    const QModelIndex parent_index = view_model.index(1, 0, QModelIndex());  // index of the first item

    EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                           parent_index));
    EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                        parent_index));

    EXPECT_EQ(m_model.GetRootItem()->GetAllItems().size(), 3);
    auto inserted_item =
        m_model.GetRootItem()->GetAllItems().at(1);  // inserted before the sequence
    EXPECT_EQ(inserted_item->GetType(), mvvm::GetTypeName<IncludeItem>());
  }
}

TEST_F(FavoriteInstructionsViewModelTest, DropCopyOnTopOfMiddleItem)
{
  auto message_instruction = CreateInstructionItem(domainconstants::kMessageInstructionType);
  auto mime_data = CreateInstructionSelectionCopyMimeData({message_instruction.get()});

  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence0 = m_model.InsertItem<SequenceItem>(container);
  auto sequence1 = m_model.InsertItem<SequenceItem>(container);
  auto sequence2 = m_model.InsertItem<SequenceItem>(container);
  FavoriteInstructionsViewModel view_model(&m_model);
  view_model.SetRootSessionItem(container);

  auto container_index = QModelIndex();
  auto sequence1_index = view_model.index(1, 0, container_index);

  // drop on top of sequence1
  const std::int32_t drop_indicator_row = -1;
  const QModelIndex parent_index = sequence1_index;
  EXPECT_TRUE(view_model.canDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                         parent_index));
  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator_row, 0,
                                      parent_index));

  EXPECT_EQ(container->GetInstructionCount(), 4);
  auto inserted_item = container->GetInstructions().at(1);  // inserted before sequence1
  EXPECT_EQ(inserted_item->GetDomainType(), domainconstants::kMessageInstructionType);
}

}  // namespace oac_tree_gui::test
