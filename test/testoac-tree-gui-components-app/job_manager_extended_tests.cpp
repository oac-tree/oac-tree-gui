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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/abstract_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/model/model_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/exceptions.h>

#include <gtest/gtest.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>

#include "oac_tree_gui/jobsystem/job_manager.h"

using msec = std::chrono::milliseconds;

namespace oac_tree_gui::test
{

/**
 * @brief Tests for JobManager class by running actual sequencer procedures.
 */
class JobManagerExtendedTest : public ::testing::Test
{
public:
  JobManagerExtendedTest()
  {
    m_models.CreateEmpty();
    m_job_item = m_models.GetJobModel()->InsertItem<LocalJobItem>();
  }

  /**
   * @brief Creates context necessary for JobManager to function.
   */
  JobManager::create_handler_func_t GetContext()
  {
    return GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  ApplicationModels m_models;
  LocalJobItem* m_job_item{nullptr};
  test::MockRemoteConnectionService m_mock_connection_service;
  UserContext m_user_context;
};

TEST_F(JobManagerExtendedTest, InitialState)
{
  JobManager manager(GetContext());
  EXPECT_EQ(manager.GetJobHandler(m_job_item), nullptr);
  EXPECT_FALSE(manager.HasRunningJobs());
}

TEST_F(JobManagerExtendedTest, SubmitProcedure)
{
  auto copy_procedure = test::CreateCopyProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  EXPECT_EQ(m_job_item->GetExpandedProcedure(), nullptr);

  JobManager manager(GetContext());
  manager.SubmitJob(m_job_item);

  ASSERT_TRUE(manager.GetJobHandler(m_job_item));
  EXPECT_EQ(manager.GetJobHandler(m_job_item)->GetExpandedProcedure(),
            m_job_item->GetExpandedProcedure());
}

TEST_F(JobManagerExtendedTest, AttemptToSubmitProcedure)
{
  auto copy_procedure = test::CreateCopyProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(copy_procedure);

  JobManager manager(GetContext());

  // it shouldn't be possible to submit undefined job
  EXPECT_THROW(manager.SubmitJob(nullptr), RuntimeException);

  manager.SubmitJob(m_job_item);

  // it shouldn't be possible to submit job twice
  EXPECT_THROW(manager.SubmitJob(m_job_item), RuntimeException);
}

//! Attempt to submit wrongly configured procedure.
TEST_F(JobManagerExtendedTest, AttemptToSubmitMalformedProcedure)
{
  auto invalid_procedure = test::CreateInvalidProcedureItem(GetSequencerModel());
  m_job_item->SetProcedure(invalid_procedure);

  JobManager manager(GetContext());

  EXPECT_THROW(manager.SubmitJob(m_job_item), sup::oac_tree::ProcedureSetupException);
}

//! Set first procedure to the JobManager and execute it.
TEST_F(JobManagerExtendedTest, SetCurrentJobAndExecute)
{
  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  auto copy_procedure = test::CreateCopyProcedureItem(GetSequencerModel());

  m_job_item->SetProcedure(copy_procedure);

  JobManager manager(GetContext());

  manager.SubmitJob(m_job_item);

  auto job_handler = manager.GetJobHandler(m_job_item);
  ASSERT_TRUE(job_handler != nullptr);

  EXPECT_FALSE(job_handler->IsRunning());

  auto procedure = job_handler->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariableCount(), 2);

  // starting procedure
  manager.Start(m_job_item);
  QTest::qWait(20);

  // We are testing here queued signals, need special waiting to let procedure complete

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);

  auto predicate = [&job_handler, var_inside0, var_inside1, &anyvalue0]
  {
    const bool is_completed = !job_handler->IsRunning();
    const bool var0_updated = test::IsEqual(*var_inside0, anyvalue0);
    const bool var1_updated = test::IsEqual(*var_inside1, anyvalue0);
    return is_completed && var0_updated && var1_updated;
  };

  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue0));

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  auto var0 = inside.at(0);
  auto var1 = inside.at(1);
  EXPECT_TRUE(test::IsEqual(*var0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var1, anyvalue1));
}

//! Removing submitted job.
TEST_F(JobManagerExtendedTest, OnRemoveJobRequest)
{
  auto copy_procedure = test::CreateCopyProcedureItem(GetSequencerModel());

  m_job_item->SetProcedure(copy_procedure);

  JobManager manager(GetContext());

  // nothing wrong if we are trying to remove non-submitted job
  EXPECT_NO_THROW(manager.RemoveJobHandler(m_job_item));

  manager.SubmitJob(m_job_item);

  // should be possible to remove submitted, but non-running job
  EXPECT_NO_THROW(manager.RemoveJobHandler(m_job_item));

  EXPECT_EQ(manager.GetJobHandler(m_job_item), nullptr);
}

//! Attempt to remove long running job.
TEST_F(JobManagerExtendedTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = test::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  m_job_item->SetProcedure(procedure);

  JobManager manager(GetContext());

  manager.SubmitJob(m_job_item);
  manager.Start(m_job_item);

  auto job_handler = manager.GetJobHandler(m_job_item);
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return job_handler->IsRunning(); }, 50));

  EXPECT_TRUE(job_handler->IsRunning());
  EXPECT_TRUE(manager.HasRunningJobs());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_THROW(manager.RemoveJobHandler(m_job_item), RuntimeException);
  QTest::qWait(20);

  manager.Stop(m_job_item);
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return !job_handler->IsRunning(); }, 50));

  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_FALSE(manager.HasRunningJobs());
}

//! Long running job removal.
TEST_F(JobManagerExtendedTest, StopAllJobs)
{
  auto procedure0 = test::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));
  auto procedure1 = test::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  m_job_item->SetProcedure(procedure0);
  auto job_item1 = m_job_item;

  auto job_item2 = m_models.GetJobModel()->InsertItem<LocalJobItem>();
  job_item2->SetProcedure(procedure1);

  JobManager manager(GetContext());

  EXPECT_NO_FATAL_FAILURE(manager.StopAllJobs());

  manager.SubmitJob(job_item1);
  manager.SubmitJob(job_item2);
  manager.Start(job_item1);

  manager.Start(job_item1);

  EXPECT_TRUE(QTest::qWaitFor([&manager]() { return manager.HasRunningJobs(); }, 100));

  EXPECT_TRUE(manager.HasRunningJobs());

  manager.StopAllJobs();
  EXPECT_TRUE(QTest::qWaitFor([&manager]() { return !manager.HasRunningJobs(); }, 100));

  EXPECT_FALSE(manager.HasRunningJobs());
}

}  // namespace oac_tree_gui::test
