/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/procedure_runner.h"

#include <sequencergui/components/custom_meta_types.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>
#include <testutils/test_utils.h>

#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include <iostream>
#include <memory>
#include <thread>

using namespace sequencergui;

using msec = std::chrono::milliseconds;
//! Tests for ProcedureRunner.

class ProcedureRunnerTest : public ::testing::Test
{
};

TEST_F(ProcedureRunnerTest, InitialState)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(10000));
  procedure->Setup();
  ProcedureRunner runner(procedure.get());
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
}

//! ProcedureRunner dies before execution of procedure finished.
//! FIXME uncomment
// TEST_F(ProcedureRunnerTest, PrematureDeletion)
//{
//   auto procedure = testutils::CreateSingleWaitProcedure(msec(10000));
//   procedure->Setup();
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//  auto runner = std::make_unique<ProcedureRunner>();
//  runner->SetProcedure(procedure.get());

//  runner->Start();

//  std::this_thread::sleep_for(msec(100));
//  EXPECT_TRUE(runner->IsBusy());
//  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
//  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

//  // sudden runner destruction
//  runner.reset();

//  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
//}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, ShortProcedureThatExecutesNormally)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(10));
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  EXPECT_TRUE(runner->Start());

  EXPECT_TRUE(testutils::WaitForCompletion(*runner, testutils::kDefaultWaitPrecision + msec(20)));

  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 2);

  const int status_pos{1};  // position of status argument in InstructionStatusChanged signal
  // first signal should come with status "Not finished"
  EXPECT_EQ(spy_instruction_status.at(0).at(status_pos).value<QString>(), QString("Not finished"));
  // Second signal should come with status "Success"
  EXPECT_EQ(spy_instruction_status.at(1).at(status_pos).value<QString>(), QString("Success"));
}

//! Terminates procedure which runs too long.

TEST_F(ProcedureRunnerTest, StartAndTerminate)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(10000));
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  EXPECT_TRUE(runner->Start());

  std::this_thread::sleep_for(msec(100));
  EXPECT_TRUE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

  EXPECT_EQ(spy_instruction_status.count(), 1);
  EXPECT_EQ(spy_runner_status.count(), 1);  // running

  EXPECT_TRUE(runner->Stop());

  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kStopped);

  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
  EXPECT_FALSE(runner->IsBusy());

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 3);  // running, canceling, canceled
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, CopyVariable)
{
  auto procedure = testutils::CreateCopyProcedure();
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  EXPECT_TRUE(runner->Start());

  std::this_thread::sleep_for(msec(100));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 2);
  EXPECT_EQ(spy_variable_changed.count(), 1);

  auto arguments = spy_variable_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 2);
  EXPECT_EQ(arguments.at(0).value<QString>(), QStringLiteral("var1"));
  EXPECT_EQ(arguments.at(1).value<QString>(), QStringLiteral("42"));
}

//! Stepwise procedure execution.

TEST_F(ProcedureRunnerTest, StepwiseExecution)
{
  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kIdle);

  EXPECT_TRUE(runner->Step());
  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kPaused);

  EXPECT_TRUE(runner->Step());
  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_runner_status.count(), 4);  // running, paused, running, completed
}

//! Waiting for user input.

TEST_F(ProcedureRunnerTest, UserInput)
{
  // User input callback.
  auto on_user_input = [](auto, auto) { return "42"; };

  auto procedure = testutils::CreateInputProcedure();
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());
  runner->SetUserContext({on_user_input});

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  EXPECT_TRUE(runner->Start());
  std::this_thread::sleep_for(msec(50));

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);

  QTest::qWait(100);  // to make queued connection in UserController succeed

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(spy_runner_status.count(), 2);
  EXPECT_EQ(spy_variable_changed.count(), 1);

  auto arguments = spy_variable_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 2);
  EXPECT_EQ(arguments.at(0).value<QString>(), QStringLiteral("var0"));
  EXPECT_EQ(arguments.at(1).value<QString>(), QStringLiteral("42"));
}

//! Waiting for user choice.

TEST_F(ProcedureRunnerTest, UserChoice)
{
  // User choice to select Copy instruction, and not long Wait.
  auto on_user_choice = [](auto, auto)
  {
    return 1;  // selecting second instruction
  };

  auto procedure = testutils::CreateUserChoiceProcedure();
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());
  runner->SetUserContext({{}, on_user_choice});

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  EXPECT_TRUE(runner->Start());
  std::this_thread::sleep_for(msec(50));

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  QTest::qWait(100);  // to make queued connection in UserController succeed

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(spy_runner_status.count(), 2);
  EXPECT_EQ(spy_instruction_status.count(), 4);  // UserChoice, Copy
  EXPECT_EQ(spy_variable_changed.count(), 1);

  auto arguments = spy_variable_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 2);
  EXPECT_EQ(arguments.at(0).value<QString>(), QStringLiteral("var1"));
  EXPECT_EQ(arguments.at(1).value<QString>(), QStringLiteral("42"));
}

TEST_F(ProcedureRunnerTest, LogEvents)
{
  const std::string expected_message("abc");
  auto procedure = testutils::CreateMessageProcedure(expected_message);
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>(procedure.get());

  QSignalSpy spy_log_message(runner.get(), &ProcedureRunner::LogEventReceived);

  EXPECT_TRUE(runner->Start());

  std::this_thread::sleep_for(msec(20));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_log_message.count(), 2);

  // event reported by SequencerObserver::StartSingleStepImpl()
  QList<QVariant> arguments = spy_log_message.at(0);
  auto event1 = arguments.at(0).value<sequencergui::LogEvent>();
  EXPECT_EQ(event1.severity, Severity::kDebug);
  // FIXME message will probably change, how to make test robust?
  EXPECT_EQ(event1.message, std::string("StartSingleStep()"));

  // event reported by SequencerObserver::LogImpl()
  arguments = spy_log_message.at(1);
  auto event2 = arguments.at(0).value<sequencergui::LogEvent>();
  EXPECT_EQ(event2.severity, Severity::kInfo);
  EXPECT_EQ(event2.message, expected_message);
}
