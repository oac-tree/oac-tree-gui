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
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>
#include <testutils/mock_dialog.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>

using namespace sequencergui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for InstructionEditorActionHandler class.

class InstructionEditorActionHandlerTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  /**
   * @brief Test helper to create context mimicking current InstructionEditor widget state.
   *
   * It contains a callbacks to mimick currently selected procedure and instruction, callback to
   * edit AnyValue and copy/paste activity.
   *
   * @param procedure Currently selected procedure.
   * @param instruction Currently selected instruction
   * @param dialog_result The data to return to the caller mimicking dialog answer.
   * @param current_mime The content of the clipboard.
   *
   * @return Context object.
   */
  InstructionEditorContext CreateContext(ProcedureItem* procedure, InstructionItem* instruction,
                                         AnyValueDialogResult dialog_result = {},
                                         const QMimeData* current_mime = nullptr)
  {
    m_mock_dialog.SetItemToReturn(std::move(dialog_result));

    InstructionEditorContext result;
    result.selected_procedure = [procedure]() { return procedure; };
    result.selected_instruction = [instruction]() { return instruction; };
    result.send_message_callback = m_warning_listener.CreateCallback();
    result.edit_anyvalue_callback = m_mock_dialog.CreateCallback();
    return result;
  }

  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(
      ProcedureItem* procedure, InstructionItem* instruction,
      AnyValueDialogResult dialog_result = {})
  {
    return std::make_unique<InstructionEditorActionHandler>(
        CreateContext(procedure, instruction, std::move(dialog_result)));
  }

  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  testutils::MockDialog m_mock_dialog;
};

//! Attempt to insert an instruction when no procedure created upfront.

TEST_F(InstructionEditorActionHandlerTest, AttemptToInsertInstructionWhenNoProcedureSelected)
{
  // creating the context pretending that no procedures/instructions are selected
  auto actions = CreateActionHandler(nullptr, nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // it is not possible to add instruction when no procedure is selected, expecting callback
  EXPECT_NO_THROW(actions->OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type)));
}

//! Adding wait instruction.

TEST_F(InstructionEditorActionHandlerTest, AddWait)
{
  auto actions = CreateActionHandler(m_procedure, nullptr);

  QSignalSpy spy_selection_request(actions.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  actions->OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);

  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem*>(spy_selection_request), instructions.at(0));
}

//! Adding choice instruction. Checking that universal instruction is correctly handled.

TEST_F(InstructionEditorActionHandlerTest, AddChoice)
{
  auto actions = CreateActionHandler(m_procedure, nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  actions->OnInsertInstructionAfterRequest(
      QString::fromStdString(domainconstants::kChoiceInstructionType));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), UniversalInstructionItem::Type);
}

//! Insertion instruction after selected instruction.

TEST_F(InstructionEditorActionHandlerTest, InsertInstructionAfter)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating the context mimicking `sequence` instruction selected
  auto actions = CreateActionHandler(m_procedure, sequence);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  actions->OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), WaitItem::Type);

  // Check coordinates of Wait instruction. It should be placed nearby to the original
  // instruction.
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(1)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(1)->GetY(), offset + sequence_y);
}

//! Consequent insert of one instruction after the other, nothing is selected.

TEST_F(InstructionEditorActionHandlerTest, InsertInstructionAfterWhenInAppendMode)
{
  // creating the context mimicking "no instruction selected"
  auto actions = CreateActionHandler(m_procedure, nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // appending instruction to the container
  actions->OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  // appending instruction to the container
  actions->OnInsertInstructionAfterRequest(QString::fromStdString(SequenceItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), SequenceItem::Type);
}

//! Insertion instruction after selected instruction, when no more insertions is allowed.

TEST_F(InstructionEditorActionHandlerTest, AttemptToInsertInstructionAfter)
{
  // inserting instruction in the container
  auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
  auto sequence = m_model.InsertItem<SequenceItem>(repeat);

  // creating the context mimicking `sequence` instruction selected
  auto actions = CreateActionHandler(m_procedure, sequence);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // It is not possible to add second instruction to repeat instruction, expecting warning callback
  EXPECT_NO_THROW(actions->OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type)));

  ASSERT_EQ(repeat->GetInstructions().size(), 1);
}

//! Insertion instruction in the selected instruction.

TEST_F(InstructionEditorActionHandlerTest, InsertInstructionInto)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating the context mimicking `sequence` instruction selected
  auto actions = CreateActionHandler(m_procedure, sequence);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // inserting instruction into selected instruction
  actions->OnInsertInstructionIntoRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(sequence->GetInstructions().size(), 1);

  // inserting second instruction
  actions->OnInsertInstructionIntoRequest(
      QString::fromStdString(domainconstants::kMessageInstructionType));
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = sequence->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), UniversalInstructionItem::Type);
  auto universal = dynamic_cast<UniversalInstructionItem*>(instructions.at(1));
  ASSERT_NE(universal, nullptr);
  EXPECT_EQ(universal->GetDomainType(), domainconstants::kMessageInstructionType);

  // Check coordinates of the instruction. It should be placed nearby to the original instruction.
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(1)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(1)->GetY(), offset + sequence_y);
}

//! Attempt to insert instruction into the one, that can't have children.

TEST_F(InstructionEditorActionHandlerTest, AttemptToInsertInstructionInto)
{
  // inserting instruction in the container
  auto wait = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking `wait` instruction selected
  auto actions = CreateActionHandler(m_procedure, wait);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to insert instruction into selected instruction, expecting callback
  EXPECT_NO_THROW(actions->OnInsertInstructionIntoRequest(QString::fromStdString(WaitItem::Type)));
  ASSERT_EQ(wait->GetInstructions().size(), 0);
}

//! Removing selected instruction.

TEST_F(InstructionEditorActionHandlerTest, RemoveInstruction)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking no instruction selected
  auto actions = CreateActionHandler(m_procedure, nullptr);

  // nothing selected, remove request does nothing
  actions->OnRemoveInstructionRequest();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 1);

  // creating the context mimicking sequencer selected
  actions = CreateActionHandler(m_procedure, sequence);

  // remove request should remove item
  actions->OnRemoveInstructionRequest();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 0);
}

//! Move selected instruction up.

TEST_F(InstructionEditorActionHandlerTest, MoveUp)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  // creating the context mimicking wait2 is selected
  auto actions = CreateActionHandler(m_procedure, wait2);

  QSignalSpy spy_selection_request(actions.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  // moving selected item up
  actions->OnMoveUpRequest();

  // checking that the order of instructions has changed
  const std::vector<InstructionItem*> expected({wait0, wait2, wait1});
  EXPECT_EQ(sequence->GetInstructions(), expected);

  // checking the request to select just moved item
  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem*>(spy_selection_request), wait2);
}

//! Move selected instruction up.

TEST_F(InstructionEditorActionHandlerTest, MoveDown)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  // creating the context mimicking wait0 is selected
  auto actions = CreateActionHandler(m_procedure, wait0);

  QSignalSpy spy_selection_request(actions.get(),
                                   &InstructionEditorActionHandler::SelectItemRequest);

  // moving selected item up
  actions->OnMoveDownRequest();

  // checking that the order of instructions has changed
  const std::vector<InstructionItem*> expected({wait1, wait0, wait2});
  EXPECT_EQ(sequence->GetInstructions(), expected);

  // checking the request to select just moved item
  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem*>(spy_selection_request), wait0);
}

//! Attempt to edit AnyValueItem when nothing appropriate is selected.

TEST_F(InstructionEditorActionHandlerTest, OnEditRequestWhenNothingIsSelected)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // pretending that sequence is selected
  auto actions = CreateActionHandler(m_procedure, sequence);

  // expecting warning callbacks complaining that sequence can't have AnyValueItem
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  actions->OnEditAnyvalueRequest();
}

//! Editing AnyValueItem when EPICS instruction is selected.

TEST_F(InstructionEditorActionHandlerTest, OnEditRequestWhenInstructionIsSelected)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }
  const bool dialog_was_acccepted = true;

  auto item =
      m_model.InsertItem<PvAccessWriteInstructionItem>(m_procedure->GetInstructionContainer());

  auto previous_anyvalue = GetAnyValueItem(*item);
  ASSERT_NE(previous_anyvalue, nullptr);
  EXPECT_EQ(previous_anyvalue->GetType(), sup::gui::AnyValueEmptyItem::Type);

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // pretending that instruction is selected
  auto actions =
      CreateActionHandler(m_procedure, item, {dialog_was_acccepted, std::move(editing_result)});

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  actions->OnEditAnyvalueRequest();

  // checking that instruction got new AnyValueItem
  EXPECT_EQ(GetAnyValueItem(*item), editing_result_ptr);
}

//! Simulating the case when user removes AnyValueItem in the editor.
//! It shouldn't be allowed.

TEST_F(InstructionEditorActionHandlerTest, AttemptToRemoveItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }
  const bool dialog_was_acccepted = true;

  auto item =
      m_model.InsertItem<PvAccessWriteInstructionItem>(m_procedure->GetInstructionContainer());

  auto previous_anyvalue = GetAnyValueItem(*item);
  ASSERT_NE(previous_anyvalue, nullptr);
  EXPECT_EQ(previous_anyvalue->GetType(), sup::gui::AnyValueEmptyItem::Type);

  // item intentionally uninitialised to mimick item removal in the editor
  std::unique_ptr<sup::gui::AnyValueStructItem> editing_result;

  // pretending that instruction is selected
  auto actions =
      CreateActionHandler(m_procedure, item, {dialog_was_acccepted, std::move(editing_result)});

  // expecting error callback.
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  actions->OnEditAnyvalueRequest();

  // checking that instruction still has old AnyValye
  EXPECT_EQ(GetAnyValueItem(*item), previous_anyvalue);
}
