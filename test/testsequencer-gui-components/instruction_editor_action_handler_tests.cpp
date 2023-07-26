/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/universal_instruction_item.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>

#include <QSignalSpy>

using namespace sequencergui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for SequencerObserver class.

class InstructionEditorActionHandlerTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  //! Creates InstructionEditorContext for testing purposes. It contains callbacks to mimick
  //! user choice regarding the selected procedure and instruction.
  InstructionEditorContext CreateContext(ProcedureItem* procedure, InstructionItem* instruction)
  {
    InstructionEditorContext result;
    result.selected_procedure = [procedure]() { return procedure; };
    result.selected_instruction = [instruction]() { return instruction; };
    result.send_message_callback = m_warning_listener.CreateCallback();
    return result;
  }

  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(ProcedureItem* procedure,
                                                                      InstructionItem* instruction)
  {
    return std::make_unique<InstructionEditorActionHandler>(CreateContext(procedure, instruction));
  }

  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
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

  EXPECT_EQ(spy_selection_request.count(), 1);
  auto arguments = spy_selection_request.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, instructions.at(0));
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

  // Check coordinates of Wait instruction. It should be placed nearby to original
  // instruction

  // hardcoded in composer_actions.cpp in UpdateChildCoordinate() function
  const double coordinate_offset = 10;

  EXPECT_EQ(instructions.at(1)->GetX(), coordinate_offset + sequence_x);
  EXPECT_EQ(instructions.at(1)->GetY(), coordinate_offset + sequence_y);
}

//! Insertion instruction after selected instruction.
//! Mimicking the case when no instruction is actually selected.
//! Items should be added one after another.

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

  // Check coordinates of Wait instruction. It should be placed nearby to original
  // instruction

  // hardcoded in composer_actions.cpp in UpdateChildCoordinate() function
  const double coordinate_offset = 10;

  EXPECT_EQ(instructions.at(1)->GetX(), coordinate_offset + sequence_x);
  EXPECT_EQ(instructions.at(1)->GetY(), coordinate_offset + sequence_y);
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
