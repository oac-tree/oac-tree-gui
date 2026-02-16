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

#include "oac_tree_gui/viewmodel/instruction_editor_viewmodel.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/providers/qtcore_helper.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods from drag_and_drop_helper.h
 */
class DragAndDropHelperTest : public ::testing::Test
{
public:
  DragAndDropHelperTest() = default;

  mvvm::ApplicationModel m_model;
};

TEST_F(DragAndDropHelperTest, GetStringListFromMime)
{
  const QString test_mime("test_mime");

  {  // empty data
    const QMimeData mime_data;
    EXPECT_TRUE(GetStringListFromMime(&mime_data, test_mime).empty());
  }

  {  // two elements
    const QStringList list({"abc", "def"});
    auto mime_data = std::make_unique<QMimeData>();
    mime_data->setData(test_mime, mvvm::utils::GetByteArray(list));

    auto result = GetStringListFromMime(mime_data.get(), test_mime);
    EXPECT_EQ(result, std::vector<std::string>({"abc", "def"}));
  }
}

TEST_F(DragAndDropHelperTest, CreateItemIdentifierMimeData)
{
  const QString mime_type("mime_type");
  {  // empty list
    auto mime_data = CreateItemIdentifierMimeData({}, mime_type);
    auto identifiers = GetStringListFromMime({}, mime_type);
    EXPECT_TRUE(identifiers.empty());
  }

  {  // single instruction
    const mvvm::AllItemsViewModel view_model(&m_model);
    auto sequence = m_model.InsertItem<SequenceItem>();
    auto sequence_displayname_index = view_model.index(0, 0);

    auto mime_data = CreateItemIdentifierMimeData({sequence_displayname_index}, mime_type);

    auto identifiers = GetStringListFromMime(mime_data.get(), mime_type);
    EXPECT_EQ(identifiers, std::vector<std::string>({sequence->GetIdentifier()}));
  }
}

TEST_F(DragAndDropHelperTest, CreateItemIdentifierMimeDataForVectorItem)
{
  const QString mime_type("mime_type");
  const mvvm::AllItemsViewModel view_model(&m_model);
  auto vector_item = m_model.InsertItem<mvvm::VectorItem>();
  auto vector_item_name_index = view_model.index(0, 0);

  auto x_item = vector_item->GetItem(mvvm::VectorItem::kX);
  auto x_item_display_index = view_model.index(0, 0, vector_item_name_index);
  auto x_item_value_index = view_model.index(0, 1, vector_item_name_index);

  auto mime_data = CreateItemIdentifierMimeData(
      {vector_item_name_index, x_item_display_index, x_item_value_index}, mime_type);

  auto identifiers = GetStringListFromMime(mime_data.get(), mime_type);
  EXPECT_EQ(identifiers,
            std::vector<std::string>({vector_item->GetIdentifier(), x_item->GetIdentifier()}));
}

TEST_F(DragAndDropHelperTest, GetNewInstructionType)
{
  const QMimeData mime_data;
  EXPECT_TRUE(GetNewInstructionType(&mime_data).empty());
  // more tests below
}

TEST_F(DragAndDropHelperTest, CreateNewInstructionMimeData)
{
  EXPECT_EQ(GetNewInstructionType(nullptr), std::string(""));

  auto mime_data = CreateNewInstructionMimeData("abc");
  EXPECT_EQ(GetNewInstructionType(mime_data.get()), std::string("abc"));

  mime_data = CreateNewInstructionMimeData("");
  EXPECT_EQ(GetNewInstructionType(mime_data.get()), std::string(""));
}

TEST_F(DragAndDropHelperTest, GetInternalMoveTagIndexLastWaitToVariousPlaces)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence0);
  auto sequence1 = m_model.InsertItem<SequenceItem>();
  auto wait3 = m_model.InsertItem<WaitItem>(sequence1);

  // Positions of drop indicator and reported parameters of canDropMimeData:
  //
  // [0 ]  --------------   row_col=( 0,  0)    QModelIndex(-1, -1)   Container
  // [1 ]  sequence0        row_col=(-1, -1)    QModelIndex(0, 0)
  // [2 ]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
  // [3 ]      -----------  row_col=( 0,  0)    QModelIndex(0, 0)     Sequence
  // [4 ]      Wait0        row_col=(-1, -1)    QModelIndex(0, 0)
  // [5 ]      -----------  row_col=( 1,  0)    QModelIndex(0, 0)     Sequence
  // [6 ]      Wait1        row_col=(-1, -1)    QModelIndex(1, 0)
  // [7 ]      -----------  row_col=( 2,  0)    QModelIndex(0, 0)     Sequence
  // [8 ]      Wait2        row_col=(-1, -1)    QModelIndex(2, 0)
  // [9 ]      -----------  row_col=( 3,  0)    QModelIndex(0, 0)     Sequence
  // [10]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
  // [11]  sequence1        row_col=(-1, -1)    QModelIndex(1, 0)
  // [12]  --------------   row_col=( 2,  0)    QModelIndex(-1, -1)   Container
  // [13]     -----------   row_col=( 0,  0)    QModelIndex(1, 0)     Sequence
  // [14]     Wait3         row_col=(-1, -1)    QModelIndex(0, 0)
  // [15]     -----------   row_col=( 1,  0)    QModelIndex(1, 0)     Sequence
  //
  // [viewport]             row_col=(-1, -1)    QModelIndex(-1, -1)

  const mvvm::SessionItem* item_to_move = wait2;

  {  // moving wait2 to [viewport]
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = m_model.GetRootItem();
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [3]
    const std::int32_t drop_indicator = 0;
    const mvvm::SessionItem* parent = sequence0;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(0);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [4]
    // it is not valid to try to drop child onto child, but we still expect correct indexes reported
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = wait0;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [5]
    const std::int32_t drop_indicator = 1;
    const mvvm::SessionItem* parent = sequence0;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(1);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [6]
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = wait1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [10]
    const std::int32_t drop_indicator = 1;
    const mvvm::SessionItem* parent = m_model.GetRootItem();
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(1);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [11]
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = sequence1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [12]
    const std::int32_t drop_indicator = 2;
    const mvvm::SessionItem* parent = m_model.GetRootItem();
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(2);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [13]
    const std::int32_t drop_indicator = 0;
    const mvvm::SessionItem* parent = sequence1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(0);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait2 to [15]
    const std::int32_t drop_indicator = 1;
    const mvvm::SessionItem* parent = sequence1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(1);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }
}

TEST_F(DragAndDropHelperTest, GetInternalMoveTagIndexFirstWaitToVariousPlaces)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence0);
  auto sequence1 = m_model.InsertItem<SequenceItem>();
  auto wait3 = m_model.InsertItem<WaitItem>(sequence1);

  // Positions of drop indicator and reported parameters of canDropMimeData:
  //
  // [0 ]  --------------   row_col=( 0,  0)    QModelIndex(-1, -1)   Container
  // [1 ]  sequence0        row_col=(-1, -1)    QModelIndex(0, 0)
  // [2 ]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
  // [3 ]      -----------  row_col=( 0,  0)    QModelIndex(0, 0)     Sequence
  // [4 ]      Wait0        row_col=(-1, -1)    QModelIndex(0, 0)
  // [5 ]      -----------  row_col=( 1,  0)    QModelIndex(0, 0)     Sequence
  // [6 ]      Wait1        row_col=(-1, -1)    QModelIndex(1, 0)
  // [7 ]      -----------  row_col=( 2,  0)    QModelIndex(0, 0)     Sequence
  // [8 ]      Wait2        row_col=(-1, -1)    QModelIndex(2, 0)
  // [9 ]      -----------  row_col=( 3,  0)    QModelIndex(0, 0)     Sequence
  // [10]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
  // [11]  sequence1        row_col=(-1, -1)    QModelIndex(1, 0)
  // [12]  --------------   row_col=( 2,  0)    QModelIndex(-1, -1)   Container
  // [13]     -----------   row_col=( 0,  0)    QModelIndex(1, 0)     Sequence
  // [14]     Wait3         row_col=(-1, -1)    QModelIndex(0, 0)
  // [15]     -----------   row_col=( 1,  0)    QModelIndex(1, 0)     Sequence
  //
  // [viewport]             row_col=(-1, -1)    QModelIndex(-1, -1)

  const mvvm::SessionItem* item_to_move = wait0;

  {  // moving wait0 to [7]
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = wait1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait0 to [9]
    const std::int32_t drop_indicator = 3;
    const mvvm::SessionItem* parent = sequence0;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(3);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait0 to [10]
    const std::int32_t drop_indicator = 1;
    const mvvm::SessionItem* parent = m_model.GetRootItem();
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Default(1);
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }

  {  // moving wait0 to [11]
    const std::int32_t drop_indicator = -1;
    const mvvm::SessionItem* parent = sequence1;
    const mvvm::TagIndex expected_tag = mvvm::TagIndex::Append();
    EXPECT_EQ(GetInternalMoveTagIndexV2(drop_indicator, *item_to_move, *parent), expected_tag);
  }
}

TEST_F(DragAndDropHelperTest, GetDropTagIndex)
{
  // item is hovered on top of another item
  EXPECT_TRUE(GetDropTagIndex(-1) == mvvm::TagIndex("", 0));
  EXPECT_TRUE(GetDropTagIndex(42) == mvvm::TagIndex("", 42));
}

TEST_F(DragAndDropHelperTest, CanInsertType)
{
  auto sequence = m_model.InsertItem<SequenceItem>();
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_FALSE(mvvm::utils::GetInsertTypeErrorCode(domainconstants::kWaitInstructionType, *sequence,
                                                   mvvm::TagIndex::Default(0)));
  EXPECT_TRUE(mvvm::utils::GetInsertTypeErrorCode(domainconstants::kWaitInstructionType, *wait,
                                                  mvvm::TagIndex::Default(0)));
}

TEST_F(DragAndDropHelperTest, CreateInstructionFromMime)
{
  const QString mime_type = kCopyInstructionMimeType;

  const std::string expected_name("abc");
  WaitItem item;
  item.SetDisplayName(expected_name);

  auto data = sup::gui::CreateCopyMimeData(item, mime_type);
  EXPECT_TRUE(data->hasFormat(mime_type));

  auto reconstructed_item = sup::gui::CreateSessionItem(data.get(), mime_type);
  EXPECT_EQ(reconstructed_item->GetDisplayName(), expected_name);
  EXPECT_NE(dynamic_cast<WaitItem*>(reconstructed_item.get()), nullptr);
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveRowForFirstItem)
{
  // pretending to move first item
  const std::int32_t source_row = 0;

  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)     -> 0
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)       -> 0
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)     -> 0
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)       -> 1
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)     -> 1
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)       -> 2
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)     -> 2

  {  // drop on viewport
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index),
              kViewportDropIndex);
  }

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveRowForMiddleItem)
{
  // pretending to move middle item
  const std::int32_t source_row = 1;

  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)  -> 0
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)    -> 0
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)  -> 1
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)    -> 1
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)  -> 1
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)    -> 2
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)  -> 2

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveRowForLastItem)
{
  // pretending to move last item
  const std::int32_t source_row = 2;

  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 1);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(GetListInternalMoveIndex(drop_indicator, source_row, parent_index), 2);
  }
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveTagIndexForFirstItem)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  // pretending to move first item
  const mvvm::TagIndex source_tag_index = procedure0->GetTagIndex();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)

  {  // drop on viewport itself, outside any item -> should lead to append
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index;  // invalid
    // expecting index denoting append
    const auto expected_tag_index = mvvm::TagIndex::Append(procedure0->GetTagIndex().GetTag());
    EXPECT_EQ(GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index),
              expected_tag_index);
  }

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 2);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 2);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 3);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 3);
  }
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveTagIndexForMiddleItem)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  // pretending to move middle item
  const mvvm::TagIndex source_tag_index = procedure1->GetTagIndex();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 1);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 1);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 1);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 3);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 3);
  }
}

TEST_F(DragAndDropHelperTest, GetListInternalMoveTagIndexForLastItem)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>();
  auto procedure1 = m_model.InsertItem<ProcedureItem>();
  auto procedure2 = m_model.InsertItem<ProcedureItem>();

  // pretending to move last item
  const mvvm::TagIndex source_tag_index = procedure2->GetTagIndex();

  const mvvm::TopItemsViewModel view_model(&m_model);
  auto procedure0_index = view_model.index(0, 0);
  auto procedure1_index = view_model.index(1, 0);
  auto procedure2_index = view_model.index(2, 0);

  // [0]  ----------  row_col=( 0,  0)  QModelIndex(-1, -1)
  // [1]  procedure0  row_col=(-1, -1)  QModelIndex(0, 0)
  // [2]  ----------  row_col=( 1,  0)  QModelIndex(-1, -1)
  // [3]  procedure1  row_col=(-1, -1)  QModelIndex(1, 0)
  // [4]  ----------  row_col=( 2,  0)  QModelIndex(-1, -1)
  // [5]  procedure2  row_col=(-1, -1)  QModelIndex(2, 0)
  // [6]  ----------  row_col=( 3,  0)  QModelIndex(-1, -1)

  {  // hovering case #0
    const std::int32_t drop_indicator = 0;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #1
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure0_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 0);
  }

  {  // hovering case #2
    const std::int32_t drop_indicator = 1;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 1);
  }

  {  // hovering case #3
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure1_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 1);
  }

  {  // hovering case #4
    const std::int32_t drop_indicator = 2;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 2);
  }

  {  // hovering case #5
    const std::int32_t drop_indicator = -1;
    const QModelIndex parent_index = procedure2_index;
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 2);
  }

  {  // hovering case #6
    const std::int32_t drop_indicator = 3;
    const QModelIndex parent_index;  // invalid
    EXPECT_EQ(
        GetListInternalMoveTagIndex(drop_indicator, source_tag_index, parent_index).GetIndex(), 2);
  }
}

}  // namespace oac_tree_gui::test
