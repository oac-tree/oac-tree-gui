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

#include "sequencergui/composer/instruction_editor_actions.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sup/gui/core/exceptions.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_message_handler.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for SequencerObserver class.

class InstructionEditorActionsTest : public ::testing::Test
{
public:
  InstructionEditorActionsTest() : m_actions(&m_model)
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  //! Creates ComposerContext for testing purposes. It contains callbacks to mimick
  //! user choice regarding the selected procedure, instruction, and variable.
  InstructionEditorContext CreateContext(ProcedureItem* procedure, InstructionItem* instruction)
  {
    InstructionEditorContext result;
    result.selected_procedure = [procedure]() { return procedure; };
    result.selected_instruction = [instruction]() { return instruction; };
    return result;
  }

  SequencerModel m_model;
  InstructionEditorActions m_actions;
  ProcedureItem* m_procedure{nullptr};
};

//! Insertion instruction after selected instruction, when no more insertions is allowed.

TEST_F(InstructionEditorActionsTest, AttemptToInsertInstructionWhenNoProcedureSelected)
{
  // creating the context pretending that no procedures/instructions are selected
  auto context = CreateContext(nullptr, nullptr);
  m_actions.SetContext(context);

  // It is not possible to add second instruction to repeat instruction
  EXPECT_THROW(m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type)),
               sup::gui::RuntimeException);
}

//! Insertion instruction after selected instruction.

TEST_F(InstructionEditorActionsTest, InsertInstructionAfter)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating the context mimicking `sequence` instruction selected
  auto context = CreateContext(m_procedure, sequence);
  m_actions.SetContext(context);

  // appending instruction to the container
  m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type));
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

TEST_F(InstructionEditorActionsTest, InsertInstructionAfterWhenInAppendMode)
{
  // creating the context mimicking "no instruction selected"
  auto context = CreateContext(m_procedure, nullptr);
  m_actions.SetContext(context);

  // appending instruction to the container
  m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  // appending instruction to the container
  m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(SequenceItem::Type));
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), SequenceItem::Type);
}

//! Insertion instruction after selected instruction, when no more insertions is allowed.

TEST_F(InstructionEditorActionsTest, AttemptToInsertInstructionAfter)
{
  // inserting instruction in the container
  auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
  auto sequence = m_model.InsertItem<SequenceItem>(repeat);

  // creating the context mimicking `sequence` instruction selected
  auto context = CreateContext(m_procedure, sequence);
  m_actions.SetContext(context);

  // It is not possible to add second instruction to repeat instruction
  EXPECT_THROW(m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type)),
               sup::gui::RuntimeException);

  ASSERT_EQ(repeat->GetInstructions().size(), 1);

  // setting message handler
  testutils::MockMessageHandler mock_handler;
  m_actions.SetMessageHandler(CreateMessageHandlerDecorator(&mock_handler));

  // after handler set, we expect no throws; handler method should be called
  EXPECT_CALL(mock_handler, SendMessage(_)).Times(1);
  EXPECT_NO_THROW(
      m_actions.OnInsertInstructionAfterRequest(QString::fromStdString(WaitItem::Type)));
}

//! Insertion instruction in the selected instruction.

TEST_F(InstructionEditorActionsTest, InsertInstructionInto)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  const double sequence_x = 10;
  const double sequence_y = 20;
  sequence->SetX(sequence_x);
  sequence->SetY(sequence_y);

  // creating the context mimicking `sequence` instruction selected
  auto context = CreateContext(m_procedure, sequence);
  m_actions.SetContext(context);

  // inserting instruction into selected instruction
  m_actions.OnInsertInstructionIntoRequest(QString::fromStdString(WaitItem::Type));
  ASSERT_EQ(sequence->GetInstructions().size(), 1);

  // inserting second instruction
  m_actions.OnInsertInstructionIntoRequest(QString::fromStdString(MessageItem::Type));
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = sequence->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), MessageItem::Type);

  // Check coordinates of Wait instruction. It should be placed nearby to original
  // instruction

  // hardcoded in composer_actions.cpp in UpdateChildCoordinate() function
  const double coordinate_offset = 10;

  EXPECT_EQ(instructions.at(1)->GetX(), coordinate_offset + sequence_x);
  EXPECT_EQ(instructions.at(1)->GetY(), coordinate_offset + sequence_y);
}

//! Attempt to insert instruction into the one, that can't have children.

TEST_F(InstructionEditorActionsTest, AttemptToInsertInstructionInto)
{
  // inserting instruction in the container
  auto wait = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking `wait` instruction selected
  auto context = CreateContext(m_procedure, wait);
  m_actions.SetContext(context);

  // inserting instruction into selected instruction
  EXPECT_THROW(m_actions.OnInsertInstructionIntoRequest(QString::fromStdString(WaitItem::Type)),
               sup::gui::RuntimeException);
  ASSERT_EQ(wait->GetInstructions().size(), 0);

  // setting message handler
  testutils::MockMessageHandler mock_handler;
  m_actions.SetMessageHandler(CreateMessageHandlerDecorator(&mock_handler));

  // after handler set, we expect no throws; handler method should be called
  EXPECT_CALL(mock_handler, SendMessage(_)).Times(1);
  EXPECT_NO_THROW(m_actions.OnInsertInstructionIntoRequest(QString::fromStdString(WaitItem::Type)));
}

//! Removing selected variable.

TEST_F(InstructionEditorActionsTest, RemoveInstruction)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking no instruction selected
  m_actions.SetContext(CreateContext(m_procedure, nullptr));

  // nothing selected, remove request does nothing
  m_actions.OnRemoveInstructionRequest();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 1);

  // creating the context mimicking sequencer selected
  m_actions.SetContext(CreateContext(m_procedure, sequence));

  // remove request should remove item
  m_actions.OnRemoveInstructionRequest();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 0);
}
