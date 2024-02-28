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

#include <sequencergui/experimental/domain_events.h>
#include <sequencergui/experimental/domain_event_dispatcher.h>

#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>

using namespace sequencergui::experimental;
using ::testing::_;

//! Tests for DomainRunner class.

class DomainRunnerTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper method to create a context necessary for DomainEventDispatcher to function.
   */
  DomainEventDispatcherContext CreateContext()
  {
    return {{}, m_instruction_status_listener.CreateCallback(),
            m_job_status_listener.CreateCallback()};
  }

  mvvm::test::MockCallbackListener<InstructionStatusChanged> m_instruction_status_listener;
  mvvm::test::MockCallbackListener<JobStatusChanged> m_job_status_listener;
};

TEST_F(DomainRunnerTest, InitialState)
{
  DomainRunner runner(CreateContext());

}

