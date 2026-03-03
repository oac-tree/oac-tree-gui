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

#include "oac_tree_gui/composer/instruction_editor_drop_handler.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/composer/instruction_copy_helper.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/instruction_editor_viewmodel.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorDropHandlerTest for basic editing scenarios.
 */
class InstructionEditorDropHandlerTest : public ::testing::Test
{
public:
  InstructionEditorDropHandlerTest() : m_view_model(&m_model) {}

  /** @brief Helper method to create mime data for new instruction of given type. */
  std::unique_ptr<InstructionEditorDropHandler> CreateDefaultDropHandler()
  {
    auto container_callback = [this]() -> mvvm::SessionItem* { return m_model.GetRootItem(); };
    return std::make_unique<InstructionEditorDropHandler>(container_callback);
  }

  mvvm::ApplicationModel m_model;
  InstructionEditorViewModel m_view_model;
};

TEST_F(InstructionEditorDropHandlerTest, AttemptToCreateWithInitializedContext)
{
  EXPECT_THROW(InstructionEditorDropHandler({}), RuntimeException);

  auto container_callback = []() -> mvvm::SessionItem* { return nullptr; };
  EXPECT_NO_THROW((InstructionEditorDropHandler(container_callback)));
}

TEST_F(InstructionEditorDropHandlerTest, InsertNewType)
{
  // [0 ]  --------------   row_col=( 0,  0)    QModelIndex(-1, -1)   Container
  // [1 ]  sequence0        row_col=(-1, -1)    QModelIndex(0, 0)     Sequence
  // [2 ]  --------------   row_col=( 1,  0)    QModelIndex(-1, -1)   Container
  // [3 ]      -----------  row_col=( 0,  0)    QModelIndex(0, 0)     Sequence
  // [4 ]      Wait0        row_col=(-1, -1)    QModelIndex(0, 0)     Wait
  // [5 ]      -----------  row_col=( 1,  0)    QModelIndex(0, 0)     Sequence

  auto container = m_model.GetRootItem();
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence0);

  auto container_index = QModelIndex();
  auto sequence_index = m_view_model.index(0, 0);
  auto wait_index = m_view_model.index(0, 0, sequence_index);

  auto mime_data = CreateNewInstructionMimeData(domainconstants::kWaitInstructionType);
  auto handler = CreateDefaultDropHandler();

  {  // area [0]
    const std::int32_t drop_indicator = 0;
    EXPECT_TRUE(
        handler->CanDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, container_index));
    EXPECT_TRUE(
        handler->DropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, container_index));
    ASSERT_EQ(container->GetAllItems().size(), 2);
    EXPECT_EQ(container->GetAllItems().at(0)->GetDisplayName(),
              domainconstants::kWaitInstructionType);
  }

  {  // area [1]
    const std::int32_t drop_indicator = -1;
    EXPECT_TRUE(
        handler->CanDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, sequence_index));
    EXPECT_TRUE(
        handler->DropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, sequence_index));
    ASSERT_EQ(sequence0->GetInstructions().size(), 2);
    EXPECT_EQ(sequence0->GetInstructions().at(0)->GetDisplayName(),
              domainconstants::kWaitInstructionType);
    EXPECT_EQ(sequence0->GetInstructions().at(1),
              wait0);  // wait0 should be after the newly inserted item
  }

  {  // area [4]
    const std::int32_t drop_indicator = -1;
    EXPECT_FALSE(
        handler->CanDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, wait_index));
    EXPECT_FALSE(
        handler->DropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, wait_index));
  }
}

TEST_F(InstructionEditorDropHandlerTest, DropInstructionEditorMimeDataBetweenItems)
{
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

  auto container = m_model.GetRootItem();
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence0);
  auto sequence1 = m_model.InsertItem<SequenceItem>();
  auto wait3 = m_model.InsertItem<WaitItem>(sequence1);

  auto sequence0_index = m_view_model.index(0, 0);
  auto wait2_index = m_view_model.index(2, 0, sequence0_index);

  // move Wait2 to position [3]
  const std::unique_ptr<QMimeData> mime_data(m_view_model.mimeData({wait2_index}));

  auto handler = CreateDefaultDropHandler();
  const std::int32_t drop_indicator = 0;
  EXPECT_TRUE(
      handler->CanDropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator, sequence0_index));
  EXPECT_TRUE(
      handler->DropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator, sequence0_index));
  ASSERT_EQ(sequence0->GetInstructions().size(), 3);
  EXPECT_EQ(sequence0->GetInstructions().at(0), wait2);
  EXPECT_EQ(sequence0->GetInstructions().at(1), wait0);
  EXPECT_EQ(sequence0->GetInstructions().at(2), wait1);
}

TEST_F(InstructionEditorDropHandlerTest, DropInstructionCopyMimeDataBetweenItems)
{
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

  auto container = m_model.GetRootItem();
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto wait0 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence0);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence0);

  auto message_instruction = CreateInstructionItem(domainconstants::kMessageInstructionType);
  auto mime_data = sup::gui::CreateItemSelectionCopyMimeData({message_instruction.get()},
                                                             kCopyInstructionMimeType);

  // copying it into position [3]
  auto handler = CreateDefaultDropHandler();

  const std::int32_t drop_indicator = 0;
  auto sequence0_index = m_view_model.index(0, 0);
  EXPECT_TRUE(
      handler->CanDropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, sequence0_index));
  EXPECT_TRUE(
      handler->DropMimeData(mime_data.get(), Qt::CopyAction, drop_indicator, sequence0_index));
  ASSERT_EQ(sequence0->GetInstructions().size(), 4);
  EXPECT_EQ(sequence0->GetInstructions().at(0)->GetType(), message_instruction->GetType());
  EXPECT_EQ(sequence0->GetInstructions().at(1), wait0);
  EXPECT_EQ(sequence0->GetInstructions().at(2), wait1);
  EXPECT_EQ(sequence0->GetInstructions().at(3), wait2);
}

}  // namespace oac_tree_gui::test
