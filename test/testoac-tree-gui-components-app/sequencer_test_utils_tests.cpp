/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "testutils/sequencer_test_utils.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_item_transform_helper.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_job_handler.h>

#include <QTimer>

using msec = std::chrono::milliseconds;

namespace oac_tree_gui::test
{

/**
 * @brief Tests for Qt event loop waits from sequencer_test_utils.h.
 */
class SequencerTestUtilsTest : public ::testing::Test
{
public:
  SequencerTestUtilsTest() : m_job_handler(m_listener, &m_job_item)
  {
    ON_CALL(m_listener, IsRunning(::testing::_)).WillByDefault(::testing::Return(false));
    ON_CALL(m_listener, GetRunnerStatus(::testing::_))
        .WillByDefault(::testing::Return(RunnerStatus::kInitial));
  }

  LocalJobItem m_job_item;
  ::testing::NiceMock<MockJobHandlerListener> m_listener;
  MockJobHandler m_job_handler;
};

//! WaitForCompletion succeeds when the status arrives through the event loop.
TEST_F(SequencerTestUtilsTest, WaitForCompletion)
{
  QTimer::singleShot(0, [this]() { m_job_item.SetStatus(RunnerStatus::kSucceeded); });

  EXPECT_TRUE(WaitForCompletion(m_job_handler));
}

TEST_F(SequencerTestUtilsTest, WaitForCompletionWithExpectedStatus)
{
  m_job_item.SetStatus(RunnerStatus::kHalted);

  EXPECT_TRUE(WaitForCompletion(m_job_handler, RunnerStatus::kHalted));
}

TEST_F(SequencerTestUtilsTest, WaitForCompletionWhileRunning)
{
  ON_CALL(m_listener, IsRunning(::testing::_)).WillByDefault(::testing::Return(true));
  m_job_item.SetStatus(RunnerStatus::kSucceeded);

  auto result = WaitForCompletion(m_job_handler, RunnerStatus::kSucceeded, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("running true"));
}

TEST_F(SequencerTestUtilsTest, WaitForCompletionWrongStatus)
{
  m_job_item.SetStatus(RunnerStatus::kFailed);

  auto result = WaitForCompletion(m_job_handler, RunnerStatus::kSucceeded, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("job status 'Failure', expected 'Success'"));
}

TEST_F(SequencerTestUtilsTest, WaitForRunning)
{
  ON_CALL(m_listener, IsRunning(::testing::_)).WillByDefault(::testing::Return(true));

  EXPECT_TRUE(WaitForRunning(m_job_handler));
}

TEST_F(SequencerTestUtilsTest, WaitForRunningTimeout)
{
  auto result = WaitForRunning(m_job_handler, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("runner status 'Initial'"));
}

TEST_F(SequencerTestUtilsTest, WaitForJobStatus)
{
  QTimer::singleShot(0, [this]() { m_job_item.SetStatus(RunnerStatus::kPaused); });

  EXPECT_TRUE(WaitForJobStatus(m_job_item, RunnerStatus::kPaused));

  auto result = WaitForJobStatus(m_job_item, RunnerStatus::kRunning, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("job status 'Paused', expected 'Running'"));
}

TEST_F(SequencerTestUtilsTest, WaitForInstructionStatus)
{
  auto instruction = CreateInstructionItem(domainconstants::kWaitInstructionType);
  QTimer::singleShot(0, [&instruction]() { instruction->SetStatus(InstructionStatus::kSuccess); });

  EXPECT_TRUE(WaitForInstructionStatus(*instruction, InstructionStatus::kSuccess));

  auto result = WaitForInstructionStatus(*instruction, InstructionStatus::kHalted, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("Wait status 'Success', expected 'Halted'"));
}

TEST_F(SequencerTestUtilsTest, WaitForValue)
{
  const sup::dto::AnyValue value0{sup::dto::SignedInteger32Type, 0};
  const sup::dto::AnyValue value1{sup::dto::SignedInteger32Type, 42};

  auto variable = CreateVariableItem(domainconstants::kLocalVariableType);
  variable->SetName("var0");
  SetAnyValue(value0, *variable);
  QTimer::singleShot(0, [&variable, &value1]() { SetAnyValue(value1, *variable); });

  EXPECT_TRUE(WaitForValue(*variable, value1));

  auto result = WaitForValue(*variable, value0, msec(10));
  EXPECT_FALSE(result);
  EXPECT_THAT(result.message(), ::testing::HasSubstr("variable 'var0' value"));
}

TEST_F(SequencerTestUtilsTest, WaitForValueWithoutValue)
{
  const sup::dto::AnyValue value{sup::dto::SignedInteger32Type, 42};

  {  // empty AnyValueItem
    auto variable = CreateVariableItem(domainconstants::kLocalVariableType);
    auto result = WaitForValue(*variable, value, msec(10));
    EXPECT_FALSE(result);
    EXPECT_THAT(result.message(), ::testing::HasSubstr("value null, expected 42"));
  }

  {  // no AnyValueItem
    const VariableItem variable;
    auto result = WaitForValue(variable, value, msec(10));
    EXPECT_FALSE(result);
    EXPECT_THAT(result.message(), ::testing::HasSubstr("value no value, expected 42"));
  }
}

}  // namespace oac_tree_gui::test
