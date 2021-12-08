/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QDebug>
#include <iostream>
#include <memory>
#include <thread>

using namespace sequi;

using msec = std::chrono::milliseconds;
//! Tests for ProcedureRunner.

class ProcedureRunnerTest : public ::testing::Test
{
public:
  //! Retuns domain procedure that contain only a single wait instruction.
  std::unique_ptr<procedure_t> CreateProcedureWithSingleWait(int msec_to_wait) const
  {
    auto result = std::make_unique<procedure_t>();
    auto wait0 = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait0->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute,
                        std::to_string(double(msec_to_wait) / 1000));  // expects in sec
    result->PushInstruction(wait0.release());
    result->AddAttribute(DomainConstants::kTickTimeOutAttribute, "0.01");  // 10 msec
    return result;
  }

  //! Returns procedure that makes a single copy.
  std::unique_ptr<procedure_t> CreateProcedureWithCopy() const
  {
    auto result = std::make_unique<procedure_t>();

    auto copy = DomainUtils::CreateDomainInstruction(DomainConstants::kCopyInstructionType);
    copy->AddAttribute(sequi::DomainConstants::kInputAttribute, "var0");
    copy->AddAttribute(sequi::DomainConstants::kOutputAttribute, "var1");
    result->PushInstruction(copy.release());

    auto var0 = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
    var0->AddAttribute(DomainConstants::kNameAttribute, "var0");
    var0->AddAttribute(DomainConstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
    var0->AddAttribute(DomainConstants::kValueAttribute, "42");
    result->AddVariable("var0", var0.release());

    auto var1 = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
    var1->AddAttribute(DomainConstants::kNameAttribute, "var1");
    var1->AddAttribute(DomainConstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
    var1->AddAttribute(DomainConstants::kValueAttribute, "0");
    result->AddVariable("var1", var1.release());

    return result;
  }

  //! Creates sequence with two waits.
  std::unique_ptr<procedure_t> CreateNestedProcedure() const
  {
    auto result = std::make_unique<procedure_t>();
    auto sequence = DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
    auto wait0 = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait0->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "0.01");
    auto wait1 = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait1->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "0.01");

    sequence->InsertInstruction(wait0.release(), 0);
    sequence->InsertInstruction(wait1.release(), 1);

    result->PushInstruction(sequence.release());
    return result;
  }

  //! Creates procedure with one variable and input instruction.
  std::unique_ptr<procedure_t> CreateInputProcedure() const
  {
    auto result = std::make_unique<procedure_t>();
    auto sequence = DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
    auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kInputInstructionType);
    input->AddAttribute(DomainConstants::kInputTargetAttribute, "var0");
    input->AddAttribute(DomainConstants::kDescriptionAttribute, "description");
    sequence->InsertInstruction(input.release(), 0);
    result->PushInstruction(sequence.release());

    auto var0 = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
    var0->AddAttribute(DomainConstants::kNameAttribute, "var0");
    var0->AddAttribute(DomainConstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
    var0->AddAttribute(DomainConstants::kValueAttribute, "0");
    result->AddVariable("var0", var0.release());

    return result;
  }
};

TEST_F(ProcedureRunnerTest, InitialState)
{
  ProcedureRunner runner;
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
}

//! ProcedureRunner dies before execution of procedure finished.

TEST_F(ProcedureRunnerTest, PrematureDeletion)
{
  auto procedure = CreateProcedureWithSingleWait(10000);
  EXPECT_TRUE(procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  auto runner = std::make_unique<ProcedureRunner>();

  runner->ExecuteProcedure(procedure.get());
  std::this_thread::sleep_for(msec(100));
  EXPECT_TRUE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

  // sudden runner destruction
  runner.reset();

  EXPECT_TRUE(procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::NOT_STARTED);
}

//! Terminates procedure which runs too long.

TEST_F(ProcedureRunnerTest, StartAndTerminate)
{
  auto procedure = CreateProcedureWithSingleWait(10000);

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->ExecuteProcedure(procedure.get());
  std::this_thread::sleep_for(msec(100));
  EXPECT_TRUE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

  EXPECT_EQ(spy_instruction_status.count(), 1);
  EXPECT_EQ(spy_runner_status.count(), 1);  // running

  runner->Terminate();

  std::this_thread::sleep_for(msec(10));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCanceled);

  EXPECT_TRUE(procedure->GetStatus() == ::sup::sequencer::ExecutionStatus::NOT_STARTED);
  EXPECT_FALSE(runner->IsBusy());

  EXPECT_EQ(spy_instruction_status.count(), 2);
  EXPECT_EQ(spy_runner_status.count(), 3);  // running, canceling, canceled
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, StartAndStop)
{
  auto procedure = CreateProcedureWithSingleWait(10);

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->ExecuteProcedure(procedure.get());
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
  auto procedure = CreateProcedureWithSingleWait(100);

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->ExecuteProcedure(procedure.get());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner->WaitForCompletion(1.0));
  EXPECT_FALSE(runner->IsBusy());
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
}

//! Short procedure which is executed normally.

TEST_F(ProcedureRunnerTest, CopyVariable)
{
  auto procedure = CreateProcedureWithCopy();

  auto runner = std::make_unique<ProcedureRunner>();

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  runner->ExecuteProcedure(procedure.get());
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
  auto procedure = CreateNestedProcedure();

  auto runner = std::make_unique<ProcedureRunner>();
  runner->SetWaitingMode(WaitingMode::kWaitForRelease);

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);

  runner->ExecuteProcedure(procedure.get());
  std::this_thread::sleep_for(msec(50));

  // Making 6 steps (3 instructions, 2 status change per each)
  for (size_t i = 0; i < 6; ++i)
  {
    EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
    runner->onMakeStepRequest();
    std::this_thread::sleep_for(msec(50));
  }
  std::this_thread::sleep_for(msec(50));

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
}

//! Waiting for user input.

TEST_F(ProcedureRunnerTest, UserInput)
{
  auto procedure = CreateInputProcedure();

  auto runner = std::make_unique<ProcedureRunner>();
  runner->SetWaitingMode(WaitingMode::kProceed);

  QSignalSpy spy_instruction_status(runner.get(), &ProcedureRunner::InstructionStatusChanged);
  QSignalSpy spy_runner_status(runner.get(), &ProcedureRunner::RunnerStatusChanged);
  QSignalSpy spy_input_request(runner.get(), &ProcedureRunner::InputRequest);
  QSignalSpy spy_variable_changed(runner.get(), &ProcedureRunner::VariableChanged);

  runner->ExecuteProcedure(procedure.get());
  std::this_thread::sleep_for(msec(50));

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  runner->SetAsUserInput("42");
  std::this_thread::sleep_for(msec(50));

  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(spy_input_request.count(), 1);
  EXPECT_EQ(spy_runner_status.count(), 2);
  EXPECT_EQ(spy_variable_changed.count(), 1);

  auto arguments = spy_variable_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 2);
  EXPECT_EQ(arguments.at(0).value<QString>(), QStringLiteral("var0"));
  qDebug() << arguments.at(1).value<QString>();
  EXPECT_EQ(arguments.at(1).value<QString>(), QStringLiteral("42"));
}
