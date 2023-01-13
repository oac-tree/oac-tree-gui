/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/sequencer_monitor_actions.h"

#include <gtest/gtest.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_context.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <testutils/standard_procedure_items.h>

#include <mvvm/model/model_utils.h>

#include <QSignalSpy>
#include <QTest>
#include <iostream>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

Q_DECLARE_METATYPE(sequencergui::JobItem*)

class SequencerMonitorActionsTests : public ::testing::Test
{
public:
  SequencerMonitorActionsTests() : m_actions(&m_job_manager, [this] { return m_selected_item; })
  {
    m_actions.SetJobModel(GetJobModel());
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  std::vector<JobItem*> GetJobItems() { return mvvm::utils::GetTopItems<JobItem>(GetJobModel()); }

  ApplicationModels m_models;
  JobManager m_job_manager;
  SequencerMonitorActions m_actions;
  JobItem* m_selected_item{nullptr};
};

TEST_F(SequencerMonitorActionsTests, AttemptToUseWhenMisconfigured)
{
  {
    SequencerMonitorActions actions(nullptr, {});
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }

  {
    SequencerMonitorActions actions(&m_job_manager, {});
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }

  {
    SequencerMonitorActions actions(&m_job_manager, [this] { return m_selected_item; });
    EXPECT_THROW(actions.OnStartJobRequest(), RuntimeException);
  }
}

//! Submission of the procedure.

TEST_F(SequencerMonitorActionsTests, OnSubmitJobRequest)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10));

  QSignalSpy spy_selected_request(&m_actions, &SequencerMonitorActions::MakeJobSelectedRequest);

  EXPECT_NO_FATAL_FAILURE(m_actions.OnSubmitJobRequest(nullptr));

  // At the beginning there is not JobItems in a modelo
  EXPECT_TRUE(GetJobItems().empty());

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_EQ(m_job_manager.GetContext(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  EXPECT_EQ(spy_selected_request.count(), 1);
  auto arguments = spy_selected_request.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  EXPECT_EQ(arguments.at(0).value<JobItem*>(), job_item);

  // we can submit same procedure twice, it will be two different jobs
  m_actions.OnSubmitJobRequest(procedure);
  ASSERT_EQ(GetJobItems().size(), 2);

  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(GetJobItems().at(0), GetJobItems().at(1));

  EXPECT_EQ(GetJobItems().at(0)->GetProcedure(), procedure);
  EXPECT_EQ(GetJobItems().at(1)->GetProcedure(), procedure);
}

//! Attempt to submit wronly configured procedure.

TEST_F(SequencerMonitorActionsTests, AttemptToSubmitMalformedProcedure)
{
  auto procedure = testutils::CreateInvalidProcedure(GetSequencerModel());

  JobManager manager;

  EXPECT_THROW(m_actions.OnSubmitJobRequest(procedure), sequencergui::RuntimeException);

  // After unsuccessfull submission JobItem remains there
  ASSERT_EQ(GetJobItems().size(), 1);
}

//! Submit the job, when start and wait till the end.

TEST_F(SequencerMonitorActionsTests, OnStartJobRequest)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(20));

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetContext(job_item));

  // sarting the job when now JobItem is selected
  m_actions.OnStartJobRequest();

  EXPECT_FALSE(m_job_manager.GetContext(job_item)->IsRunning());
  EXPECT_FALSE(m_job_manager.GetCurrentContext());  // no item was selected, context is not switched

  // making item selected
  m_selected_item = job_item;

  m_actions.OnStartJobRequest();

  EXPECT_EQ(m_job_manager.GetCurrentContext(), m_job_manager.GetContext(job_item));
  EXPECT_TRUE(m_job_manager.GetContext(job_item)->IsRunning());

  QTest::qWait(100);
  EXPECT_FALSE(m_job_manager.GetContext(job_item)->IsRunning());

  EXPECT_EQ(job_item->GetStatus(), std::string("Completed"));
}

//! Removing submitted job.

TEST_F(SequencerMonitorActionsTests, OnRemoveJobRequest)
{
  auto procedure = testutils::CreateCopyProcedure(GetSequencerModel());

  m_actions.OnSubmitJobRequest(procedure);
  EXPECT_EQ(GetJobItems().size(), 1);

  // if no selection provided, the command does nothing
  EXPECT_NO_THROW(m_actions.OnRemoveJobRequest());

  auto job_item = GetJobItems().at(0);
  m_selected_item = GetJobItems().at(0);

  EXPECT_NO_THROW(m_actions.OnRemoveJobRequest());
  EXPECT_TRUE(GetJobItems().empty());
}

//! Attempt to remove long running job.

TEST_F(SequencerMonitorActionsTests, AttemptToRemoveLongRunningJob)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10000));

  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  m_selected_item = GetJobItems().at(0);

  m_actions.OnStartJobRequest();

  auto context = m_job_manager.GetCurrentContext();
  EXPECT_TRUE(context->IsRunning());

  // it shouldn't be possible to remove running job without first stopping it
  EXPECT_THROW(m_actions.OnRemoveJobRequest(), RuntimeException);
  QTest::qWait(20);

  m_actions.OnStopJobRequest();
  QTest::qWait(20);

  EXPECT_FALSE(context->IsRunning());
}

//! Regenerate submitted job.

TEST_F(SequencerMonitorActionsTests, OnRegenerateJobRequest)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10));

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto context = m_job_manager.GetContext(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  EXPECT_EQ(m_job_manager.GetContext(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  QSignalSpy spy_selected_request(&m_actions, &SequencerMonitorActions::MakeJobSelectedRequest);

  // regenerating a job
  m_selected_item = job_item;
  m_actions.OnRegenerateJobRequest();

  EXPECT_EQ(spy_selected_request.count(), 1);
  auto arguments = spy_selected_request.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  EXPECT_EQ(arguments.at(0).value<JobItem*>(), job_item);

  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but different contexts
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(m_job_manager.GetContext(job_item), context);
  EXPECT_NE(job_item->GetExpandedProcedure(), expanded_procedure);
}

//! Regenerate submitted job.

TEST_F(SequencerMonitorActionsTests, OnRegenerateJobRequestWhenProcedureDeleted)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10));

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  auto context = m_job_manager.GetContext(job_item);
  auto expanded_procedure = job_item->GetExpandedProcedure();

  // deleting procedure
  GetSequencerModel()->RemoveItem(procedure);

  QSignalSpy spy_selected_request(&m_actions, &SequencerMonitorActions::MakeJobSelectedRequest);

  // regenerating a job
  m_selected_item = job_item;
  EXPECT_THROW(m_actions.OnRegenerateJobRequest(), RuntimeException);

  // job item has lost it's procedure and expanded procedure
  EXPECT_FALSE(job_item->GetProcedure());
  EXPECT_FALSE(job_item->GetExpandedProcedure());

  EXPECT_EQ(spy_selected_request.count(), 0);
  ASSERT_EQ(GetJobItems().size(), 1);

  // it should be same JobItem, but no context
  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_FALSE(m_job_manager.GetContext(job_item));
}

//! Consequent execution of same job.

TEST_F(SequencerMonitorActionsTests, ExecuteSameJobTwice)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(50));

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_TRUE(m_job_manager.GetContext(job_item));

  // sarting the job when now JobItem is selected
  m_selected_item = job_item;

  m_actions.OnStartJobRequest();

  EXPECT_EQ(m_job_manager.GetCurrentContext(), m_job_manager.GetContext(job_item));
  EXPECT_TRUE(m_job_manager.GetContext(job_item)->IsRunning());

  QTest::qWait(100);
  EXPECT_FALSE(m_job_manager.GetContext(job_item)->IsRunning());

  EXPECT_EQ(job_item->GetStatus(), std::string("Completed"));

  // starting same job again
  m_actions.OnStartJobRequest();

  QTest::qWait(100);
  EXPECT_FALSE(m_job_manager.GetContext(job_item)->IsRunning());

  EXPECT_EQ(job_item->GetStatus(), std::string("Completed"));
}
