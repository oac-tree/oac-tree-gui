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

#include "sequencergui/operation/operation_action_handler.h"
#include "test_automation_server.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/automation_client.h>
#include <sequencergui/jobsystem/i_job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/jobsystem/remote_connection_service.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_job_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/operation/operation_action_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>

namespace sequencergui
{

namespace
{

const std::string kServerName = "OperationActionHandlerRemoteScenarioTestServer";
const size_t kJobIndex{0};

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
  static void SetUpTestSuite() { m_test_automation_server.Start(kServerName, kProcedureBodyText); }

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
    return GetJobModel()->GetRootItem()->GetItems<T>(mvvm::TagIndex::kDefaultTag);
  }

  ApplicationModels m_models;
  RemoteConnectionService m_remote_connection_service;
  UserContext m_user_context;
  JobManager m_job_manager;
  testutils::MockOperationActionContext m_mock_context;
  static testutils::TestAutomationServer m_test_automation_server;
};

testutils::TestAutomationServer OperationActionHandlerRemoteScenarioTest::m_test_automation_server;

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
  auto predicate = [this, job_item]()
  {
    auto status = job_item->GetStatus();
    return !status.empty() && GetRunnerStatus(status) == RunnerStatus::kInitial;
  };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 0};
  EXPECT_TRUE(testutils::IsEqual(*variables.at(0), expected_value));
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
  handler->OnImportRemoteJobRequest();

  // validating job model
  auto submitted_jobs = GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);

  // making item selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  handler->OnStartJobRequest();

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [this, job_item]()
  {
    auto status = job_item->GetStatus();
    return !status.empty() && GetRunnerStatus(status) == RunnerStatus::kSucceeded;
  };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 3};
  EXPECT_TRUE(testutils::IsEqual(*variables.at(0), expected_value));
}

}  // namespace sequencergui
