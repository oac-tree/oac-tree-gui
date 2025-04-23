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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "test_automation_server.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/automation_client.h>
#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>
#include <chrono>
#include <thread>

#include "oac_tree_gui/jobsystem/job_manager.h"

namespace oac_tree_gui
{

namespace
{

const std::string kServerName = "JobManagerRemoteScenarioTestServer";

const std::string kProcedureBodyText{
    R"RAW(
  <Repeat maxCount="3">
    <Sequence>
       <Increment varName="var0"/>
    </Sequence>
  </Repeat>
  <Workspace>
    <Local name="var0" type='{"type":"uint32"}' value='0'/>
  </Workspace>
)RAW"};

}  // namespace

/**
 * @brief Tests for JobManager class to run remote jobs.
 */
class JobManagerRemoteScenarioTest : public ::testing::Test
{
public:
  JobManagerRemoteScenarioTest() : m_remote_connection_service(GetAutomationClientFactoryFunc())
  {
    // creating remote job item
    const std::size_t job_index{0};
    m_job_item = m_model.InsertItem<RemoteJobItem>();
    m_job_item->SetServerName(kServerName);
    m_job_item->SetRemoteJobIndex(job_index);
  }

  /**
   * @brief Runs remoet server with single procedure on board.
   */
  static void SetUpTestSuite()
  {
    m_test_automation_server.Start(kServerName, kProcedureBodyText);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  static void TearDownTestSuite()
  {
    m_test_automation_server.Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  /**
   * @brief Returns factory function to create automation clients.
   *
   * This function is used to setup RemoteConnectionService.
   */
  static RemoteConnectionService::create_client_t GetAutomationClientFactoryFunc()
  {
    auto result = [](const std::string& name) { return std::make_unique<AutomationClient>(name); };
    return result;
  }

  /**
   * @brief Creates new fully set JobManager.
   */
  std::unique_ptr<JobManager> CreateJobManager()
  {
    return std::make_unique<JobManager>(
        GetJobHandlerFactoryFunc(m_user_context, m_remote_connection_service));
  }

  RemoteConnectionService m_remote_connection_service;
  UserContext m_user_context;
  mvvm::ApplicationModel m_model;
  RemoteJobItem* m_job_item{nullptr};
  static test::TestAutomationServer m_test_automation_server;
};

test::TestAutomationServer JobManagerRemoteScenarioTest::m_test_automation_server;

TEST_F(JobManagerRemoteScenarioTest, InitialState)
{
  auto job_manager = CreateJobManager();
  EXPECT_EQ(job_manager->GetJobCount(), 0);
  EXPECT_TRUE(m_remote_connection_service.GetServerNames().empty());
}

TEST_F(JobManagerRemoteScenarioTest, SubmitJob)
{
  // submitting remote job
  auto job_manager = CreateJobManager();
  job_manager->SubmitJob(m_job_item);

  // validating initial state of RemoteJobHandler
  EXPECT_EQ(job_manager->GetJobCount(), 1);
  EXPECT_EQ(job_manager->GetJobItems(), std::vector<JobItem*>({m_job_item}));
  auto job_handler = job_manager->GetJobHandler(m_job_item);
  ASSERT_NE(job_handler, nullptr);
  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_EQ(job_handler->GetRunnerStatus(), RunnerStatus::kInitial);

  // validating that connection service has established client connection
  EXPECT_TRUE(m_remote_connection_service.HasClient(kServerName));

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [this]() { return m_job_item->GetStatus() == RunnerStatus::kInitial; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  // validating internal expanded ProcedureItem
  auto procedure_item = m_job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = m_job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 0};
  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

TEST_F(JobManagerRemoteScenarioTest, SubmitJobAndStart)
{
  // submitting remote job
  auto job_manager = CreateJobManager();
  job_manager->SubmitJob(m_job_item);

  job_manager->Start(m_job_item);

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [this]() { return m_job_item->GetStatus() == RunnerStatus::kSucceeded; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_FALSE(job_manager->GetJobHandler(m_job_item)->IsRunning());

  // validating internal expanded ProcedureItem
  auto procedure_item = m_job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = m_job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 3};
  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

}  // namespace oac_tree_gui
