/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/core/exceptions.h"
#include "sequencergui/jobsystem/job_context.h"
#include "sequencergui/jobsystem/job_manager.h"
#include "sequencergui/model/application_models.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"
#include "sequencergui/monitor/sequencer_monitor_actions.h"
#include "test_procedure_items.h"

#include <gtest/gtest.h>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

class SequencerMonitorActionsTests : public ::testing::Test
{
public:
  SequencerMonitorActionsTests() : m_actions(&m_job_manager, [this] { return m_selected_item; })
  {
    m_actions.SetJobModel(GetJobModel());
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  std::vector<JobItem*> GetJobItems() { return GetJobModel()->GetTopItems<JobItem>(); }

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

TEST_F(SequencerMonitorActionsTests, OnSubmitJobRequest)
{
  auto procedure = testutils::CreateSingleWaitProcedure(GetSequencerModel(), msec(10));

  EXPECT_NO_FATAL_FAILURE(m_actions.OnSubmitJobRequest(nullptr));

  // At the beginning there is not JobItems in a modelo
  EXPECT_TRUE(GetJobItems().empty());

  // submitting the procedure
  m_actions.OnSubmitJobRequest(procedure);

  // successfull job submlission leads to the creation of JobItem with expanded procedure
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);
  EXPECT_EQ(m_job_manager.GetContext(job_item)->GetExpandedProcedure(),
            job_item->GetExpandedProcedure());
  EXPECT_EQ(job_item->GetProcedure(), procedure);

  // we can submit same procedure twice, it will be two different jobs
  m_actions.OnSubmitJobRequest(procedure);
  ASSERT_EQ(GetJobItems().size(), 2);

  EXPECT_EQ(GetJobItems().at(0), job_item);
  EXPECT_NE(GetJobItems().at(0), GetJobItems().at(1));

  EXPECT_EQ(GetJobItems().at(0)->GetProcedure(), procedure);
  EXPECT_EQ(GetJobItems().at(1)->GetProcedure(), procedure);
}
