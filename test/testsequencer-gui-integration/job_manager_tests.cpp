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

#include "sequencergui/jobsystem/job_manager.h"

#include <gtest/gtest.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_context.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/monitor/message_panel.h>
#include <testutils/standard_procedure_items.h>

#include <mvvm/model/model_utils.h>

#include <sup/sequencer/exceptions.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobManager.

class JobManagerTest : public ::testing::Test
{
public:
  JobManagerTest() { m_job_item = m_models.GetJobModel()->InsertItem<JobItem>(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

TEST_F(JobManagerTest, InitialState)
{
  JobManager manager;
  EXPECT_FALSE(manager.GetCurrentContext());
  EXPECT_FALSE(manager.GetContext(m_job_item));
  EXPECT_FALSE(manager.GetCurrentJob());
}

TEST_F(JobManagerTest, SubmitProcedure)
{
  auto copy_procedure = testutils::CreateCopyProcedure(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  EXPECT_EQ(m_job_item->GetExpandedProcedure(), nullptr);

  JobManager manager;
  manager.SubmitJob(m_job_item);
  EXPECT_FALSE(manager.GetCurrentJob());

  ASSERT_TRUE(manager.GetContext(m_job_item));
  EXPECT_EQ(manager.GetContext(m_job_item)->GetExpandedProcedure(),
            m_job_item->GetExpandedProcedure());
}

TEST_F(JobManagerTest, AttemptToSubmitProcedure)
{
  auto copy_procedure = testutils::CreateCopyProcedure(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  JobManager manager;

  // it shouldn't be possible to submit undefined job
  EXPECT_THROW(manager.SubmitJob(nullptr), RuntimeException);

  manager.SubmitJob(m_job_item);

  // it shouldn't be possible to submit job twice
  EXPECT_THROW(manager.SubmitJob(m_job_item), RuntimeException);
}

//! Attempt to submit wronly configured procedure.

TEST_F(JobManagerTest, AttemptToSubmitMalformedProcedure)
{
  auto invalid_procedure = testutils::CreateInvalidProcedure(GetSequencerModel());
  m_job_item->SetProcedure(invalid_procedure);

  JobManager manager;

  EXPECT_THROW(manager.SubmitJob(m_job_item), sup::sequencer::InvalidOperationException);
}

//! Set first procedure to the JobManager and execute it.

TEST_F(JobManagerTest, SetCurrentJobAndExecute)
{
  auto copy_procedure = testutils::CreateCopyProcedure(GetSequencerModel());

  MessagePanel panel;
  m_job_item->SetProcedure(copy_procedure);

  JobManager manager;
  manager.SetMessagePanel(&panel);
  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);
  EXPECT_EQ(manager.GetCurrentJob(), m_job_item);
  manager.onChangeDelayRequest(10);

  QSignalSpy spy_instruction_status(&manager, &JobManager::InstructionStatusChanged);

  auto context = manager.GetCurrentContext();
  ASSERT_TRUE(context != nullptr);

  EXPECT_FALSE(context->IsRunning());

  auto procedure = context->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariableCount(), 2);

  // starting procedure
  manager.OnStartJobRequest();
  EXPECT_TRUE(context->IsRunning());

  // We are testing here queued signals, need special waiting to let procedure complete
  QTest::qWait(100);
  EXPECT_FALSE(context->IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  EXPECT_EQ(inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(inside.at(1)->GetJsonValue(), std::string("43"));
}

//! Removing submitted job.

TEST_F(JobManagerTest, OnRemoveJobRequest)
{
  auto copy_procedure = testutils::CreateCopyProcedure(GetSequencerModel());

  MessagePanel panel;
  m_job_item->SetProcedure(copy_procedure);

  JobManager manager;
  manager.SetMessagePanel(&panel);

  // nothing wrong if we are trying to remove non-submitted job
  EXPECT_NO_THROW(manager.OnRemoveJobRequest(m_job_item));

  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);

  // should be possible to remove submitted, but non-running job
  EXPECT_NO_THROW(manager.OnRemoveJobRequest(m_job_item));

  EXPECT_EQ(manager.GetCurrentContext(), nullptr);
  EXPECT_EQ(manager.GetContext(m_job_item), nullptr);
  EXPECT_EQ(manager.GetCurrentJob(), nullptr);
}

//! Attempt to remove long running job.

TEST_F(JobManagerTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10000));

  MessagePanel panel;
  m_job_item->SetProcedure(procedure);

  JobManager manager;
  manager.SetMessagePanel(&panel);

  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);
  manager.OnStartJobRequest();

  auto context = manager.GetCurrentContext();
  EXPECT_TRUE(context->IsRunning());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_THROW(manager.OnRemoveJobRequest(m_job_item), RuntimeException);
  QTest::qWait(20);

  manager.OnStopJobRequest();
  QTest::qWait(20);

  EXPECT_FALSE(context->IsRunning());
}
