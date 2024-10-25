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

#include "sequencergui/jobsystem/domain_runner_service.h"

#include <sequencergui/jobsystem/domain_event_dispatcher_context.h>
#include <sequencergui/jobsystem/user_context.h>

#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/standard_procedures.h>

#include <QTest>

using namespace sequencergui;
using ::testing::_;

class DomainRunnerServiceTest : public ::testing::Test
{
public:
  testutils::MockDomainEventListener m_listener;
};

//! Running short domain procedure that executes normally.
//! Testing queued connection.

TEST_F(DomainRunnerServiceTest, ShortProcedureThatExecutesNormally)
{
  auto procedure = testutils::CreateMessageProcedure("text");
  auto procedure_ptr = procedure.get();

  DomainRunnerService runner(m_listener.CreateDispatcherContext(), {}, std::move(procedure));

  // JobState: initial, stepping, paused
  EXPECT_CALL(m_listener, OnJobStateChanged(_)).Times(3);
  // Instruction: repeat (not finished) + one increment (not finished, success)
  EXPECT_CALL(m_listener, OnInstructionStateUpdated(_)).Times(2);
  EXPECT_CALL(m_listener, OnLogEvent(_)).Times(1);
  EXPECT_CALL(m_listener, OnNextLeavesChangedV2(_)).Times(1);

  runner.Start();

  auto predicate = [this, &runner]()
  { return runner.GetJobState() == sup::sequencer::JobState::kSucceeded; };

  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));  // letting event loop to work

  // The listener is connected with the DomainRunnerService via queued connections. So it's not
  // enough to wait for the "succeeded" state of the runner, one have to force processing of all
  // queued signals.
  QApplication::processEvents();

  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}
