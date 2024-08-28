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
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/item_utils.h>

#include <sup/auto-server/i_job_info_io.h>
#include <sup/auto-server/job_info.h>
#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

#include <QTest>

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
  using msec = std::chrono::milliseconds;
  JobModel m_model;
};

TEST_F(AutomationJobHandlerTest, InitialState)
{
  auto job_item = m_model.InsertItem<JobItem>();
  auto job_info = testutils::CreateJobInfo(kSequenceTwoWaitsBody);
  AutomationJobHandler handler(job_item, job_info);

  EXPECT_NE(handler.GetJobObserver(), nullptr);
  ASSERT_NE(job_item->GetProcedure(), nullptr);
  EXPECT_EQ(job_item->GetExpandedProcedure(), job_item->GetProcedure());
  EXPECT_EQ(job_item->GetStatus(), std::string());

  auto instruction_items = mvvm::utils::FindItemDown<SequenceItem>(job_item);
  ASSERT_EQ(instruction_items.size(), 1);
  EXPECT_TRUE(instruction_items.at(0)->GetStatus().empty());
}

//! Validating propagation of IJobInfoIO::JobStateUpdated calls to JobItem.
TEST_F(AutomationJobHandlerTest, OnJobStateChanged)
{
  auto job_item = m_model.InsertItem<JobItem>();
  auto job_info = testutils::CreateJobInfo(kSequenceTwoWaitsBody);
  AutomationJobHandler handler(job_item, job_info);

  handler.GetJobObserver()->JobStateUpdated(sup::sequencer::JobState::kRunning);

  // queued connection needs event loop
  auto predicate = [job_item]() { return !job_item->GetStatus().empty(); };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));  // letting event loop to work

  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kRunning);
}

//! Validating propagation of IJobInfoIO::InstructionStateUpdated calls to InstructionItem.
TEST_F(AutomationJobHandlerTest, InstructionStateUpdated)
{
  auto job_item = m_model.InsertItem<JobItem>();
  auto job_info = testutils::CreateJobInfo(kSequenceTwoWaitsBody);
  AutomationJobHandler handler(job_item, job_info);

  // updating instruction state of second Wait instruction
  const size_t index_of_second_wait{2};  // from JobInfo
  handler.GetJobObserver()->InstructionStateUpdated(
      index_of_second_wait,
      sup::auto_server::InstructionState{false, sup::sequencer::ExecutionStatus::RUNNING});

  auto wait_items = mvvm::utils::FindItemDown<WaitItem>(job_item);
  ASSERT_EQ(wait_items.size(), 2);
  auto wait_item1 = wait_items.at(1);

  // queued connection needs event loop
  auto predicate = [wait_item1]() { return !wait_item1->GetStatus().empty(); };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));  // letting event loop to work

  EXPECT_EQ(GetRunnerStatus(wait_item1->GetStatus()), RunnerStatus::kRunning);
}

//! Validating propagation of IJobInfoIO::InstructionStateUpdated calls to InstructionItem.
TEST_F(AutomationJobHandlerTest, OnVariableUpdated)
{
  auto job_item = m_model.InsertItem<JobItem>();
  auto job_info = testutils::CreateJobInfo(kSequenceTwoWaitsBody);
  AutomationJobHandler handler(job_item, job_info);

  auto variable_items = mvvm::utils::FindItemDown<LocalVariableItem>(job_item);
  ASSERT_EQ(variable_items.size(), 2);

  auto anyvalue0 = GetAnyValue(*variable_items.at(0));
  auto anyvalue1 = GetAnyValue(*variable_items.at(1));

  // LocalVariableItem is always available
  EXPECT_TRUE(variable_items.at(0)->IsAvailable());
  EXPECT_TRUE(variable_items.at(1)->IsAvailable());

  EXPECT_EQ(anyvalue0, sup::dto::AnyValue(sup::dto::UnsignedInteger32Type, 0));
  EXPECT_EQ(anyvalue1, sup::dto::AnyValue(sup::dto::UnsignedInteger32Type, 1));

  sup::dto::AnyValue new_anyvalue(sup::dto::UnsignedInteger32Type, 42);

  // updating the value of second variable
  const size_t index_of_second_variable{1};  // from JobInfo
  handler.GetJobObserver()->VariableUpdated(index_of_second_variable, new_anyvalue, false);

  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

          // LocalVariableItem is always available
  EXPECT_EQ(GetAnyValue(*variable_items.at(1)), new_anyvalue);
  EXPECT_TRUE(variable_items.at(1)->IsAvailable());
}
