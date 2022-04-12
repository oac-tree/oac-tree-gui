/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/composeractions.h"

#include "mockmessagehandler.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/core/exceptions.h"

#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for SequencerObserver class.

class ComposerActionsTest : public ::testing::Test
{
public:
  //! Test class to wrap MockHandlerDecorator and pass it inside ComposerActions.
  //! This is to avoid using testing::Mock::AllowLeak.
  class TestHandlerDecorator : public sequencergui::MessageHandlerInterface
  {
  public:
    explicit TestHandlerDecorator(sequencergui::MessageHandlerInterface* context)
        : m_context(context)
    {
    }

    void SendMessage(const std::string& text) override { m_context->SendMessage(text); }
    sequencergui::MessageHandlerInterface* m_context{nullptr};
  };

  ComposerActionsTest() : m_actions(&m_model)
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  //! Creates ComposerContext for testing purposes. It contains callbacks to mimick
  //! user choice regarding the selected procedure, instruction, and variable.
  ComposerContext CreateContext(InstructionItem* instruction, VariableItem* variable)
  {
    ComposerContext result;
    result.m_selected_procedure = [this]() { return m_procedure; };
    result.m_selected_instruction = [instruction]() { return instruction; };
    result.m_selected_variable = [variable]() { return variable; };
    return result;
  }

  //! Create message handler to pass it inside ComposerActions.
  static std::unique_ptr<MessageHandlerInterface> CreateMessageHandler(
      MessageHandlerInterface* mock_handler)
  {
    return std::make_unique<TestHandlerDecorator>(mock_handler);
  }

  SequencerModel m_model;
  ComposerActions m_actions;
  ProcedureItem* m_procedure{nullptr};
};

//! Insertion instruction after selected instruction.

TEST_F(ComposerActionsTest, InsertInstructionAfter)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking `sequence` instruction selected
  auto context = CreateContext(sequence, nullptr);
  m_actions.SetContext(context);

  // appending instruction to the container
  m_actions.InsertInstructionAfterRequest(WaitItem::Type);
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), WaitItem::Type);
}

//! Insertion instruction after selected instruction.
//! Mimicking the case when no instruction is actually selected.
//! Items should be added one after another.

TEST_F(ComposerActionsTest, InsertInstructionAfterWhenInAppendMode)
{
  // creating the context mimicking "no instruction selected"
  auto context = CreateContext(nullptr, nullptr);
  m_actions.SetContext(context);

  // appending instruction to the container
  m_actions.InsertInstructionAfterRequest(WaitItem::Type);
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  // appending instruction to the container
  m_actions.InsertInstructionAfterRequest(SequenceItem::Type);
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), SequenceItem::Type);
}

//! Insertion instruction in the selected instruction.

TEST_F(ComposerActionsTest, InsertInstructionInto)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking `sequence` instruction selected
  auto context = CreateContext(sequence, nullptr);
  m_actions.SetContext(context);

  // inserting instruction into selected instruction
  m_actions.InsertInstructionIntoRequest(WaitItem::Type);
  ASSERT_EQ(sequence->GetInstructions().size(), 1);

  // inserting second instruction
  m_actions.InsertInstructionIntoRequest(MessageItem::Type);
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = sequence->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::Type);
  EXPECT_EQ(instructions.at(1)->GetType(), MessageItem::Type);
}

//! Attempt to insert instruction into the one, that can't have children.

TEST_F(ComposerActionsTest, AttemptToInsertInstructionInto)
{
  // inserting instruction in the container
  auto wait = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());

  // creating the context mimicking `wait` instruction selected
  auto context = CreateContext(wait, nullptr);
  m_actions.SetContext(context);

  // inserting instruction into selected instruction
  EXPECT_THROW(m_actions.InsertInstructionIntoRequest(WaitItem::Type),
               mvvm::InvalidInsertException);
  ASSERT_EQ(wait->GetInstructions().size(), 0);

  // setting message handler

  MockMessageHandler mock_handler;
  m_actions.SetMessageHandler(CreateMessageHandler(&mock_handler));

  // After handler set, we expect no throws, and trigger of MessageHandler method.
  EXPECT_CALL(mock_handler, SendMessage(_)).Times(1);
  EXPECT_NO_THROW(m_actions.InsertInstructionIntoRequest(WaitItem::Type));
}
