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

#include "sequencergui/experimental/domain_runner.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/experimental/domain_event_dispatcher.h>
#include <sequencergui/experimental/domain_events.h>

#include <mvvm/test/mock_callback_listener.h>

#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>

#include <QTest>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainRunner class.

class DomainRunnerTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper method to create a context necessary for DomainEventDispatcher to function.
   */
  experimental::DomainEventDispatcherContext CreateContext()
  {
    return {
        {}, m_instruction_status_listener.CreateCallback(), m_job_status_listener.CreateCallback()};
  }

  mvvm::test::MockCallbackListener<experimental::InstructionStatusChanged>
      m_instruction_status_listener;
  mvvm::test::MockCallbackListener<experimental::JobStatusChanged> m_job_status_listener;
};

TEST_F(DomainRunnerTest, InitialState)
{
  EXPECT_THROW(experimental::DomainRunner(CreateContext(), nullptr), RuntimeException);

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  auto procedure = testutils::CreateMessageProcedure("text");
  experimental::DomainRunner runner(CreateContext(), procedure.get());
}

//! Running short domain procedure that executes normally. No callbacks in the direction of the GUI
//! is set.

TEST_F(DomainRunnerTest, ShortProcedureThatExecutesNormally)
{
  // no callbacks to report changes
  auto empty_context = experimental::DomainEventDispatcherContext{};

  auto procedure = testutils::CreateMessageProcedure("text");
  experimental::DomainRunner runner(empty_context, procedure.get());

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  runner.Start();

  auto final_state = runner.WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
}

//! Running short domain procedure that executes normally. Callbacks to report to the GUI is set.

TEST_F(DomainRunnerTest, ShortProcedureThatExecutesNormallyWithCallbacks)
{
  auto procedure = testutils::CreateMessageProcedure("text");
  experimental::DomainRunner runner(CreateContext(), procedure.get());

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(::testing::AtLeast(1));

  runner.Start();

  // We need special waiting here to process queued connections in an event loop.
  auto predicate = [this, &runner]() { return runner.IsFinished(); };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));
}
