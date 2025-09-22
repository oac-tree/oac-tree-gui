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

#include "oac_tree_gui/operation/objects/operation_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/job_utils.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(oac_tree_gui::JobItem*)

namespace oac_tree_gui
{

using msec = std::chrono::milliseconds;

/**
 * @brief Tests of OperationActionHandler class by submitting and/or running actual sequencer
 * procedures.
 */
class OperationActionHandlerExtendedTest : public test::FolderTest
{
public:
  OperationActionHandlerExtendedTest()
      : FolderTest("OperationActionHandlerExtendedTest")
      , m_job_manager(GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service))
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
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

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  std::vector<JobItem*> GetJobItems() { return mvvm::utils::GetTopItems<JobItem>(*GetJobModel()); }

  static bool IsCompleted(JobItem* job_item)
  {
    return job_item->GetStatus() == RunnerStatus::kSucceeded;
  }

  ApplicationModels m_models;
  UserContext m_user_context;
  JobManager m_job_manager;

  test::MockOperationActionContext m_mock_context;
  test::MockRemoteConnectionService m_mock_connection_service;
};

TEST_F(OperationActionHandlerExtendedTest, AttemptToUseWhenMisconfigured)
{
  EXPECT_THROW(OperationActionHandler(nullptr, m_mock_context.CreateContext()), RuntimeException);
  EXPECT_THROW(OperationActionHandler(&m_job_manager, {}), RuntimeException);
}

//! Submission of the procedure.
TEST_F(OperationActionHandlerExtendedTest, OnSubmitJobRequest)
{
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");
  procedure->SetDisplayName("procedure_display_name");

  auto handler = CreateOperationHandler();

  QSignalSpy spy_selected_request(handler.get(), &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnSetTickTimeoutRequest(42);

  EXPECT_FALSE(handler->SubmitLocalJob(nullptr));

  // At the beginning there is not JobItems in a modelo
  EXPECT_TRUE(GetJobItems().empty());

  // submitting the procedure
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_TRUE(handler->SubmitLocalJob(procedure));

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
  handler->SubmitLocalJob(procedure);
  ASSERT_EQ(GetJobItems().size(), 2);

  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(GetJobItems().at(0), GetJobItems().at(1));

  EXPECT_EQ(GetJobItems().at(0)->GetProcedure(), procedure);
  EXPECT_EQ(GetJobItems().at(1)->GetProcedure(), procedure);
}

//! Attempt to submit wronly configured procedure.
TEST_F(OperationActionHandlerExtendedTest, AttemptToSubmitMalformedProcedure)
{
  auto procedure = test::CreateInvalidProcedureItem(GetSequencerModel());

  const JobManager manager(GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service));
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_FALSE(handler->SubmitLocalJob(procedure));

  // After unsuccessfull submission JobItem remains there
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kSubmitFailure);
}

//! Submit the job, when start and wait till the end.
TEST_F(OperationActionHandlerExtendedTest, OnStartJobRequest)
{
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");
  auto handler = CreateOperationHandler();

  // submitting the procedure
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // starting the job when no JobItem is selected
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  // making item selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 200));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kSucceeded);

  // starting second time, jobHandler should be the same as before
  auto prev_job_handler = m_job_manager.GetJobHandler(job_item);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();
  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 200));

  EXPECT_TRUE(QTest::qWaitFor(
      [this, job_item]() { return !m_job_manager.GetJobHandler(job_item)->IsRunning(); }, 200));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kSucceeded);

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item), prev_job_handler);
}

//! Removing submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRemoveJobRequest)
{
  auto procedure = test::CreateCopyProcedureItem(GetSequencerModel());
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);
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

//! Attempt to remove long running job.
TEST_F(OperationActionHandlerExtendedTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = test::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  auto job_handler = m_job_manager.GetJobHandler(job_item);
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return job_handler->IsRunning(); }, 100));
  EXPECT_TRUE(job_handler->IsRunning());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  EXPECT_FALSE(handler->OnRemoveJobRequest());
  QTest::qWait(10);

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStopJobRequest();
  QTest::qWait(10);

  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return !job_handler->IsRunning(); }, 100));

  EXPECT_FALSE(job_handler->IsRunning());
}

//! Regenerate submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRegenerateJobRequest)
{
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  job_item->SetStatus(RunnerStatus::kHalted);  // set arbitrary status

  QSignalSpy spy_selected_request(handler.get(), &OperationActionHandler::MakeJobSelectedRequest);

  // regenerating a job
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnRegenerateJobRequest();

  // on regeneration status should be reset
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kUndefined);

  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);

  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but different job_handler
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(job_item->GetExpandedProcedure(), expanded_procedure);
}

//! Regenerate submitted job.
TEST_F(OperationActionHandlerExtendedTest, OnRegenerateJobRequestWhenProcedureDeleted)
{
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  // deleting procedure
  GetSequencerModel()->RemoveItem(procedure);

  const QSignalSpy spy_selected_request(handler.get(),
                                        &OperationActionHandler::MakeJobSelectedRequest);

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
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->SubmitLocalJob(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // starting the job when now JobItem is selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  // check that JobItem got correct state via queued connection
  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 100));
  // check that JobManager sees correct state from async runner
  EXPECT_TRUE(QTest::qWaitFor(
      [this, job_item]() { return !m_job_manager.GetJobHandler(job_item)->IsRunning(); }, 100));

  QTest::qWait(20);

  // starting same job again
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  // check that JobItem got correct state via queued connection
  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 100));
  // check that JobManager sees correct state from async runner
  EXPECT_TRUE(QTest::qWaitFor(
      [this, job_item]() { return !m_job_manager.GetJobHandler(job_item)->IsRunning(); }, 100));
}

//! Submit file based job. First submit a file with the error in XML. Then make a fix of XML, and
//! resubmit again. Finally run selected job.
TEST_F(OperationActionHandlerExtendedTest, SubmitMalFormedFileBasedJobThenResubmit)
{
  const std::string body_with_xml_error{R"(
  <Message text="Hello"
)"};

  // writing procedure in a file
  const auto file_name = GetFilePath("ProcedureWithFixedError.xml");
  mvvm::test::CreateTextFile(file_name, test::CreateProcedureString(body_with_xml_error));

  auto handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  handler->SubmitFileBasedJob(file_name);

  // After unsuccessfull submission JobItem remains there
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kSubmitFailure);

  // Fixing XML file (pretending user has properly edited it)
  const std::string body_with_xml_fixed{R"(
  <Message text="Hello" />
)"};
  mvvm::test::CreateTextFile(file_name, test::CreateProcedureString(body_with_xml_fixed));

  // making item selected
  ON_CALL(m_mock_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  // resubmitting a job, it will use same file name, but content will be different
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnRegenerateJobRequest();
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kUndefined);  // not a failure anymore

  // sarting the job
  EXPECT_CALL(m_mock_context, OnSelectedJob()).Times(1);
  handler->OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 100));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(job_item->GetStatus(), RunnerStatus::kSucceeded);
}

}  // namespace oac_tree_gui
