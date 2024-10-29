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

#include <sequencergui/jobsystem/log_event.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/i_job_info_io.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>
#include <testutils/mock_user_context.h>

#include <QTest>
#include <future>
using ::testing::_;

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

TEST_F(DomainJobServiceTest, VariableUpdated)
{
  auto service = CreateService();

  const sup::dto::uint32 index{42};
  const sup::dto::AnyValue value(43);
  const bool connected{true};

  const VariableUpdatedEvent expected_event{index, value, connected};
  EXPECT_CALL(m_event_listener, OnVariableUpdated(expected_event)).Times(1);

  service->GetJobInfoIO()->VariableUpdated(index, value, connected);

  QApplication::processEvents();
}

TEST_F(DomainJobServiceTest, JobStateUpdated)
{
  using ::sup::sequencer::JobState;

  auto service = CreateService();

  const JobState state{JobState::kRunning};

  const JobStateChangedEvent expected_event{state};
  EXPECT_CALL(m_event_listener, OnJobStateChanged(expected_event)).Times(1);

  service->GetJobInfoIO()->JobStateUpdated(state);

  QApplication::processEvents();
}

TEST_F(DomainJobServiceTest, PutValue)
{
  auto service = CreateService();

  const sup::dto::AnyValue value(43);
  const std::string description("description");

  EXPECT_CALL(m_event_listener, OnLogEvent(_)).Times(1);

  service->GetJobInfoIO()->PutValue(value, description);

  QApplication::processEvents();
}

TEST_F(DomainJobServiceTest, GetUserValue)
{
  sup::dto::AnyValue initial_value{sup::dto::SignedInteger32Type, 41};
  const std::string description("description");
  const UserInputArgs args{initial_value, description};

  const sup::dto::AnyValue expected_value{sup::dto::SignedInteger32Type, 42};
  const UserInputResult expected_result{expected_value, true};

  auto service = CreateService();

  ON_CALL(m_user_listener, OnUserInputRequest(args))
      .WillByDefault(::testing::Return(expected_result));

  EXPECT_CALL(m_user_listener, OnUserInputRequest(args)).Times(1);

  // mimick sequencer thread asking for user input
  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&service, &ready_for_test, &initial_value, &description]()
  {
    ready_for_test.set_value();
    return service->GetJobInfoIO()->GetUserValue(initial_value, description);
  };

  // launching runner in a thread
  std::future<bool> future_result = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // waiting for the future, and still allowing event loop to function
  auto predicate = [&future_result]()
  { return future_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_TRUE(future_result.get());
  EXPECT_EQ(initial_value, expected_value);
}

TEST_F(DomainJobServiceTest, GetUserChoice)
{
  const std::vector<std::string> options({"option1"});
  sup::dto::AnyValue metadata{sup::dto::SignedInteger32Type, 41};
  const UserChoiceArgs args{options, metadata};

  const int user_index_choice{1};
  const UserChoiceResult expected_result{user_index_choice, true};

  auto service = CreateService();

  ON_CALL(m_user_listener, OnUserChoiceRequest(args))
      .WillByDefault(::testing::Return(expected_result));

  EXPECT_CALL(m_user_listener, OnUserChoiceRequest(args)).Times(1);

  // mimick sequencer thread asking for user choice
  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&service, &ready_for_test, &options, &metadata]()
  {
    ready_for_test.set_value();
    return service->GetJobInfoIO()->GetUserChoice(options, metadata);
  };

  // launching runner in a thread
  std::future<int> future_result = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // waiting for the future, and still allowing event loop to function
  auto predicate = [&future_result]()
  { return future_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_EQ(future_result.get(), user_index_choice);
}

TEST_F(DomainJobServiceTest, Message)
{
  auto service = CreateService();

  const std::string message("mesage");

  EXPECT_CALL(m_event_listener, OnLogEvent(CreateLogEvent(Severity::kInfo, message))).Times(1);

  service->GetJobInfoIO()->Message(message);

  QApplication::processEvents();
}

TEST_F(DomainJobServiceTest, Log)
{
  auto service = CreateService();

  const int severity{42};
  const std::string message("mesage");

  EXPECT_CALL(m_event_listener,
              OnLogEvent(CreateLogEvent(static_cast<Severity>(severity), message)))
      .Times(1);

  service->GetJobInfoIO()->Log(severity, message);

  QApplication::processEvents();
}

TEST_F(DomainJobServiceTest, NextInstructionsUpdated)
{
  auto service = CreateService();

  const std::vector<sup::dto::uint32> indices({42, 43});

  const NextLeavesChangedEvent expected_event{indices};

  EXPECT_CALL(m_event_listener, OnNextLeavesChanged(expected_event)).Times(1);

  service->GetJobInfoIO()->NextInstructionsUpdated(indices);

  QApplication::processEvents();
}
