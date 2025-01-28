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

#include "oac-tree-gui/composer/instruction_editor_action_handler.h"

#include <oac-tree-gui/model/instruction_container_item.h>
#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/sequencer_model.h>
#include <oac-tree-gui/model/standard_instruction_items.h>
#include <oac-tree-gui/nodeeditor/scene_utils.h>
#include <oac-tree-gui/viewmodel/drag_and_drop_helper.h>
#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace sequencergui::test
{

//! Tests for InstructionEditorActionHandler class.

class InstructionEditorActionHandlerCopyPasteTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerCopyPasteTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  /**
   * @brief Test helper to create context mimicking current InstructionEditor widget state.
   *
   * @param instruction Currently selected instruction
   * @param current_mime The content of the clipboard.
   *
   * @return Context object.
   */
  InstructionEditorContext CreateContext(InstructionItem* instruction,
                                         const QMimeData* current_mime)
  {
    InstructionEditorContext result;
    result.selected_procedure = [this]() { return m_procedure; };
    result.selected_instruction = [instruction]() { return instruction; };
    result.send_message_callback = m_warning_listener.AsStdFunction();
    result.get_mime_data = [current_mime]() { return current_mime; };
    result.set_mime_data = [this](std::unique_ptr<QMimeData> data)
    { m_copy_result = std::move(data); };
    return result;
  }

  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(InstructionItem* instruction,
                                                                      const QMimeData* current_mime)
  {
    return std::make_unique<InstructionEditorActionHandler>(
        CreateContext(instruction, current_mime));
  }
  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_warning_listener;
  std::unique_ptr<QMimeData> m_copy_result;
};

//! Available operations when nothing is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPasteAfter());
  EXPECT_FALSE(handler->CanPasteInto());
  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyOperation)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  sequence->SetName("abc");

  EXPECT_EQ(m_copy_result.get(), nullptr);

  // instruction is selected, no mime
  auto handler = CreateActionHandler(sequence, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kCopyInstructionMimeType));
}

//! Testing CanPasteAfter and CanPasteInto methods at different selections.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateActionHandler(nullptr, &mime_data);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);
    auto handler = CreateActionHandler(nullptr, mime_data.get());

    // paste-after when nothing is selected is allowed
    EXPECT_TRUE(handler->CanPasteAfter());

    // paste-into when nothing is selected is forbidden
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // selected item in the container, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
    auto handler = CreateActionHandler(sequence, mime_data.get());
    EXPECT_TRUE(handler->CanPasteAfter());
    EXPECT_TRUE(handler->CanPasteInto());
  }

  {  // repeat instruction with Sequence
    const WaitItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
    auto sequence = m_model.InsertItem<SequenceItem>(repeat);

    auto handler = CreateActionHandler(sequence, mime_data.get());

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

    auto handler = CreateActionHandler(sequence1, mime_data.get());

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

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler(nullptr, mime_data.get());

  QSignalSpy spy_selection_request(handler.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  EXPECT_CALL(m_warning_listener, Call(::testing::_)).Times(0);

  EXPECT_TRUE(handler->CanPasteAfter());
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(instructions.at(0)->GetDisplayName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selection_request), instructions.at(0));
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

  // creating action handler mimicking `sequence` instruction selected, and mime data in a buffer
  auto handler = CreateActionHandler(sequence, mime_data.get());

  EXPECT_CALL(m_warning_listener, Call(::testing::_)).Times(0);

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

  // wait0 is selected
  auto handler = CreateActionHandler(wait0, mime_data.get());

  EXPECT_CALL(m_warning_listener, Call(::testing::_)).Times(0);

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
}

//! Insertion instruction in the selected instruction.

TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteInto)
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

  // creating action handler mimicking `sequence` instruction selected
  auto handler = CreateActionHandler(sequence, mime_data.get());

  EXPECT_CALL(m_warning_listener, Call(::testing::_)).Times(0);

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
}

//! Cut selected instruction.

TEST_F(InstructionEditorActionHandlerCopyPasteTest, CutOperation)
{
  // inserting instruction in the container
  auto wait0 = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());
  auto wait1 = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());

  // wait0 is selected
  auto handler = CreateActionHandler(wait0, nullptr);

  QSignalSpy spy_selection_request(handler.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  EXPECT_CALL(m_warning_listener, Call(::testing::_)).Times(0);

  // inserting instruction into selected instruction
  EXPECT_TRUE(handler->CanCut());

  handler->Cut();

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  ASSERT_EQ(instructions.size(), 1);

  // checking the request to select remaining item
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selection_request), wait1);
}

}  // namespace sequencergui::test
