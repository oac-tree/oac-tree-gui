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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_instruction_editor_context.h>

#include <QMimeData>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorActionHandler for basic editing scenarios.
 */
class InstructionEditorActionHandlerTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  /**
   * @brief Creates action handler.
   *
   * @param selection Instruction which will be reported as user selection.
   */
  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(
      const std::vector<InstructionItem*>& selection)
  {
    return m_mock_context.CreateActionHandler(m_procedure->GetInstructionContainer(), selection);
  }

  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
  test::MockInstructionEditorContext m_mock_context;
};

//! Checking exceptions depending on how many callbacks left undefined.
TEST_F(InstructionEditorActionHandlerTest, AttemptToCreateWhenNoContextIsInitialised)
{
  {
    const InstructionEditorContext context{};
    EXPECT_THROW(InstructionEditorActionHandler{context}, RuntimeException);
  }

  {
    InstructionEditorContext context;
    context.instruction_container = []() -> InstructionContainerItem* { return nullptr; };
    EXPECT_THROW(InstructionEditorActionHandler{context}, RuntimeException);
  }

  {
    InstructionEditorContext context;
    context.instruction_container = []() -> InstructionContainerItem* { return nullptr; };
    context.selected_instructions = []() -> std::vector<InstructionItem*> { return {}; };
    EXPECT_THROW(InstructionEditorActionHandler{context}, RuntimeException);
  }

  {
    InstructionEditorContext context;
    context.instruction_container = []() -> InstructionContainerItem* { return nullptr; };
    context.selected_instructions = []() -> std::vector<InstructionItem*> { return {}; };
    context.select_notify = [](auto item) {};
    EXPECT_NO_THROW(InstructionEditorActionHandler{context});
  }
}

TEST_F(InstructionEditorActionHandlerTest, GetSelectedInstructions)
{
  // inserting instruction in the container
  auto sequence0 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto sequence1 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  {
    auto handler = CreateActionHandler({});
    EXPECT_EQ(handler->GetSelectedInstruction(), nullptr);
    EXPECT_TRUE(handler->GetSelectedInstructions().empty());
  }

  {
    auto handler = CreateActionHandler({sequence0, sequence1});
    EXPECT_EQ(handler->GetSelectedInstruction(), sequence0);
    EXPECT_EQ(handler->GetSelectedInstructions(),
              std::vector<InstructionItem*>({sequence0, sequence1}));
  }
}

//! Attempt to insert an instruction when no procedure created upfront.
TEST_F(InstructionEditorActionHandlerTest, AttemptToInsertInstructionWhenNoProcedureSelected)
{
  // creating the context pretending that no procedures/instructions are selected
  auto handler = m_mock_context.CreateActionHandler(nullptr, {});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // it is not possible to add instruction when no procedure is selected, expecting callback
  EXPECT_NO_THROW(handler->InsertInstructionAfter(WaitItem::GetStaticType()));
}

//! Adding wait instruction.
TEST_F(InstructionEditorActionHandlerTest, AddWait)
{
  auto handler = CreateActionHandler({});

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // appending instruction to the container
  handler->InsertInstructionAfter(WaitItem::GetStaticType());
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());

  EXPECT_EQ(reported_item, instructions.at(0));
}

TEST_F(InstructionEditorActionHandlerTest, AddToWrongPlaceWhenNoMessageCallbackDefined)
{
  InstructionEditorContext context;
  context.instruction_container = []() -> InstructionContainerItem* { return nullptr; };
  context.selected_instructions = []() -> std::vector<InstructionItem*> { return {}; };
  context.select_notify = [](auto item) {};

  InstructionEditorActionHandler handler{context};

  EXPECT_THROW(
      handler.InsertItem(SequenceItem::GetStaticType(), m_procedure->GetInstructionContainer(),
                         mvvm::TagIndex{"no-such-tag", 0}),
      RuntimeException);
}

TEST_F(InstructionEditorActionHandlerTest, DropInstruction)
{
  const double expected_x{42.0};
  const double expected_y{43.0};

  auto handler = CreateActionHandler({});

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // appending instruction to the container
  handler->DropInstruction(WaitItem::GetStaticType(), {expected_x, expected_y});
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  ASSERT_EQ(instructions.size(), 1);
  auto inserted_instruction = instructions.at(0);
  EXPECT_EQ(inserted_instruction->GetType(), WaitItem::GetStaticType());

  EXPECT_EQ(reported_item, inserted_instruction);
  EXPECT_EQ(inserted_instruction->GetX(), expected_x);
  EXPECT_EQ(inserted_instruction->GetY(), expected_y);
}

//! Adding choice instruction. Checking that universal instruction is correctly handled.
TEST_F(InstructionEditorActionHandlerTest, AddChoice)
{
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // appending instruction to the container
  handler->InsertInstructionAfter(domainconstants::kChoiceInstructionType);
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), UniversalInstructionItem::GetStaticType());
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

  // creating action handler mimicking `sequence` instruction selected
  auto handler = CreateActionHandler({sequence});

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // appending instruction to the container
  handler->InsertInstructionAfter(WaitItem::GetStaticType());
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), SequenceItem::GetStaticType());
  EXPECT_EQ(instructions.at(1)->GetType(), WaitItem::GetStaticType());

  // Check coordinates of Wait instruction. It should be placed nearby to the original
  // instruction.
  const double offset = GetInstructionDropOffset();

  EXPECT_DOUBLE_EQ(instructions.at(1)->GetX(), offset + sequence_x);
  EXPECT_DOUBLE_EQ(instructions.at(1)->GetY(), offset + sequence_y);
}

TEST_F(InstructionEditorActionHandlerTest, InsertInstructionAfterWhenInAppendMode)
{
  // creating action handler mimicking "no instruction selected"
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(2);

  // appending instruction to the container
  handler->InsertInstructionAfter(WaitItem::GetStaticType());
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 1);

  // appending instruction to the container
  handler->InsertInstructionAfter(SequenceItem::GetStaticType());
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetTotalItemCount(), 2);

  auto instructions = m_procedure->GetInstructionContainer()->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(instructions.at(1)->GetType(), SequenceItem::GetStaticType());
}

TEST_F(InstructionEditorActionHandlerTest, AttemptToInsertInstructionAfter)
{
  // inserting instruction in the container
  auto repeat = m_model.InsertItem<RepeatItem>(m_procedure->GetInstructionContainer());
  auto sequence = m_model.InsertItem<SequenceItem>(repeat);

  // creating action handler mimicking `sequence` instruction selected
  auto handler = CreateActionHandler({sequence});

  EXPECT_TRUE(handler->CanInsertInstructionInto(domainconstants::kMessageInstructionType));
  EXPECT_FALSE(handler->CanInsertInstructionAfter(domainconstants::kMessageInstructionType));

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // It is not possible to add second instruction to repeat instruction, expecting warning
  EXPECT_NO_THROW(handler->InsertInstructionAfter(WaitItem::GetStaticType()));

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

  // creating action handler mimicking `sequence` instruction selected
  auto handler = CreateActionHandler({sequence});

  EXPECT_TRUE(handler->CanInsertInstructionInto(domainconstants::kMessageInstructionType));
  EXPECT_TRUE(handler->CanInsertInstructionAfter(domainconstants::kMessageInstructionType));

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // inserting instruction into selected instruction
  handler->InsertInstructionInto(WaitItem::GetStaticType());
  ASSERT_EQ(sequence->GetInstructions().size(), 1);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // inserting second instruction
  handler->InsertInstructionInto(domainconstants::kMessageInstructionType);
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  // Wait instruction should be after Sequence instruction
  auto instructions = sequence->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(instructions.at(1)->GetType(), UniversalInstructionItem::GetStaticType());
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

  // creating action handler mimicking `wait` instruction selected
  auto handler = CreateActionHandler({wait});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to insert instruction into selected instruction, expecting callback
  EXPECT_NO_THROW(handler->InsertInstructionInto(WaitItem::GetStaticType()));
  ASSERT_EQ(wait->GetInstructions().size(), 0);
}

//! Attempt to insert instruction into something, when nothing is selected.
TEST_F(InstructionEditorActionHandlerTest, InsertIntoWhenNothingIsSelected)
{
  // creating action handler mimicking no instruction selected
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  handler->InsertInstructionInto(WaitItem::GetStaticType());
}

//! Remove operation when nothing is selected.
TEST_F(InstructionEditorActionHandlerTest, RemoveInstructionWhenNothingIsSelected)
{
  // inserting instruction in the container
  m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating action handler mimicking no instruction selected
  auto handler = CreateActionHandler({});

  EXPECT_FALSE(handler->CanRemoveInstruction());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(0);

  // nothing selected, remove request does nothing
  handler->RemoveInstruction();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 1);
}

TEST_F(InstructionEditorActionHandlerTest, RemoveInstruction)
{
  // inserting instruction in the container
  auto sequence0 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto sequence1 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating action handler mimicking sequence0 is selected
  auto handler = CreateActionHandler({sequence0});
  EXPECT_TRUE(handler->CanRemoveInstruction());

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // nothing selected, remove request does nothing
  handler->RemoveInstruction();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 1);

  // checking the request to select remaining item
  EXPECT_EQ(reported_item, sequence1);
}

TEST_F(InstructionEditorActionHandlerTest, RemoveTwoInstructions)
{
  // inserting instruction in the container
  auto sequence0 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto sequence1 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto sequence2 = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // creating action handler mimicking sequence0 is selected
  auto handler = CreateActionHandler({sequence0, sequence1});
  EXPECT_TRUE(handler->CanRemoveInstruction());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(1);

  // nothing selected, remove request does nothing
  handler->RemoveInstruction();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 1);

  // checking the request to select remaining item
  EXPECT_EQ(m_mock_context.GetSelectRequests(), std::vector<mvvm::SessionItem*>({sequence2}));
}

TEST_F(InstructionEditorActionHandlerTest, RemoveParentAndChild)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait = m_model.InsertItem<SequenceItem>(sequence);

  // creating action handler mimicking sequence0 is selected
  auto handler = CreateActionHandler({sequence, wait});
  EXPECT_TRUE(handler->CanRemoveInstruction());

  // nothing selected, remove request does nothing
  handler->RemoveInstruction();
  ASSERT_EQ(m_procedure->GetInstructionContainer()->GetInstructions().size(), 0);

  // checking the request to select remaining item
  EXPECT_TRUE(m_mock_context.GetSelectRequests().empty());
}

//! Move selected instruction up.
TEST_F(InstructionEditorActionHandlerTest, MoveUp)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  // creating action handler mimicking wait2 is selected
  auto handler = CreateActionHandler({wait2});

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // moving selected item up
  handler->MoveUp();

  // checking that the order of instructions has changed
  const std::vector<InstructionItem*> expected({wait0, wait2, wait1});
  EXPECT_EQ(sequence->GetInstructions(), expected);

  // checking the request to select just moved item
  EXPECT_EQ(reported_item, wait2);
}

//! Move selected instruction up.
TEST_F(InstructionEditorActionHandlerTest, MoveDown)
{
  // inserting instruction in the container
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  // creating action handler mimicking wait0 is selected
  auto handler = CreateActionHandler({wait0});

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // moving selected item up
  handler->MoveDown();

  // checking that the order of instructions has changed
  const std::vector<InstructionItem*> expected({wait1, wait0, wait2});
  EXPECT_EQ(sequence->GetInstructions(), expected);

  // checking the request to select just moved item
  EXPECT_EQ(reported_item, wait0);
}

//! Attempt to edit AnyValueItem when nothing appropriate is selected.
TEST_F(InstructionEditorActionHandlerTest, OnEditRequestWhenNothingIsSelected)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());

  // pretending that sequence is selected
  auto handler = CreateActionHandler({sequence});

  // expecting warning callbacks complaining that sequence can't have AnyValueItem
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  handler->OnEditAnyvalueRequest();
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
  EXPECT_EQ(previous_anyvalue->GetType(), sup::gui::AnyValueEmptyItem::GetStaticType());

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(previous_anyvalue))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  auto handler = CreateActionHandler({item});

  EXPECT_CALL(m_mock_context, OnEditAnyvalue(previous_anyvalue)).Times(1);

  handler->OnEditAnyvalueRequest();

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
  EXPECT_EQ(previous_anyvalue->GetType(), sup::gui::AnyValueEmptyItem::GetStaticType());

  // item intentionally uninitialised to mimick item removal in the editor
  std::unique_ptr<sup::gui::AnyValueStructItem> editing_result;

  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(previous_anyvalue))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  auto handler = CreateActionHandler({item});

  // expecting error callback.
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(previous_anyvalue)).Times(1);

  handler->OnEditAnyvalueRequest();

  // checking that instruction still has old AnyValye
  EXPECT_EQ(GetAnyValueItem(*item), previous_anyvalue);
}

}  // namespace oac_tree_gui::test
