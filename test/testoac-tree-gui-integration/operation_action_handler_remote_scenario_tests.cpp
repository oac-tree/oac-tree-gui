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

#include "oac_tree_gui/operation/objects/operation_action_handler.h"
#include "test_automation_server.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/automation_client.h>
#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>
#include <chrono>
#include <thread>

namespace oac_tree_gui
{

namespace
{

const std::string kServerName = "OperationActionHandlerRemoteScenarioTestServer";
const std::size_t kJobIndex{0};

const std::string kProcedureBodyText{
    R"RAW(
  <Repeat maxCount="3">
    <Sequence>
      <Output fromVar="var0" description="Variable is"/>
      <Increment varName="var0"/>
      <Message text="Variable was incremented"/>
      <Wait timeout="0.01"/>
    </Sequence>
  </Repeat>
  <Workspace>
    <Local name="var0" type='{"type":"int32"}' value="42"/>
  </Workspace>
)RAW"};

}  // namespace

/**
 * @brief Tests for OperationActionHandler class in his full glory to run remote jobs.
 */
class OperationActionHandlerRemoteScenarioTest : public ::testing::Test
{
public:
  OperationActionHandlerRemoteScenarioTest()
      : m_remote_connection_service(GetAutomationClientFunc())
      , m_job_manager(GetJobHandlerFactoryFunc(m_user_context, m_remote_connection_service))
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
  }

  /**
   * @brief Runs remote server with single procedure on board.
   */
  static void SetUpTestSuite()
  {
    m_test_automation_server.Start(kServerName, kProcedureBodyText);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  static void TearDownTestSuite() { m_test_automation_server.Stop(); }

  /**
   * @brief Returns factory function to create automation clients.
   *
   * This function is used to setup RemoteConnectionService.
   */
  static RemoteConnectionService::create_client_t GetAutomationClientFunc()
  {
    auto result = [](const std::string& name) { return std::make_unique<AutomationClient>(name); };
    return result;
  }

  /**
   * @brief Creates operation action handler.
   */
  std::unique_ptr<OperationActionHandler> CreateOperationHandler()
  {
    auto result =
        std::make_unique<OperationActionHandler>(&m_job_manager, m_mock_context.CreateContext());
    result->SetJobContainer(GetJobModel()->GetRootItem());
    return result;
  }

  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  template <typename T>
  std::vector<T*> GetJobs()
  {
    return GetJobModel()->GetRootItem()->GetItems<T>(mvvm::TagIndex::GetDefaultTag());
  }

  ApplicationModels m_models;
  RemoteConnectionService m_remote_connection_service;
  UserContext m_user_context;
  JobManager m_job_manager;
  test::MockOperationActionContext m_mock_context;
  static test::TestAutomationServer m_test_automation_server;
};

test::TestAutomationServer OperationActionHandlerRemoteScenarioTest::m_test_automation_server;

TEST_F(OperationActionHandlerRemoteScenarioTest, InitialState)
{
  auto handler = CreateOperationHandler();
  EXPECT_EQ(m_job_manager.GetJobCount(), 0);
  EXPECT_TRUE(m_remote_connection_service.GetServerNames().empty());
}

TEST_F(OperationActionHandlerRemoteScenarioTest, OnImportRemoteJobRequest)
{
  auto handler = CreateOperationHandler();

  // setup remote dialog to return remote job info
  // setting up context, so it report "user choice" related to remote job import
  const RemoteConnectionInfo connection_context{kServerName, {kJobIndex}};
  ON_CALL(m_mock_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));

  // submit job
  EXPECT_CALL(m_mock_context, OnGetRemoteConnectionInfo());
  EXPECT_CALL(m_mock_context, OnSelectedJob());
  handler->OnImportRemoteJobRequest();

  // validating job model
  auto submitted_jobs = GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);
  EXPECT_EQ(job_item->GetServerName(), kServerName);
  EXPECT_EQ(job_item->GetRemoteJobIndex(), kJobIndex);

  // validating initial state of RemoteJobHandler
  EXPECT_EQ(m_job_manager.GetJobCount(), 1);
  EXPECT_EQ(m_job_manager.GetJobItems(), std::vector<JobItem*>({job_item}));
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  ASSERT_NE(job_handler, nullptr);
  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_EQ(job_handler->GetRunnerStatus(), RunnerStatus::kInitial);

  // validating that connection service has established client connection
  EXPECT_TRUE(m_remote_connection_service.HasClient(kServerName));

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [this, job_item]() { return job_item->GetStatus() == RunnerStatus::kInitial; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 5000));

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 42};

  auto predicate2 = [this, &expected_value, &variables]()
  { return test::IsEqual(*variables.at(0), expected_value); };
  EXPECT_TRUE(QTest::qWaitFor(predicate2, 5000));

  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

TEST_F(OperationActionHandlerRemoteScenarioTest, ImportRemoteJobAndStart)
{
  auto handler = CreateOperationHandler();

  // setup remote dialog to return remote job info
  // setting up context, so it report "user choice" related to remote job import
  const RemoteConnectionInfo connection_context{kServerName, {kJobIndex}};
  ON_CALL(m_mock_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));

  // submit job
  EXPECT_CALL(m_mock_context, OnGetRemoteConnectionInfo());
  EXPECT_CALL(m_mock_context, OnSelectedJob());
  handler->OnImportRemoteJobRequest();

  // validating job model
  auto submitted_jobs = GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);

  // making item selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob());
  handler->OnStartJobRequest();

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [this, job_item]() { return job_item->GetStatus() == RunnerStatus::kSucceeded; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 500));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 45};

  auto predicate2 = [this, &expected_value, &variables]()
  { return test::IsEqual(*variables.at(0), expected_value); };
  EXPECT_TRUE(QTest::qWaitFor(predicate2, 5000));

  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

}  // namespace oac_tree_gui
