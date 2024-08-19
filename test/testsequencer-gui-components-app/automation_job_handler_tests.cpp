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

#include "sequencergui/automation/automation_job_handler.h"

#include <sequencergui/jobsystem/runner_status.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
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

//! Tests for AutomationJobHandler class.

class AutomationJobHandlerTest : public ::testing::Test
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

  JobModel m_model;
};

TEST_F(AutomationJobHandlerTest, InitialState)
{
  auto job_item = m_model.InsertItem<JobItem>();

  auto job_info = CreateJobInfo(kSequenceTwoWaitsBody);

  AutomationJobHandler handler(job_item, job_info);

  EXPECT_NE(handler.GetJobObserver(), nullptr);
  ASSERT_NE(job_item->GetProcedure(), nullptr);
  EXPECT_EQ(job_item->GetExpandedProcedure(), job_item->GetProcedure());
  EXPECT_EQ(job_item->GetStatus(), std::string());
}
