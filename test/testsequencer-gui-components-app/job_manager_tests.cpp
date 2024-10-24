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

#include "sequencergui/jobsystem/job_manager.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

#include <mvvm/model/model_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/exceptions.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

/**
 * @brief Tests for JobManager class.
 */
class JobManagerTest : public ::testing::Test
{
public:
  JobManagerTest()
  {
    m_models.CreateNewProject();
    m_job_item = m_models.GetJobModel()->InsertItem<JobItem>();
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

TEST_F(JobManagerTest, InitialState)
{
  JobManager manager({});
  EXPECT_FALSE(manager.GetCurrentJobHandler());
  EXPECT_FALSE(manager.GetJobHandler(m_job_item));
  EXPECT_FALSE(manager.GetCurrentJob());
  EXPECT_FALSE(manager.HasRunningJobs());
}

TEST_F(JobManagerTest, SubmitProcedure)
{
  auto copy_procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  EXPECT_EQ(m_job_item->GetExpandedProcedure(), nullptr);

  JobManager manager({});
  manager.SubmitJob(m_job_item);
  EXPECT_FALSE(manager.GetCurrentJob());

  ASSERT_TRUE(manager.GetJobHandler(m_job_item));
  EXPECT_EQ(manager.GetJobHandler(m_job_item)->GetExpandedProcedure(),
            m_job_item->GetExpandedProcedure());
}

TEST_F(JobManagerTest, AttemptToSubmitProcedure)
{
  auto copy_procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  JobManager manager({});

  // it shouldn't be possible to submit undefined job
  EXPECT_THROW(manager.SubmitJob(nullptr), RuntimeException);

  manager.SubmitJob(m_job_item);

  // it shouldn't be possible to submit job twice
  EXPECT_THROW(manager.SubmitJob(m_job_item), RuntimeException);
}

//! Attempt to submit wrongly configured procedure.
TEST_F(JobManagerTest, AttemptToSubmitMalformedProcedure)
{
  auto invalid_procedure = testutils::CreateInvalidProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(invalid_procedure);

  JobManager manager({});

  EXPECT_THROW(manager.SubmitJob(m_job_item), sup::sequencer::InvalidOperationException);
}

//! Set first procedure to the JobManager and execute it.
TEST_F(JobManagerTest, SetCurrentJobAndExecute)
{
  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  auto copy_procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());

  m_job_item->SetProcedure(copy_procedure);

  JobManager manager({});

  JobManager::set_joblog_cb callback = [this](auto) {};

  manager.SetMessagePanel(callback);
  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);
  EXPECT_EQ(manager.GetCurrentJob(), m_job_item);

  auto job_handler = manager.GetCurrentJobHandler();
  ASSERT_TRUE(job_handler != nullptr);

  EXPECT_FALSE(job_handler->IsRunning());

  auto procedure = job_handler->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariableCount(), 2);

  // starting procedure
  manager.OnStartJobRequest();
  QTest::qWait(20);

  // We are testing here queued signals, need special waiting to let procedure complete
  EXPECT_TRUE(QTest::qWaitFor([&job_handler]() { return !job_handler->IsRunning(); }, 100));

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);

  EXPECT_TRUE(testutils::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(testutils::IsEqual(*var_inside1, anyvalue0));

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  auto var0 = inside.at(0);
  auto var1 = inside.at(1);
  EXPECT_TRUE(testutils::IsEqual(*var0, anyvalue0));
  EXPECT_TRUE(testutils::IsEqual(*var1, anyvalue1));
}

//! Removing submitted job.
TEST_F(JobManagerTest, OnRemoveJobRequest)
{
  auto copy_procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());

  m_job_item->SetProcedure(copy_procedure);

  JobManager::set_joblog_cb callback = [this](auto) {};

  JobManager manager({});
  manager.SetMessagePanel(callback);

  // nothing wrong if we are trying to remove non-submitted job
  EXPECT_NO_THROW(manager.OnRemoveJobRequest(m_job_item));

  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);

  // should be possible to remove submitted, but non-running job
  EXPECT_NO_THROW(manager.OnRemoveJobRequest(m_job_item));

  EXPECT_EQ(manager.GetCurrentJobHandler(), nullptr);
  EXPECT_EQ(manager.GetJobHandler(m_job_item), nullptr);
  EXPECT_EQ(manager.GetCurrentJob(), nullptr);
}

//! Attempt to remove long running job.

TEST_F(JobManagerTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  m_job_item->SetProcedure(procedure);

  JobManager::set_joblog_cb callback = [this](auto) {};

  JobManager manager({});
  manager.SetMessagePanel(callback);

  manager.SubmitJob(m_job_item);
  manager.SetCurrentJob(m_job_item);
  manager.OnStartJobRequest();

  auto job_handler = manager.GetCurrentJobHandler();
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return job_handler->IsRunning(); }, 50));

  EXPECT_TRUE(job_handler->IsRunning());
  EXPECT_TRUE(manager.HasRunningJobs());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_THROW(manager.OnRemoveJobRequest(m_job_item), RuntimeException);
  QTest::qWait(20);

  manager.OnStopJobRequest();
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return !job_handler->IsRunning(); }, 50));

  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_FALSE(manager.HasRunningJobs());
}

//! Long running job removal.
TEST_F(JobManagerTest, StopAllJobs)
{
  auto procedure0 = testutils::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));
  auto procedure1 = testutils::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  m_job_item->SetProcedure(procedure0);
  auto job_item1 = m_job_item;

  auto job_item2 = m_models.GetJobModel()->InsertItem<JobItem>();
  job_item2->SetProcedure(procedure1);

  JobManager manager({});
  manager.SetMessagePanel([this](auto) {});

  EXPECT_NO_FATAL_FAILURE(manager.StopAllJobs());

  manager.SubmitJob(job_item1);
  manager.SubmitJob(job_item2);
  manager.SetCurrentJob(job_item1);
  manager.OnStartJobRequest();

  manager.SetCurrentJob(job_item2);
  manager.OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([&manager]() { return manager.HasRunningJobs(); }, 100));

  EXPECT_TRUE(manager.HasRunningJobs());

  manager.StopAllJobs();
  EXPECT_TRUE(QTest::qWaitFor([&manager]() { return !manager.HasRunningJobs(); }, 100));

  EXPECT_FALSE(manager.HasRunningJobs());
}
