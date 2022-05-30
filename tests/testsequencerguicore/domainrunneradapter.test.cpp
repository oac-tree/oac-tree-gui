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

#include <chrono>
#include <memory>
#include <thread>

namespace
{
const auto duration = [](auto time_interval)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(time_interval).count(); };

//! hard-coded value in Wait instruction
const std::chrono::milliseconds kDefaultWaitPrecision(50);
}  // namespace

using namespace sequencergui;
using ::testing::_;

class DomainRunnerAdapterTest : public ::testing::Test
{
public:
  using clock_used = std::chrono::high_resolution_clock;
  using time_t = std::chrono::time_point<clock_used>;
  using duration_unit = std::chrono::milliseconds;
  using msec = std::chrono::milliseconds;

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

//! Single wait procedure started in normal way. Waiting upon completion.

TEST_F(DomainRunnerAdapterTest, ShortProcedureThatExecutesNormally)
{
  const int timeout_msec(10);
  auto procedure = testutils::CreateSingleWaitProcedure(timeout_msec);
  auto runner = CreateRunner(procedure.get());

  DomainRunnerAdapter adapter(std::move(runner), m_listener.CreateCallback());
  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status change
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

  EXPECT_TRUE(adapter.WaitForCompletion(kDefaultWaitPrecision + msec(timeout_msec)));
  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Sequence with single wait.

TEST_F(DomainRunnerAdapterTest, SequenceWithSingleWait)
{
  const int timeout_msec(10);

  auto procedure = testutils::CreateSequenceWithWaitProcedure(timeout_msec);
  auto runner = CreateRunner(procedure.get());

  DomainRunnerAdapter adapter(std::move(runner), m_listener.CreateCallback());
  adapter.SetTickTimeout(1000);

  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(4);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  time_t start_time = clock_used::now();
  EXPECT_TRUE(adapter.Start());

  EXPECT_TRUE(adapter.WaitForCompletion(kDefaultWaitPrecision + msec(timeout_msec)));

  EXPECT_EQ(adapter.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  time_t end_time = clock_used::now();

  // here we test that adapter.SetTickTimeout(1000) doesn't influence execution time,
  // since we have only one child that gets executed during single step
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              < msec(kDefaultWaitPrecision * 2));
}
