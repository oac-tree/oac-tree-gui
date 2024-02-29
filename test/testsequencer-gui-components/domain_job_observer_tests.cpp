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

#include "sequencergui/experimental/domain_job_observer.h"

#include <sequencergui/core/exceptions.h>

#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainJobObserver class.

class DomainJobObserverTest : public ::testing::Test
{
public:
  mvvm::test::MockCallbackListener<experimental::domain_event_t> m_event_listener;
};

TEST_F(DomainJobObserverTest, InitialState)
{
  EXPECT_THROW(experimental::DomainJobObserver({}), RuntimeException);
}

TEST_F(DomainJobObserverTest, OnStateChange)
{
  experimental::DomainJobObserver observer(m_event_listener.CreateCallback());

  experimental::domain_event_t expected_event(
      experimental::JobStateChanged{::sup::sequencer::JobState::kInitial});
  EXPECT_CALL(m_event_listener, OnCallback(expected_event)).Times(1);

  observer.OnStateChange(sup::sequencer::JobState::kInitial);
}
