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
#include <sequencergui/experimental/domain_events.h>

#include <mvvm/test/mock_callback_listener.h>

#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainRunner class.

class DomainRunnerTest : public ::testing::Test
{
public:
  mvvm::test::MockCallbackListener<experimental::domain_event_t> m_event_listener;
};

//! Running short domain procedure that executes normally.

TEST_F(DomainRunnerTest, ShortProcedureThatExecutesNormally)
{
  auto procedure = testutils::CreateMessageProcedure("text");
  experimental::DomainRunner runner(m_event_listener.CreateCallback(), *procedure);

  runner.Start();

  auto final_state = runner.WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
}
