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

#include "sequencergui/jobsystem/domain_procedure_observer.h"

#include <sequencergui/jobsystem/user_context.h>

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>

#include <mvvm/test/mock_callback_listener.h>

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainProcedureObserver class.

class DomainProcedureObserverTest : public ::testing::Test
{
public:
  mvvm::test::MockCallbackListener<domain_event_t> m_event_listener;
};

TEST_F(DomainProcedureObserverTest, InitialState)
{
  EXPECT_THROW(DomainProcedureObserver({}, {}), RuntimeException);
}

TEST_F(DomainProcedureObserverTest, OnStateChange)
{
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  DomainProcedureObserver observer(m_event_listener.CreateCallback(), {});

  domain_event_t expected_event(InstructionStatusChangedEvent{
      instruction.get(), ::sup::sequencer::ExecutionStatus::NOT_STARTED});

  EXPECT_CALL(m_event_listener, OnCallback(expected_event)).Times(1);

  observer.UpdateInstructionStatus(instruction.get());
}
