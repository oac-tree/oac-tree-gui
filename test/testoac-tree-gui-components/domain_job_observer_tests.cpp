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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/user_context.h>

#include <sup/oac-tree/execution_status.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "oac_tree_gui/jobsystem/domain_job_observer.h"

namespace oac_tree_gui::test
{

//! Tests for DomainJobObserver class.

class DomainJobObserverTest : public ::testing::Test
{
public:
  testing::MockFunction<void(const domain_event_t&)> m_event_listener;
};

TEST_F(DomainJobObserverTest, InitialState)
{
  EXPECT_THROW(DomainJobObserver({}, {}), RuntimeException);
}

TEST_F(DomainJobObserverTest, OnStateChange)
{
  using ::sup::oac_tree::ExecutionStatus;
  using ::sup::oac_tree::InstructionState;

  DomainJobObserver observer(m_event_listener.AsStdFunction(), {});

  domain_event_t expected_event(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_STARTED}});

  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(1);

  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_STARTED});
}

}  // namespace oac_tree_gui::test
