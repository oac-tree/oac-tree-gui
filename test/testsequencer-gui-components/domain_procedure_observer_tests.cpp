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

#include "sequencergui/jobsystem/domain_procedure_observer_v2.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/jobsystem/user_context.h>

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainProcedureObserverV2 class.

class DomainProcedureObserverTest : public ::testing::Test
{
public:
  testing::MockFunction<void(const domain_event_t&)> m_event_listener;
};

TEST_F(DomainProcedureObserverTest, InitialState)
{
  EXPECT_THROW(DomainProcedureObserverV2({}, {}), RuntimeException);
}

TEST_F(DomainProcedureObserverTest, OnStateChange)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::InstructionState;

  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  DomainProcedureObserverV2 observer(m_event_listener.AsStdFunction(), {});

  domain_event_t expected_event(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_STARTED}});

  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(1);

  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_STARTED});
}
