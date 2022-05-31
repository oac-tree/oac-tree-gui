/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/procedurerunner.h"

#include "ExecutionStatus.h"
#include "Instruction.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequencer_types.h"
#include "test_procedures.h"

#include <gtest/gtest.h>

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
public:
};

TEST_F(ProcedureRunnerTest, InitialState)
{
  ProcedureRunner runner;
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
}

//! ProcedureRunner dies before execution of procedure finished.
//! FIXME uncomment
//TEST_F(ProcedureRunnerTest, PrematureDeletion)
//{
//  auto procedure = testutils::CreateSingleWaitProcedure(msec(10000));
//  procedure->Setup();
//  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

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

//! Terminates procedure which runs too long.

TEST_F(ProcedureRunnerTest, StartAndTerminate)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(10000));
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->SetProcedure(procedure.get());

  runner->Start();

  std::this_thread::sleep_for(msec(100));
  EXPECT_TRUE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

  EXPECT_EQ(spy_instruction_status.count(), 1);
  EXPECT_EQ(spy_runner_status.count(), 1);  // running

  runner->Stop();

  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kStopped);

  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
  EXPECT_FALSE(runner->IsBusy());

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 3);  // running, canceling, canceled
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, StartAndStop)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(10));
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->SetProcedure(procedure.get());

  runner->Start();

  std::this_thread::sleep_for(msec(100));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 2);

  auto arguments = spy_instruction_status.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, WaitForCompletion)
{
  auto procedure = testutils::CreateSingleWaitProcedure(msec(100));
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->SetProcedure(procedure.get());

  runner->Start();

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner->WaitForCompletion(1.0));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, CopyVariable)
{
  auto procedure = testutils::CreateCopyProcedure();
  procedure->Setup();

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  runner->SetProcedure(procedure.get());

  runner->Start();

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

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->SetProcedure(procedure.get());

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kIdle);

  runner->Step();
  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kPaused);

  runner->Step();
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

  auto runner = std::make_unique<ProcedureRunner>();
  runner->SetUserContext({on_user_input});

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  runner->SetProcedure(procedure.get());
  runner->Start();
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

  auto runner = std::make_unique<ProcedureRunner>();
  runner->SetUserContext({{}, on_user_choice});

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  runner->SetProcedure(procedure.get());
  runner->Start();
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

//! Set procedure twice.

TEST_F(ProcedureRunnerTest, SetProcedureTwice)
{
  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  auto procedure = testutils::CreateSingleWaitProcedure(msec(10));
  procedure->Setup();

  runner->SetProcedure(procedure.get());

  // Setting procedure second time to make sure that time of life of internal components of
  // ProcedureRunner is consistent
  runner->SetProcedure(procedure.get());

  runner->Start();

  std::this_thread::sleep_for(msec(100));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 2);

  auto arguments = spy_instruction_status.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
}
