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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/local_job_handler.h>
#include <sequencergui/jobsystem/user_context.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/operation/operation_action_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(sequencergui::JobItem*)

namespace sequencergui
{

using msec = std::chrono::milliseconds;

class OperationActionHandlerExtendedTest : public ::testing::Test
{
public:
  OperationActionHandlerExtendedTest() : m_job_manager(CreateJobManagerContext())
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
  }

  /**
   * @brief Creates context necessary for JobManager to funciton.
   */
  JobManager::create_handler_func_t CreateJobManagerContext()
  {
    return CreateJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  }

  /**
   * @brief Creates operation action handler.
   */
  std::unique_ptr<OperationActionHandler> CreateOperationHandler()
  {
    auto result =
        std::make_unique<OperationActionHandler>(&m_job_manager, m_mock_context.CreateContext());
    result->SetJobModel(GetJobModel());
    return result;
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  std::vector<JobItem*> GetJobItems() { return mvvm::utils::GetTopItems<JobItem>(GetJobModel()); }

  static bool IsCompleted(JobItem* job_item)
  {
    auto status = job_item->GetStatus();
    return !status.empty() && GetRunnerStatus(status) == RunnerStatus::kSucceeded;
  }

  ApplicationModels m_models;
  JobManager m_job_manager;

  testutils::MockOperationActionContext m_mock_context;
  testutils::MockRemoteConnectionService m_mock_connection_service;
  UserContext m_user_context;
};

TEST_F(OperationActionHandlerExtendedTest, AttemptToUseWhenMisconfigured)
{
  EXPECT_THROW(OperationActionHandler(nullptr, m_mock_context.CreateContext()), RuntimeException);
  EXPECT_THROW(OperationActionHandler(&m_job_manager, {}), RuntimeException);

  OperationActionHandler actions(&m_job_manager, m_mock_context.CreateContext());
  EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
}

//! Submission of the procedure.
TEST_F(OperationActionHandlerExtendedTest, OnSubmitJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");
  procedure->SetDisplayName("procedure_display_name");

  auto handler = CreateOperationHandler();

  QSignalSpy spy_selected_request(handler.get(), &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSetTickTimeoutRequest(42);

  EXPECT_FALSE(handler->OnSubmitJobRequest(nullptr));

  // At the beginning there is not JobItems in a modelo
  EXPECT_TRUE(GetJobItems().empty());

  // submitting the procedure
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_TRUE(handler->OnSubmitJobRequest(procedure));

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);

  EXPECT_EQ(job_item->GetDisplayName(), "procedure_display_name");
  EXPECT_EQ(job_item->GetTickTimeout(), 42);

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);

  // we can submit same procedure twice, it will be two different jobs
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);
  ASSERT_EQ(GetJobItems().size(), 2);

  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(GetJobItems().at(0), GetJobItems().at(1));

  EXPECT_EQ(GetJobItems().at(0)->GetProcedure(), procedure);
  EXPECT_EQ(GetJobItems().at(1)->GetProcedure(), procedure);
}

//! Attempt to submit wronly configured procedure.
TEST_F(OperationActionHandlerExtendedTest, AttemptToSubmitMalformedProcedure)
{
  auto procedure = testutils::CreateInvalidProcedureItem(GetSequencerModel());

  const JobManager manager(CreateJobManagerContext());
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_FALSE(handler->OnSubmitJobRequest(procedure));

  // After unsuccessfull submission JobItem remains there
  ASSERT_EQ(GetJobItems().size(), 1);
}

//! Submit the job, when start and wait till the end.
TEST_F(OperationActionHandlerExtendedTest, OnStartJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");
  auto handler = CreateOperationHandler();

  // submitting the procedure
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // sarting the job when no JobItem is selected
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  // making item selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);

  // starting second time, jobHandler should be the same as before
  auto prev_job_handler = m_job_manager.GetJobHandler(job_item);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();
  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item), prev_job_handler);
}

//! Removing submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRemoveJobRequest)
{
  auto procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);
  EXPECT_EQ(GetJobItems().size(), 1);

  // if no selection provided, the command does nothing
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_FALSE(handler->OnRemoveJobRequest());

  // making item selected
  auto job_item = GetJobItems().at(0);
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_TRUE(handler->OnRemoveJobRequest());
  EXPECT_TRUE(GetJobItems().empty());
}

//! Removing submitted job together with original procedure.
TEST_F(OperationActionHandlerExtendedTest, OnRemoveJobAndCleanupRequest)
{
  auto procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());
  EXPECT_EQ(GetSequencerModel()->GetProcedureContainer()->GetSize(), 1);

  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);
  EXPECT_EQ(GetJobItems().size(), 1);

  // if no selection provided, the command does nothing
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_NO_THROW(handler->OnRemoveJobRequest(/*clean-up*/ true));

  auto job_item = GetJobItems().at(0);
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_NO_THROW(handler->OnRemoveJobRequest(/*clean-up*/ true));
  EXPECT_TRUE(GetJobItems().empty());
  EXPECT_EQ(GetSequencerModel()->GetProcedureContainer()->GetSize(), 0);
}

//! Attempt to remove long running job.
TEST_F(OperationActionHandlerExtendedTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  auto job_handler = m_job_manager.GetJobHandler(job_item);
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return job_handler->IsRunning(); }, 50));
  EXPECT_TRUE(job_handler->IsRunning());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  EXPECT_FALSE(handler->OnRemoveJobRequest());
  QTest::qWait(5);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStopJobRequest();
  QTest::qWait(5);

  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return !job_handler->IsRunning(); }, 50));

  EXPECT_FALSE(job_handler->IsRunning());
}

//! Regenerate submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRegenerateJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  job_item->SetStatus("abc");  // set arbitrary status

  QSignalSpy spy_selected_request(handler.get(), &OperationActionHandler::MakeJobSelectedRequest);

  // regenerating a job
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnRegenerateJobRequest();

  // on regeneration status should be reset
  EXPECT_TRUE(job_item->GetStatus().empty());

  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);

  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but different job_handler
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(job_item->GetExpandedProcedure(), expanded_procedure);
}

//! Regenerate submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRegenerateJobRequestWhenProcedureDeleted)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  // deleting procedure
  GetSequencerModel()->RemoveItem(procedure);

  QSignalSpy spy_selected_request(handler.get(), &OperationActionHandler::MakeJobSelectedRequest);

  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  // there will be throw, since original procedure was deleted and regeneration is impossible
  EXPECT_FALSE(handler->OnRegenerateJobRequest());

  // job item has lost it's procedure
  EXPECT_FALSE(job_item->GetProcedure());

  // expanded procedure is still the same
  EXPECT_EQ(job_item->GetExpandedProcedure(), expanded_procedure);

  EXPECT_EQ(spy_selected_request.count(), 0);
  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but no job_handler
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item));
}

//! Consequent execution of same job.
TEST_F(OperationActionHandlerExtendedTest, ExecuteSameJobTwice)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // sarting the job when now JobItem is selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);

  // starting same job again
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));
  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);
}

}  // namespace sequencergui
