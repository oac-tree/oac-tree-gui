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

#include "sequencergui/jobsystem/domainrunneradapter.h"

#include "Procedure.h"
#include "Runner.h"
#include "mockrunnerlistener.h"
#include "mocksequencerobserver.h"
#include "test_procedures.h"

#include <gtest/gtest.h>

#include <memory>
#include <thread>

using namespace sequencergui;

using msec = std::chrono::milliseconds;
using ::testing::_;

class DomainRunnerAdapterTest : public ::testing::Test
{
public:
  std::unique_ptr<runner_t> CreateRunner(procedure_t* procedure)
  {
    auto result = std::make_unique<runner_t>(&m_observer);
    procedure->Setup();
    result->SetProcedure(procedure);
    return result;
  }

  testutils::MockSequencerObserver m_observer;
  testutils::MockRunnerListener m_listener;
};

TEST_F(DomainRunnerAdapterTest, ShortProcedureThatExecutesNormally)
{
  auto procedure = testutils::CreateSingleWaitProcedure(10);
  auto runner = CreateRunner(procedure.get());

  DomainRunnerAdapter adapter(std::move(runner), m_listener.CreateCallback());
  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kIdle);
  EXPECT_TRUE(procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(2);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter.Start());

  EXPECT_TRUE(adapter.WaitForCompletion(0.1));
  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_TRUE(procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::SUCCESS);
}
