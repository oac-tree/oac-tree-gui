/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/composer/instruction_editor_action_handler.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_instruction_editor_context.h>
#include <testutils/test_utils.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorActionHandler for copy/paste editing scenarios.
 */
class InstructionEditorActionHandlerCopyPasteTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerCopyPasteTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  /**
   * @brief Creates action handler.
   *
   * @param selection Instruction which will be reported as user selection.
   * @param current_mime The content of the clipboard.
   */
  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(
      const std::vector<InstructionItem*>& selection, std::unique_ptr<QMimeData> clipboard = {})
  {
    m_mock_context.SetClipboardContent(std::move(clipboard));
    return m_mock_context.CreateActionHandler(m_procedure->GetInstructionContainer(), selection);
  }
  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};

  test::MockInstructionEditorContext m_mock_context;
};

//! Available operations when nothing is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  auto handler = CreateActionHandler(/*selected instruction*/ {}, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
  EXPECT_FALSE(handler->CanPasteAfter());

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
  EXPECT_FALSE(handler->CanPasteInto());

  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyOperation)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  sequence->SetName("abc");

  EXPECT_EQ(m_mock_context.GetClipboardContent(), nullptr);

  // instruction is selected, no mime
  auto handler = CreateActionHandler({sequence}, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_mock_context.GetClipboardContent(), nullptr);
  EXPECT_TRUE(m_mock_context.GetClipboardContent()->hasFormat(kCopyInstructionMimeType));
}

//! Testing CanPasteAfter and CanPasteInto methods at different selections.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(/*selected instruction*/ {}, /*mime*/ {});
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, wrong mime data
    auto mime_data = std::make_unique<QMimeData>();
    auto handler = CreateActionHandler({}, std::move(mime_data));
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);
    auto handler = CreateActionHandler({}, std::move(mime_data));

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    // paste-after when nothing is selected is allowed
    EXPECT_TRUE(handler->CanPasteAfter());

    // paste-into when nothing is selected is forbidden
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // selected item in the container, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
    auto handler = CreateActionHandler({sequence}, std::move(mime_data));
    EXPECT_TRUE(handler->CanPasteAfter());
    EXPECT_TRUE(handler->CanPasteInto());
  }

  {  // repeat instruction with Sequence
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
    auto sequence = m_model.InsertItem<SequenceItem>(repeat);

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    auto handler = CreateActionHandler({sequence}, std::move(mime_data));

    // it shouldn't be possible to paste after a sequence, since repeat can hold only one
    // instruction
    EXPECT_FALSE(handler->CanPasteAfter());

    // it is possible to paste into a sequence itself
    EXPECT_TRUE(handler->CanPasteInto());
  }

  {  // sequence instruction inside Sequence
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto sequence0 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
    auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    auto handler = CreateActionHandler({sequence1}, std::move(mime_data));

    // It is possible to paste inside a sequence0, right after sequence1
    EXPECT_TRUE(handler->CanPasteAfter());

    // it is possible to paste into a sequence1 itself
    EXPECT_TRUE(handler->CanPasteInto());
  }
}

//! Testing PasteAfter for the following scenario: empty model, nothing is selected, pasted item
//! should just appear as a first item.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteAfterIntoEmptyContainer)
{
  // creating mime data representing clipboard content
  WaitItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler({nullptr}, std::move(mime_data));

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  EXPECT_TRUE(handler->CanPasteAfter());
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(instructions.at(0)->GetDisplayName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(reported_item, instructions.at(0));
}

//! Testing PasteAfter for the following scenario: sequence in a model, selected, pasting new
//! instruction right after it.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteAfterSelectedItem)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating mime data representing clipboard content
  WaitItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  // creating action handler mimicking `sequence` instruction selected, and mime data in a buffer
  auto handler = CreateActionHandler({sequence}, std::move(mime_data));

  // appending instruction to the container
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::GetStaticType());
  EXPECT_EQ(instructions.at(1)->GetType(), WaitItem::GetStaticType());

  // Check coordinates of Wait instruction. It should be placed nearby to the original
  // instruction
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(1)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(1)->GetY(), offset + sequence_y);

  // validating request to select just inserted item
  EXPECT_EQ(reported_item, instructions.at(1));
}

//! Testing PasteAfter for the following scenario: sequence has two items, the first one selected,
//! pasting after it.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteAfterWhenInsideSequenceAfterSelectedItem)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait0 = m_model.InsertItem<SequenceItem>(sequence);
  const double wait0_x = 10;
  const double wait0_y = 20;
  wait0->SetX(wait0_x);
  wait0->SetY(wait0_y);
  auto wait1 = m_model.InsertItem<SequenceItem>(sequence);

  // creating mime data representing clipboard content
  WaitItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  // wait0 is selected
  auto handler = CreateActionHandler({wait0}, std::move(mime_data));

  // appending instruction to the container
  handler->PasteAfter();
  ASSERT_EQ(sequence->GetInstructions().size(), 3);

  // Wait instruction should be after Sequence instruction
  auto inserted_wait = sequence->GetInstructions().at(1);
  EXPECT_EQ(inserted_wait->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(inserted_wait->GetDisplayName(), std::string("abc"));

  // Check coordinates of Wait instruction. It should be placed nearby to the selected one
  const double offset = GetInstructionDropOffset();
  EXPECT_DOUBLE_EQ(inserted_wait->GetX(), offset + wait0_x);
  EXPECT_DOUBLE_EQ(inserted_wait->GetY(), offset + wait0_y);
  EXPECT_EQ(reported_item, inserted_wait);
}

TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteIntoSelectedInstruction)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating mime data representing clipboard content
  WaitItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  // creating action handler mimicking `sequence` instruction selected
  auto handler = CreateActionHandler({sequence}, std::move(mime_data));

  // inserting instruction into selected instruction
  handler->PasteInto();
  auto instructions = sequence->GetInstructions();
  ASSERT_EQ(instructions.size(), 1);

  // Wait instruction should be after Sequence instruction
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(instructions.at(0)->GetDisplayName(), std::string("abc"));

  // Check coordinates of the instruction. It should be placed nearby to the original instruction.
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(0)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(0)->GetY(), offset + sequence_y);
  EXPECT_EQ(reported_item, instructions.at(0));
}

//! Cut selected instruction.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CutOperation)
{
  // inserting instruction in the container
  auto wait0 = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());
  auto wait1 = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());

  // wait0 is selected
  auto handler = CreateActionHandler({wait0}, nullptr);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  EXPECT_TRUE(handler->CanCut());
  handler->Cut();

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  ASSERT_EQ(instructions.size(), 1);

  // checking the request to select remaining item
  EXPECT_EQ(reported_item, wait1);
}

//! Testing PasteAfter for the following scenario: sequence in a model, selected, pasting new
//! instruction right after it.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyAndPaste)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  auto handler = CreateActionHandler({sequence});

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  handler->Copy();

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(1);

  // appending instruction to the container
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::GetStaticType());
  EXPECT_EQ(instructions.at(1)->GetType(), SequenceItem::GetStaticType());

  // Check coordinates of Wait instruction. It should be placed nearby to the original
  // instruction
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(1)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(1)->GetY(), offset + sequence_y);

  // validating request to select just inserted item
  EXPECT_EQ(m_mock_context.GetSelectRequests(),
            std::vector<mvvm::SessionItem*>({instructions.at(1)}));
}

TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyAndPastePartOfSequenceTreeIntoRepeat)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);
  auto message = InsertInstruction(domainconstants::kMessageInstructionType, sequence);
  auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());

  auto handler = CreateActionHandler({sequence, message});

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  handler->Copy();

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(1);

  // appending instruction to the container
  m_mock_context.SetAsCurrentSelection({repeat});

  handler->PasteInto();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);
  ASSERT_EQ(repeat->GetInstructions().size(), 1);
  auto copied_sequence = repeat->GetInstructions().at(0);
  ASSERT_EQ(copied_sequence->GetInstructions().size(), 1);
  auto copied_message = copied_sequence->GetInstructions().at(0);
  EXPECT_EQ(copied_message->GetDomainType(), domainconstants::kMessageInstructionType);
}

TEST_F(InstructionEditorActionHandlerCopyPasteTest, CutAndPastePartOfSequenceTreeIntoRepeat)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);
  auto message = InsertInstruction(domainconstants::kMessageInstructionType, sequence);
  auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());

  auto handler = CreateActionHandler({sequence, message});

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(1);

  handler->Cut();
   // mimicking dissapearance of selection after item removal
  m_mock_context.SetAsCurrentSelection({});

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(1);

  // appending instruction to the container
  m_mock_context.SetAsCurrentSelection({repeat});

  handler->PasteInto();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(repeat->GetInstructions().size(), 1);
  auto copied_sequence = repeat->GetInstructions().at(0);
  ASSERT_EQ(copied_sequence->GetInstructions().size(), 1);
  auto copied_message = copied_sequence->GetInstructions().at(0);
  EXPECT_EQ(copied_message->GetDomainType(), domainconstants::kMessageInstructionType);
}

}  // namespace oac_tree_gui::test
