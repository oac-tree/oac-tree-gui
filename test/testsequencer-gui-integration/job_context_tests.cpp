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
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

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
  auto procedure = testutils::CreateCopyProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto vars = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars.size(), 2);
  EXPECT_EQ(vars.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars.at(1)->GetJsonValue(), std::string("43"));

  JobContext job_context(m_job_item);
  job_context.onPrepareJobRequest();

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));
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
  auto procedure = testutils::CreateInputProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  job_context.onPrepareJobRequest();

  auto on_user_input = [](auto, auto) { return "42"; };
  job_context.SetUserContext({on_user_input});

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job_context.IsRunning());
}

TEST_F(JobContextTest, UserChoiceScenario)
{
  auto procedure = testutils::CreateUserChoiceProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  job_context.onPrepareJobRequest();

  // callback to select Copy instruction
  auto on_user_choice = [](auto, auto) { return 1; };
  job_context.SetUserContext({{}, on_user_choice});

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 4);

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job_context.IsRunning());
}

//! Stop long running job.

TEST_F(JobContextTest, StopLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10000));
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
