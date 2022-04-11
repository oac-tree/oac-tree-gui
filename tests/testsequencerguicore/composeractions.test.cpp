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

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class ComposerActionsTest : public ::testing::Test
{
public:
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

  SequencerModel m_model;
  ComposerActions m_actions;
  ProcedureItem* m_procedure{nullptr};
};

//! Insertion instruction after selected instruction.
//! Mimicking the case when no instruction is actually selected.
//! Items should be added one after another.

TEST_F(ComposerActionsTest, InsertInstructionAfterWhenInAppend)
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
