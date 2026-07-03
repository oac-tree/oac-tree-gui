/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/jobsystem/objects/job_manager.h"

#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/jobsystem/objects/abstract_job_handler.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <gtest/gtest.h>
#include <testutils/mock_job_handler.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The TestJobHandler is a minimal AbstractJobHandler which lets a test emit the
 * ActiveInstructionChanged signal on demand.
 *
 * We derive from AbstractJobHandler (rather than use MockJobHandler) because JobManager only
 * connects to AbstractJobHandler-based handlers when routing active-instruction notifications.
 */
class TestJobHandler : public AbstractJobHandler
{
public:
  using AbstractJobHandler::AbstractJobHandler;

  void EmitActiveInstructionChanged(const std::vector<const InstructionItem*>& instructions)
  {
    emit ActiveInstructionChanged(GetJobItem(), instructions);
  }
};

}  // namespace

/**
 * @brief Tests for JobManager class using mock dependencies.
 */
class JobManagerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates context necessary for JobManager to function.
   */
  JobManager::create_handler_func_t GetJobHandlerFactoryFunc()
  {
    auto result = [this](JobItem& item) -> std::unique_ptr<IJobHandler>
    { return std::make_unique<test::MockJobHandler>(m_mock_job_handler_listener, &item); };
    return result;
  }

  std::unique_ptr<JobManager> CreateJobManager()
  {
    return std::make_unique<JobManager>(GetJobHandlerFactoryFunc());
  }

  test::MockJobHandlerListener m_mock_job_handler_listener;
};

//! Checking Start method, and Reset method call.
TEST_F(JobManagerTest, InitialState)
{
  LocalJobItem job_item1;

  auto manager = CreateJobManager();
  EXPECT_EQ(manager->GetJobCount(), 0);
  EXPECT_TRUE(manager->GetJobItems().empty());

  manager->SubmitJob(&job_item1);
  EXPECT_EQ(manager->GetJobCount(), 1);
  EXPECT_EQ(manager->GetJobItems(), std::vector<JobItem*>({&job_item1}));

  ImportedJobItem job_item2;
  manager->SubmitJob(&job_item2);
  EXPECT_EQ(manager->GetJobCount(), 2);
  EXPECT_EQ(manager->GetJobItems(), std::vector<JobItem*>({&job_item1, &job_item2}));
}

//! Checking Start method, and Reset method call.
TEST_F(JobManagerTest, StartJob)
{
  LocalJobItem job_item;

  auto manager = CreateJobManager();

  manager->SubmitJob(&job_item);
  auto handler = manager->GetJobHandler(&job_item);
  ASSERT_NE(handler, nullptr);

  ON_CALL(m_mock_job_handler_listener, GetRunnerStatus(::testing::_))
      .WillByDefault(::testing::Return(RunnerStatus::kInitial));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, GetRunnerStatus(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Start(handler));
  }
  manager->Start(&job_item);

  ON_CALL(m_mock_job_handler_listener, GetRunnerStatus(::testing::_))
      .WillByDefault(::testing::Return(RunnerStatus::kHalted));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, GetRunnerStatus(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Reset(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Start(handler));
  }
  manager->Start(&job_item);
}

//! Checking Step method, and Reset method call.
TEST_F(JobManagerTest, StepJob)
{
  LocalJobItem job_item;

  auto manager = CreateJobManager();

  manager->SubmitJob(&job_item);
  auto handler = manager->GetJobHandler(&job_item);
  ASSERT_NE(handler, nullptr);

  ON_CALL(m_mock_job_handler_listener, GetRunnerStatus(::testing::_))
      .WillByDefault(::testing::Return(RunnerStatus::kPaused));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, GetRunnerStatus(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Step(handler));
  }
  manager->Step(&job_item);

  ON_CALL(m_mock_job_handler_listener, GetRunnerStatus(::testing::_))
      .WillByDefault(::testing::Return(RunnerStatus::kSucceeded));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, GetRunnerStatus(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Reset(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Step(handler));
  }
  manager->Step(&job_item);
}

//! Only jobs marked as active via SetActiveJobs propagate ActiveInstructionChanged notifications.
TEST_F(JobManagerTest, SetActiveJobs)
{
  LocalJobItem job_item1;
  LocalJobItem job_item2;

  auto factory = [](JobItem& item) -> std::unique_ptr<IJobHandler>
  { return std::make_unique<TestJobHandler>(&item); };
  JobManager manager(factory);

  manager.SubmitJob(&job_item1);
  manager.SubmitJob(&job_item2);

  auto handler1 = dynamic_cast<TestJobHandler*>(manager.GetJobHandler(&job_item1));
  auto handler2 = dynamic_cast<TestJobHandler*>(manager.GetJobHandler(&job_item2));
  ASSERT_NE(handler1, nullptr);
  ASSERT_NE(handler2, nullptr);

  std::vector<JobItem*> reported_jobs;
  QObject::connect(&manager, &JobManager::ActiveInstructionChanged,
                   [&reported_jobs](JobItem* job, const std::vector<const InstructionItem*>&)
                   { reported_jobs.push_back(job); });

  // no active jobs -> nothing propagates
  handler1->EmitActiveInstructionChanged({});
  handler2->EmitActiveInstructionChanged({});
  EXPECT_TRUE(reported_jobs.empty());

  // only job1 is active -> only job1 propagates
  manager.SetActiveJobs({&job_item1});
  handler1->EmitActiveInstructionChanged({});
  handler2->EmitActiveInstructionChanged({});
  EXPECT_EQ(reported_jobs, std::vector<JobItem*>({&job_item1}));

  // both jobs active -> both propagate
  reported_jobs.clear();
  manager.SetActiveJobs({&job_item1, &job_item2});
  handler1->EmitActiveInstructionChanged({});
  handler2->EmitActiveInstructionChanged({});
  EXPECT_EQ(reported_jobs, std::vector<JobItem*>({&job_item1, &job_item2}));

  // active set cleared -> nothing propagates
  reported_jobs.clear();
  manager.SetActiveJobs({});
  handler1->EmitActiveInstructionChanged({});
  handler2->EmitActiveInstructionChanged({});
  EXPECT_TRUE(reported_jobs.empty());
}

}  // namespace oac_tree_gui
