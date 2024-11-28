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
#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/model/standard_job_items.h>

#include <gtest/gtest.h>
#include <testutils/mock_job_handler.h>

namespace sequencergui
{

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
    { return std::make_unique<testutils::MockJobHandler>(m_mock_job_handler_listener, &item); };
    return result;
  }

  std::unique_ptr<JobManager> CreateJobManager()
  {
    return std::make_unique<JobManager>(GetJobHandlerFactoryFunc());
  }

  testutils::MockJobHandlerListener m_mock_job_handler_listener;
};

//! Step for not-running job should call job reset before.
TEST_F(JobManagerTest, StepFinishedJob)
{
  LocalJobItem job_item;

  auto manager = CreateJobManager();

  manager->SubmitJob(&job_item);
  auto handler = manager->GetJobHandler(&job_item);
  ASSERT_NE(handler, nullptr);

  // letd job handler report that it is not running
  ON_CALL(m_mock_job_handler_listener, IsRunning(::testing::_))
      .WillByDefault(::testing::Return(false));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, IsRunning(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Reset(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Step(handler));
  }

  manager->Step(&job_item);
}

//! Step for running job should be simply forwarded.
TEST_F(JobManagerTest, StepRunningJob)
{
  LocalJobItem job_item;

  auto manager = CreateJobManager();

  manager->SubmitJob(&job_item);
  auto handler = manager->GetJobHandler(&job_item);
  ASSERT_NE(handler, nullptr);

  // letd job handler report that it is running
  ON_CALL(m_mock_job_handler_listener, IsRunning(::testing::_))
      .WillByDefault(::testing::Return(true));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_job_handler_listener, IsRunning(handler));
    EXPECT_CALL(m_mock_job_handler_listener, Step(handler));
  }

  manager->Step(&job_item);
}

}  // namespace sequencergui
