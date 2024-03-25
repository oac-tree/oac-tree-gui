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

#include "sequencergui/composer/instruction_editor_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/epics_instruction_items.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/universal_instruction_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>
#include <testutils/mock_dialog.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

using namespace sequencergui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

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
    result.send_message_callback = m_warning_listener.CreateCallback();
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
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  std::unique_ptr<QMimeData> m_copy_result;
};

//! Copy operation when nothing is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  //
  auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPasteAfter());  // because mime data is empty
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

//! Testing CanPasteAfter method at different selections.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CanPasteAfter)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);
    EXPECT_FALSE(handler->CanPasteAfter());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateActionHandler(nullptr, &mime_data);
    EXPECT_FALSE(handler->CanPasteAfter());
  }

  {  // nothing is selected, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);
    auto handler = CreateActionHandler(nullptr, mime_data.get());
    EXPECT_TRUE(handler->CanPasteAfter());
  }

  {  // selected item in the container, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
    auto handler = CreateActionHandler(sequence, mime_data.get());
    EXPECT_TRUE(handler->CanPasteAfter());
  }

  {  // Repeat instruction with Sequence, attempt to paste after a sequence
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
    auto sequence = m_model.InsertItem<SequenceItem>(repeat);

    auto handler = CreateActionHandler(sequence, mime_data.get());

    // it shouldn't be possible to paste after a sequence, since repeat can hold only one
    // instruction
    EXPECT_FALSE(handler->CanPasteAfter());
  }

  {  // Sequence instruction inside Sequence, attempt to paste after internal sequence
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto sequence0 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
    auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);

    auto handler = CreateActionHandler(sequence1, mime_data.get());

    // It is possible to paste inside a sequence0, right after sequence1
    EXPECT_TRUE(handler->CanPasteAfter());
  }
}

//! Testing PasteAfter for the following scenario: empty model, nothing is selected, pasted item
//! should just appear as a first item.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, PasteAfterIntoEmptyContainer)
{
  // creating mime data representing clipboard content
  WaitItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler(nullptr, mime_data.get());

  QSignalSpy spy_selection_request(handler.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  EXPECT_TRUE(handler->CanPasteAfter());
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(0)->GetDisplayName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem*>(spy_selection_request), instructions.at(0));
}

//! Testing PasteAfter for the following scenario: sequence a model, selected, pasting new
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
  auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  // creating action handler mimicking `sequence` instruction selected, and mime data in a buffer
  auto handler = CreateActionHandler(sequence, mime_data.get());

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  handler->PasteAfter();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), WaitItem::Type);

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
  auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

  // wait0 is selected
  auto handler = CreateActionHandler(wait0, mime_data.get());

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  handler->PasteAfter();
  ASSERT_EQ(sequence->GetInstructions().size(), 3);

  // Wait instruction should be after Sequence instruction
  auto inserted_wait = sequence->GetInstructions().at(1);
  EXPECT_EQ(inserted_wait->GetType(), WaitItem::Type);
  EXPECT_EQ(inserted_wait->GetDisplayName(), std::string("abc"));

  // Check coordinates of Wait instruction. It should be placed nearby to the selected one
  const double offset = GetInstructionDropOffset();
  EXPECT_DOUBLE_EQ(inserted_wait->GetX(), offset + wait0_x);
  EXPECT_DOUBLE_EQ(inserted_wait->GetY(), offset + wait0_y);
}
