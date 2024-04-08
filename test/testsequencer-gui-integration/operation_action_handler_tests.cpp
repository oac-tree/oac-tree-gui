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
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>
#include <QTest>
#include <iostream>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

Q_DECLARE_METATYPE(sequencergui::JobItem*)

class OperationActionHandlerTest : public ::testing::Test
{
public:
  OperationActionHandlerTest() : m_actions(&m_job_manager, [this] { return m_selected_item; })
  {
    m_actions.SetJobModel(GetJobModel());
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  std::vector<JobItem*> GetJobItems() { return mvvm::utils::GetTopItems<JobItem>(GetJobModel()); }

  bool IsCompleted(JobItem* job_item)
  {
    auto status = job_item->GetStatus();
    return !status.empty() && GetRunnerStatus(status) == RunnerStatus::kSucceeded;
  }

  ApplicationModels m_models;
  JobManager m_job_manager;
  OperationActionHandler m_actions;
  JobItem* m_selected_item{nullptr};
};

TEST_F(OperationActionHandlerTest, AttemptToUseWhenMisconfigured)
{
  {
    OperationActionHandler actions(nullptr, {});
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }

  {
    OperationActionHandler actions(&m_job_manager, {});
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }

  {
    OperationActionHandler actions(&m_job_manager, [this] { return m_selected_item; });
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }
}

//! Submission of the procedure.

TEST_F(OperationActionHandlerTest, OnSubmitJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");
  procedure->SetDisplayName("procedure_display_name");

  QSignalSpy spy_selected_request(&m_actions, &OperationActionHandler::MakeJobSelectedRequest);

  m_actions.OnSetTickTimeoutRequest(42);

  EXPECT_FALSE(m_actions.OnSubmitJobRequest(nullptr));

  // At the beginning there is not JobItems in a modelo
  EXPECT_TRUE(GetJobItems().empty());

  // submitting the procedure
  EXPECT_TRUE(m_actions.OnSubmitJobRequest(procedure));

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);

  EXPECT_EQ(job_item->GetDisplayName(), "procedure_display_name");
  EXPECT_EQ(job_item->GetTickTimeout(), 42);

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  EXPECT_EQ(testutils::GetSendItem<JobItem*>(spy_selected_request), job_item);

  // we can submit same procedure twice, it will be two different jobs
  m_actions.OnSubmitJobRequest(procedure);
  ASSERT_EQ(GetJobItems().size(), 2);

  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(GetJobItems().at(0), GetJobItems().at(1));

  EXPECT_EQ(GetJobItems().at(0)->GetProcedure(), procedure);
  EXPECT_EQ(GetJobItems().at(1)->GetProcedure(), procedure);
}

//! Attempt to submit wronly configured procedure.

TEST_F(OperationActionHandlerTest, AttemptToSubmitMalformedProcedure)
{
  auto procedure = testutils::CreateInvalidProcedureItem(GetSequencerModel());

  JobManager manager;

  EXPECT_THROW(m_actions.OnSubmitJobRequest(procedure), sequencergui::RuntimeException);

  // After unsuccessfull submission JobItem remains there
  ASSERT_EQ(GetJobItems().size(), 1);
}

//! Submit the job, when start and wait till the end.

TEST_F(OperationActionHandlerTest, OnStartJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // sarting the job when now JobItem is selected
  m_actions.OnStartJobRequest();

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  // no item was selected, job_handler is not switched
  EXPECT_FALSE(m_job_manager.GetCurrentJobHandler());

  // making item selected
  m_selected_item = job_item;

  m_actions.OnStartJobRequest();

  EXPECT_EQ(m_job_manager.GetCurrentJobHandler(), m_job_manager.GetJobHandler(job_item));

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);
}

//! Removing submitted job.

TEST_F(OperationActionHandlerTest, OnRemoveJobRequest)
{
  auto procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());

  m_actions.OnSubmitJobRequest(procedure);
  EXPECT_EQ(GetJobItems().size(), 1);

  // if no selection provided, the command does nothing
  EXPECT_FALSE(m_actions.OnRemoveJobRequest());

  auto job_item = GetJobItems().at(0);
  m_selected_item = GetJobItems().at(0);

  EXPECT_TRUE(m_actions.OnRemoveJobRequest());
  EXPECT_TRUE(GetJobItems().empty());
}

//! Removing submitted job together with original procedure.

TEST_F(OperationActionHandlerTest, OnRemoveJobAndCleanupRequest)
{
  auto procedure = testutils::CreateCopyProcedureItem(GetSequencerModel());
  EXPECT_EQ(GetSequencerModel()->GetProcedureContainer()->GetSize(), 1);

  m_actions.OnSubmitJobRequest(procedure);
  EXPECT_EQ(GetJobItems().size(), 1);

  // if no selection provided, the command does nothing
  EXPECT_NO_THROW(m_actions.OnRemoveJobAndCleanupRequest());

  auto job_item = GetJobItems().at(0);
  m_selected_item = GetJobItems().at(0);

  EXPECT_NO_THROW(m_actions.OnRemoveJobAndCleanupRequest());
  EXPECT_TRUE(GetJobItems().empty());
  EXPECT_EQ(GetSequencerModel()->GetProcedureContainer()->GetSize(), 0);
}

//! Attempt to remove long running job.

TEST_F(OperationActionHandlerTest, AttemptToRemoveLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(GetSequencerModel(), msec(10000));

  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  m_selected_item = GetJobItems().at(0);

  m_actions.OnStartJobRequest();

  auto job_handler = m_job_manager.GetCurrentJobHandler();
  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return job_handler->IsRunning(); }, 50));
  EXPECT_TRUE(job_handler->IsRunning());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_THROW(m_actions.OnRemoveJobRequest(), sequencergui::RuntimeException);
  QTest::qWait(5);

  m_actions.OnStopJobRequest();
  QTest::qWait(5);

  EXPECT_TRUE(QTest::qWaitFor([job_handler]() { return !job_handler->IsRunning(); }, 50));

  EXPECT_FALSE(job_handler->IsRunning());
}

//! Regenerate submitted job.

TEST_F(OperationActionHandlerTest, OnRegenerateJobRequest)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  EXPECT_EQ(m_job_manager.GetJobHandler(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  job_item->SetStatus("abc");  // set arbitrary status

  QSignalSpy spy_selected_request(&m_actions, &OperationActionHandler::MakeJobSelectedRequest);

  // regenerating a job
  m_selected_item = job_item;
  m_actions.OnRegenerateJobRequest();

  // on regeneration status should be reset
  EXPECT_TRUE(job_item->GetStatus().empty());

  EXPECT_EQ(testutils::GetSendItem<JobItem*>(spy_selected_request), job_item);

  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but different job_handler
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(job_item->GetExpandedProcedure(), expanded_procedure);
}

//! Regenerate submitted job.

TEST_F(OperationActionHandlerTest, OnRegenerateJobRequestWhenProcedureDeleted)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto job_handler = m_job_manager.GetJobHandler(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  // deleting procedure
  GetSequencerModel()->RemoveItem(procedure);

  QSignalSpy spy_selected_request(&m_actions, &OperationActionHandler::MakeJobSelectedRequest);

  // regenerating a job
  m_selected_item = job_item;
  EXPECT_THROW(m_actions.OnRegenerateJobRequest(), sequencergui::RuntimeException);

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

TEST_F(OperationActionHandlerTest, ExecuteSameJobTwice)
{
  auto procedure = testutils::CreateMessageProcedureItem(GetSequencerModel(), "text");

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetJobHandler(job_item));

  // sarting the job when now JobItem is selected
  m_selected_item = job_item;

  m_actions.OnStartJobRequest();

  EXPECT_EQ(m_job_manager.GetCurrentJobHandler(), m_job_manager.GetJobHandler(job_item));

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));

  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());
  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);

  // starting same job again
  m_actions.OnStartJobRequest();

  EXPECT_TRUE(QTest::qWaitFor([this, job_item]() { return IsCompleted(job_item); }, 50));
  EXPECT_FALSE(m_job_manager.GetJobHandler(job_item)->IsRunning());

  EXPECT_EQ(GetRunnerStatus(job_item->GetStatus()), RunnerStatus::kSucceeded);
}
