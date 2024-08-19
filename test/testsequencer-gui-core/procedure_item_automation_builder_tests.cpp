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

#include "sequencergui/transform/procedure_item_automation_builder.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>

#include <sup/auto-server/instruction_map.h>
#include <sup/auto-server/job_info.h>
#include <sup/auto-server/job_utils.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

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

//! Tests for ProcedureItemAutomationBuilder class.

class ProcedureItemAutomationBuilderTest : public ::testing::Test
{
public:
  /**
   * @brief Helper method to create JobInfo from procedure text body
   */
  static sup::auto_server::JobInfo CreateJobInfo(const std::string& procedure_text)
  {
    const std::string prefix = "JobInfoTest:FromProcedure:";
    auto procedure =
        sup::sequencer::ParseProcedureString(testutils::CreateProcedureString(procedure_text));
    auto root = procedure->RootInstruction();
    const sup::auto_server::InstructionMap instr_map{root};
    auto job_info = sup::auto_server::utils::CreateJobInfo(prefix, *procedure, instr_map);

    return job_info;
  };
};

TEST_F(ProcedureItemAutomationBuilderTest, SequenceWithTwoWaits)
{
  auto job_info = CreateJobInfo(kSequenceTwoWaitsBody);

  ProcedureItemAutomationBuilder builder;
  auto procedure_item = builder.CreateProcedureItem(job_info);

  // EXPECT_EQ(procedure_item->GetDisplayName(), std::string());

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

  auto variable_items = procedure_item->GetWorkspace()->GetVariables();
  ASSERT_EQ(variable_items.size(), 2);

  EXPECT_EQ(variable_items[0]->GetDisplayName(), "var0");
  EXPECT_EQ(variable_items[1]->GetDisplayName(), "var1");
  EXPECT_EQ(builder.GetVariable(0), variable_items[0]);
  EXPECT_EQ(builder.GetVariable(1), variable_items[1]);
}
