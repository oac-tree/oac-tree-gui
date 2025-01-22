/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencergui/transform/procedure_item_job_info_builder.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>

#include <sup/sequencer/job_info.h>
#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace sequencergui::test
{

const std::string kSequenceTwoWaitsBody{
    R"RAW(
  <Sequence>
    <Wait timeout="42"/>
    <Wait timeout="43"/>
  </Sequence>
  <Workspace>
    <Local name="var0" type='{"type":"uint32"}' value='0'/>
    <Local name="var1" type='{"type":"uint32"}' value='1'/>
  </Workspace>
)RAW"};

/**
 * @brief Tests for ProcedureItemJobInfoBuilder class.
 */
class ProcedureItemJobInfoBuilderTest : public ::testing::Test
{
};

TEST_F(ProcedureItemJobInfoBuilderTest, GetInstruction)
{
  auto job_info = test::CreateJobInfo(kSequenceTwoWaitsBody);

  ProcedureItemJobInfoBuilder builder;
  auto procedure_item = builder.CreateProcedureItem(job_info);

  ASSERT_EQ(procedure_item->GetInstructionContainer()->GetInstructionCount(), 1);

  auto sequence =
      procedure_item->GetInstructionContainer()->GetItem<SequenceItem>(mvvm::TagIndex::Default(0));
  ASSERT_NE(sequence, nullptr);
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  auto wait0 = sequence->GetItem<WaitItem>(mvvm::TagIndex::Default(0));
  ASSERT_NE(wait0, nullptr);
  EXPECT_EQ(wait0->GetTimeout(), 42);
  auto wait1 = sequence->GetItem<WaitItem>(mvvm::TagIndex::Default(1));
  ASSERT_NE(wait1, nullptr);
  EXPECT_EQ(wait1->GetTimeout(), 43);

  EXPECT_EQ(builder.GetInstruction(0), sequence);
  EXPECT_EQ(builder.GetInstruction(1), wait0);
  EXPECT_EQ(builder.GetInstruction(2), wait1);
  EXPECT_EQ(builder.GetInstruction(3), nullptr);
}

TEST_F(ProcedureItemJobInfoBuilderTest, GetVariable)
{
  auto job_info = test::CreateJobInfo(kSequenceTwoWaitsBody);

  ProcedureItemJobInfoBuilder builder;
  auto procedure_item = builder.CreateProcedureItem(job_info);

  auto variable_items = procedure_item->GetWorkspace()->GetVariables();
  ASSERT_EQ(variable_items.size(), 2);

  EXPECT_EQ(variable_items[0]->GetDisplayName(), "var0");
  EXPECT_EQ(variable_items[1]->GetDisplayName(), "var1");
  EXPECT_EQ(builder.GetVariable(0), variable_items[0]);
  EXPECT_EQ(builder.GetVariable(1), variable_items[1]);
}

TEST_F(ProcedureItemJobInfoBuilderTest, GetInstructionIndex)
{
  auto job_info = test::CreateJobInfo(kSequenceTwoWaitsBody);

  ProcedureItemJobInfoBuilder builder;
  auto procedure_item = builder.CreateProcedureItem(job_info);

  ASSERT_EQ(procedure_item->GetInstructionContainer()->GetInstructionCount(), 1);

  auto sequence =
      procedure_item->GetInstructionContainer()->GetItem<SequenceItem>(mvvm::TagIndex::Default(0));
  ASSERT_NE(sequence, nullptr);
  ASSERT_EQ(sequence->GetInstructions().size(), 2);

  auto wait0 = sequence->GetItem<WaitItem>(mvvm::TagIndex::Default(0));
  auto wait1 = sequence->GetItem<WaitItem>(mvvm::TagIndex::Default(1));

  EXPECT_EQ(builder.GetIndex(sequence), 0);
  EXPECT_EQ(builder.GetIndex(wait0), 1);
  EXPECT_EQ(builder.GetIndex(wait1), 2);

  EXPECT_THROW(builder.GetIndex(nullptr), RuntimeException);
}

TEST_F(ProcedureItemJobInfoBuilderTest, BuildFromIncludeAfterSteup)
{
  const std::string kProcedure{
      R"RAW(
  <Sequence name="CountTwice">
      <Wait/>
  </Sequence>
  <Repeat isRoot="true" maxCount="10">
      <Include name="Counts" path="CountTwice"/>
  </Repeat>
)RAW"};

  auto job_info = test::CreateJobInfo(kProcedure);

  ProcedureItemJobInfoBuilder builder;
  auto procedure_item = builder.CreateProcedureItem(job_info);

  ASSERT_EQ(procedure_item->GetInstructionContainer()->GetInstructionCount(), 1);

  auto repeat_item = procedure_item->GetInstructionContainer()->GetItem<sequencergui::RepeatItem>(
      mvvm::TagIndex::Default(0));

  auto include_item = repeat_item->GetItem<sequencergui::IncludeItem>(mvvm::TagIndex::Default(0));
  auto sequence_item =
      include_item->GetItem<sequencergui::SequenceItem>(mvvm::TagIndex::Default(0));
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>(mvvm::TagIndex::Default(0));

  EXPECT_EQ(builder.GetInstruction(0), repeat_item);
  EXPECT_EQ(builder.GetInstruction(1), include_item);
  EXPECT_EQ(builder.GetInstruction(2), sequence_item);
  EXPECT_EQ(builder.GetInstruction(3), wait_item);
}

}  // namespace sequencergui::test
