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

#include "oac_tree_gui/jobsystem/user_context.h"
#include "oac_tree_gui/operation/objects/operation_action_handler.h"
#include "test_automation_server.h"

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

const std::size_t kJobIndex{0};

/**
 * @brief Transport tag types used to parameterize the scenario tests over the automation server
 * protocol.
 *
 * Each tag exposes the AutomationServerInfo describing the server to run for that transport. Using
 * types (rather than values) lets the templated fixture start/stop a dedicated server once per
 * transport in SetUpTestSuite()/TearDownTestSuite().
 */
struct EpicsTransport
{
  static AutomationServerInfo GetServerInfo()
  {
    return EPICSServerInfo{"OperationActionHandlerRemoteScenarioEPICSTestServer"};
  }
};

struct WebSocketsTransport
{
  static AutomationServerInfo GetServerInfo()
  {
    return WebSocketsServerInfo{"localhost", 21777u};
  }
};

using TransportTypes = ::testing::Types<EpicsTransport, WebSocketsTransport>;

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
 * @brief Tests for OperationActionHandler class in his full glory to run remote jobs based on
 * automation server.
 *
 * The suite is parameterized over the transport (EPICS, WebSockets) via typed tests, so each
 * transport runs its own automation server, started and stopped once per suite.
 */
template <typename TransportT>
class OperationActionHandlerRemoteScenarioTest : public ::testing::Test
{
public:
  OperationActionHandlerRemoteScenarioTest()
      : m_remote_connection_service(GetAutomationClientFactoryFunc(), {})
      , m_job_manager(GetJobHandlerFactoryFunc(m_user_context, m_remote_connection_service))
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
  }

  static AutomationServerInfo GetServerInfo() { return TransportT::GetServerInfo(); }

  /**
   * @brief Runs remote server with single procedure on board.
   */
  static void SetUpTestSuite()
  {
    m_test_automation_server.Start(GetServerInfo(), kProcedureBodyText);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  static void TearDownTestSuite() { m_test_automation_server.Stop(); }

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
    return GetJobModel()->GetRootItem()->template GetItems<T>(mvvm::TagIndex::GetDefaultTag());
  }

  ApplicationModels m_models;
  RemoteConnectionService m_remote_connection_service;
  UserContext m_user_context;
  JobManager m_job_manager;
  test::MockOperationActionContext m_mock_context;
  static test::TestAutomationServer m_test_automation_server;
};

template <typename TransportT>
test::TestAutomationServer
    OperationActionHandlerRemoteScenarioTest<TransportT>::m_test_automation_server;

TYPED_TEST_SUITE(OperationActionHandlerRemoteScenarioTest, TransportTypes);

TYPED_TEST(OperationActionHandlerRemoteScenarioTest, InitialState)
{
  auto handler = this->CreateOperationHandler();
  EXPECT_EQ(this->m_job_manager.GetJobCount(), 0);
  EXPECT_TRUE(this->m_remote_connection_service.GetServerInfos().empty());
}

TYPED_TEST(OperationActionHandlerRemoteScenarioTest, OnImportRemoteJobRequest)
{
  auto handler = this->CreateOperationHandler();

  // setup remote dialog to return remote job info
  // setting up context, so it report "user choice" related to remote job import
  const RemoteConnectionInfo connection_context{TestFixture::GetServerInfo(), {kJobIndex}};
  ON_CALL(this->m_mock_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));

  // submit job
  EXPECT_CALL(this->m_mock_context, OnGetRemoteConnectionInfo());
  EXPECT_CALL(this->m_mock_context, OnSelectedJob());
  handler->OnImportRemoteJobRequest();

  // validating job model
  auto submitted_jobs = this->template GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);
  EXPECT_EQ(job_item->GetAutomationServerInfo(), TestFixture::GetServerInfo());
  EXPECT_EQ(job_item->GetRemoteJobIndex(), kJobIndex);

  // validating initial state of RemoteJobHandler
  EXPECT_EQ(this->m_job_manager.GetJobCount(), 1);
  EXPECT_EQ(this->m_job_manager.GetJobItems(), std::vector<JobItem*>({job_item}));
  auto job_handler = this->m_job_manager.GetJobHandler(job_item);
  ASSERT_NE(job_handler, nullptr);
  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_EQ(job_handler->GetRunnerStatus(), RunnerStatus::kInitial);

  // validating that connection service has established client connection
  EXPECT_TRUE(this->m_remote_connection_service.HasClient(TestFixture::GetServerInfo()));

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [job_item]() { return job_item->GetStatus() == RunnerStatus::kInitial; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 5000));

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 42};

  auto predicate2 = [&expected_value, &variables]()
  { return test::IsEqual(*variables.at(0), expected_value); };
  EXPECT_TRUE(QTest::qWaitFor(predicate2, 5000));

  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

TYPED_TEST(OperationActionHandlerRemoteScenarioTest, ImportRemoteJobAndStart)
{
  auto handler = this->CreateOperationHandler();

  // setup remote dialog to return remote job info
  // setting up context, so it report "user choice" related to remote job import
  const RemoteConnectionInfo connection_context{TestFixture::GetServerInfo(), {kJobIndex}};
  ON_CALL(this->m_mock_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));

  // submit job
  EXPECT_CALL(this->m_mock_context, OnGetRemoteConnectionInfo());
  EXPECT_CALL(this->m_mock_context, OnSelectedJob());
  handler->OnImportRemoteJobRequest();

  // validating job model
  auto submitted_jobs = this->template GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);

  handler->OnStartJobRequest(job_item);

  // after queued connection processed all event, JobItem should get its status
  auto predicate = [job_item]() { return job_item->GetStatus() == RunnerStatus::kSucceeded; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 500));

  EXPECT_FALSE(this->m_job_manager.GetJobHandler(job_item)->IsRunning());

  // validating internal expanded ProcedureItem
  auto procedure_item = job_item->GetExpandedProcedure();
  ASSERT_NE(procedure_item, nullptr);
  auto variables = job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);
  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 45};

  auto predicate2 = [&expected_value, &variables]()
  { return test::IsEqual(*variables.at(0), expected_value); };
  EXPECT_TRUE(QTest::qWaitFor(predicate2, 5000));

  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));
}

//! Removing the last remote job for a server drops the corresponding remote connection.
TYPED_TEST(OperationActionHandlerRemoteScenarioTest, RemoveRemoteJobDropsConnection)
{
  auto handler = this->CreateOperationHandler();

  // the drop_remote_connection callback forwards to the real connection service
  ON_CALL(this->m_mock_context, OnDropRemoteConnection(::testing::_))
      .WillByDefault([this](const AutomationServerInfo& info)
                     { this->m_remote_connection_service.Disconnect(info); });

  // submit a remote job
  const RemoteConnectionInfo connection_context{TestFixture::GetServerInfo(), {kJobIndex}};
  ON_CALL(this->m_mock_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));
  EXPECT_CALL(this->m_mock_context, OnGetRemoteConnectionInfo()).Times(1);
  EXPECT_CALL(this->m_mock_context, OnSelectedJob()).Times(1);

  handler->OnImportRemoteJobRequest();

  auto submitted_jobs = this->template GetJobs<RemoteJobItem>();
  ASSERT_EQ(submitted_jobs.size(), 1);
  auto job_item = submitted_jobs.at(0);

  EXPECT_TRUE(this->m_remote_connection_service.HasClient(TestFixture::GetServerInfo()));

  // remove the job; as it is the last one using the server, the connection must be dropped
  ON_CALL(this->m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));
  EXPECT_CALL(this->m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_CALL(this->m_mock_context, OnDropRemoteConnection(TestFixture::GetServerInfo())).Times(1);

  EXPECT_TRUE(handler->OnRemoveJobRequest());

  EXPECT_TRUE(this->template GetJobs<RemoteJobItem>().empty());
  EXPECT_FALSE(this->m_remote_connection_service.HasClient(TestFixture::GetServerInfo()));
}

}  // namespace oac_tree_gui
