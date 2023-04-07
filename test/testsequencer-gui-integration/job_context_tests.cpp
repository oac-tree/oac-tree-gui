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

#include "sequencergui/jobsystem/job_context.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/request_types.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/monitor/job_log.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>
#include <testutils/mock_item_listener.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>
#include <thread>
#include <tuple>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobContext class.

class JobContextTest : public ::testing::Test
{
public:
  JobContextTest() { m_job_item = m_models.GetJobModel()->InsertItem<JobItem>(); }

  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

TEST_F(JobContextTest, InitialState)
{
  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  EXPECT_EQ(job_context.GetExpandedProcedure(), nullptr);
}

//! Calling PrepareJobRequest.

TEST_F(JobContextTest, PrepareJobRequest)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  EXPECT_EQ(job_context.GetExpandedProcedure(), nullptr);

  job_context.onPrepareJobRequest();

  auto expanded_procedure = job_context.GetExpandedProcedure();
  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_EQ(job_context.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());
  EXPECT_TRUE(job_context.IsValid());

  // calling expanded second time
  job_context.onPrepareJobRequest();

  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_NE(job_context.GetExpandedProcedure(),
            expanded_procedure);  // old procedure was regenerated
  EXPECT_EQ(job_context.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());
}

//! Attempt to use JobContext with invalid procedure.

TEST_F(JobContextTest, InvalidProcedure)
{
  auto procedure = testutils::CreateInvalidProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_THROW(job_context.onPrepareJobRequest(), sup::sequencer::InvalidOperationException);
  EXPECT_FALSE(job_context.IsValid());
}

//! Delete JobCOntext after procedure start.

TEST_F(JobContextTest, PrematureDeletion)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  {
    JobContext job_context(m_job_item);
    job_context.onPrepareJobRequest();
    job_context.onStartRequest();
  }

  EXPECT_EQ(m_job_item->GetStatus(), std::string());
}

//! Normal execution of the procedure with single wait instruction.

TEST_F(JobContextTest, ProcedureWithSingleWait)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);
}

//! Normal execution of procedure with single wait.
//! Validating signaling going from expanded procedure (instruction status change).

TEST_F(JobContextTest, ProcedureWithSingleWaitStatusChangedSignals)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  auto wait = instructions.at(0);

  testutils::MockItemListener listener(wait);

  mvvm::PropertyChangedEvent expected_event{wait, itemconstants::kStatus};
  EXPECT_CALL(listener, OnEvent(mvvm::event_variant_t(expected_event))).Times(2);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);
}

TEST_F(JobContextTest, ProcedureWithVariableCopy)
{
  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  auto procedure = testutils::CreateCopyProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto vars = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars.size(), 2);

  auto initial_anyvalue_item0 = vars.at(0)->GetAnyValueItem();
  auto initial_anyvalue_item1 = vars.at(1)->GetAnyValueItem();

  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item1), anyvalue1);

  JobContext job_context(m_job_item);
  job_context.onPrepareJobRequest();

  // expanded procedure has different variables
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto new_anyvalue_item0 = vars_inside.at(0)->GetAnyValueItem();
  auto new_anyvalue_item1 = vars_inside.at(1)->GetAnyValueItem();

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue1);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue0);  // value was changed
}

TEST_F(JobContextTest, LocalIncludeScenario)
{
  auto procedure = testutils::CreateIncludeProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  job_context.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 8);  // Repeat, Include, Sequence, Wait x 2

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

TEST_F(JobContextTest, UserInputScenario)
{
  // value defined in testutils::CreateInputProcedureItem
  const sup::dto::AnyValue initial_value{sup::dto::SignedInteger32Type, 0};

  auto procedure = testutils::CreateInputProcedureItem(m_models.GetSequencerModel());

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_EQ(sup::gui::CreateAnyValue(*vars_inside.at(0)->GetAnyValueItem()), initial_value);

  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  job_context.onPrepareJobRequest();

  const sup::dto::AnyValue new_value{sup::dto::SignedInteger32Type, 42};
  auto on_user_input = [new_value](auto)
  {
    return UserInputResult{new_value, true};
  };
  job_context.SetUserContext({on_user_input});

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 6); // 3 instructions

  EXPECT_FALSE(job_context.IsRunning());

  vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_EQ(sup::gui::CreateAnyValue(*vars_inside.at(0)->GetAnyValueItem()), new_value);
}

TEST_F(JobContextTest, UserChoiceScenario)
{
  // value defined in testutils::CreateUserChoiceProcedureItem
  const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};

  // creating test procedure offering user the choice between two instructions: 0) wait 1) copy
  auto procedure = testutils::CreateUserChoiceProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  job_context.onPrepareJobRequest();

  // callback to select Copy instruction
  auto on_user_choice = [](auto) { return UserChoiceResult{1, true}; };
  job_context.SetUserContext({{}, on_user_choice});

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 4);

  // validating that the copy instruction worked, i.e. that is has successfully copied var0 into
  // var1
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto anyvalue_item = vars_inside.at(1)->GetAnyValueItem();
  EXPECT_EQ(sup::gui::CreateAnyValue(*anyvalue_item), expected_anyvalue);
  EXPECT_FALSE(job_context.IsRunning());
}

//! Stop long running job.

TEST_F(JobContextTest, StopLongRunningJob)
{
  auto procedure =
      testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10000));
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(40);

  EXPECT_TRUE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 1);

  job_context.onStopRequest();
  QTest::qWait(20);

  EXPECT_FALSE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);
}

//! Control log events with the help of MessageInstruction.

TEST_F(JobContextTest, LogEvents)
{
  const std::string expected_message("abc");
  auto procedure =
      testutils::CreateMessageProcedureItem(m_models.GetSequencerModel(), expected_message);
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(50);
  //  EXPECT_TRUE(QTest::qWaitFor([&job_context]() { return !job_context.IsRunning(); }, 10));

  EXPECT_FALSE(job_context.IsRunning());

  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = mvvm::utils::FindItems<MessageItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);

  ASSERT_EQ(job_context.GetJobLog()->GetSize(), 2);

  auto event1 = job_context.GetJobLog()->At(0);
  EXPECT_EQ(event1.severity, Severity::kDebug);
  // FIXME message will probably change, how to make test robust?
  EXPECT_EQ(event1.message, std::string("StartSingleStep()"));

  auto event2 = job_context.GetJobLog()->At(1);
  EXPECT_EQ(event2.severity, Severity::kInfo);
  EXPECT_EQ(event2.message, expected_message);
}
