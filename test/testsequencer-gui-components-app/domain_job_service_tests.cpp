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

#include "sequencergui/jobsystem/domain_job_service.h"

#include <sup/sequencer/i_job_info_io.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>
#include <testutils/mock_user_context.h>

#include <QTest>

using namespace sequencergui;

/**
 * @brief Tests for DomainJobService.
 */
class DomainJobServiceTest : public ::testing::Test
{
public:
  using mock_event_listener_t = ::testing::StrictMock<testutils::MockDomainEventListener>;
  using mock_user_listener_t = ::testing::StrictMock<testutils::MockUserContext>;

  std::unique_ptr<DomainJobService> CreateService()
  {
    return std::make_unique<DomainJobService>(m_event_listener.CreateDispatcherContext(),
                                              m_user_listener.CreateContext());
  }

  mock_event_listener_t m_event_listener;
  mock_user_listener_t m_user_listener;
};

TEST_F(DomainJobServiceTest, InstructionStateUpdated)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::InstructionState;

  auto service = CreateService();

  const InstructionState expected_state{false, ExecutionStatus::NOT_FINISHED};
  const sup::dto::uint32 expected_index{42};

  InstructionStateUpdatedEvent expected_event{expected_index, expected_state};
  EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(expected_event)).Times(1);

  service->GetJobInfoIO()->InstructionStateUpdated(expected_index, expected_state);

  QApplication::processEvents();
}
