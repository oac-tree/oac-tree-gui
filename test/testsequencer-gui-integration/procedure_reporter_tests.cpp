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

#include "sequencergui/jobsystem/procedure_reporter.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/transform/gui_object_builder.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <gtest/gtest.h>
#include <testutils/mock_sequencer_observer.h>
#include <testutils/standard_procedures.h>

#include <QDebug>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(sequencergui::RunnerStatus)
Q_DECLARE_METATYPE(std::vector<sequencergui::InstructionItem*>)
Q_DECLARE_METATYPE(sequencergui::InstructionItem*)

using namespace sequencergui;

//! Test ProcedureReporter class.

class ProcedureReporterTests : public ::testing::Test
{
};

//! Validating that the call of the method from the domain triggers queued connection.

TEST_F(ProcedureReporterTests, OnDomainRunnerStatusChanged)
{
  ProcedureReporter reporter({});

  QSignalSpy spy_runner_status(&reporter, &ProcedureReporter::RunnerStatusChanged);

  // should trigger queued signal
  reporter.OnDomainRunnerStatusChanged(RunnerStatus::kCompleted);

  auto predicate = [&spy_runner_status]() { return spy_runner_status.count() == 1; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  QList<QVariant> arguments = spy_runner_status.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto status = arguments.at(0).value<RunnerStatus>();
  EXPECT_EQ(status, RunnerStatus::kCompleted);
}

//! Validating that tick callback from the domain leads to the signal with InstructionItems
//! representing domain leaves.

TEST_F(ProcedureReporterTests, OnDomainProcedureTick)
{
  // preparing domain procedure and its GUI counterpart
  GUIObjectBuilder builder;
  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  EXPECT_NO_THROW(procedure->Setup());
  auto procedure_item = builder.CreateProcedureItem(procedure.get(), /*root_only*/ false);

  // accessing instructions in GUI and domain
  auto sequence = procedure->GetTopInstructions().at(0);
  auto sequence_item = procedure_item->GetInstructionContainer()->GetInstructions().at(0);
  auto message0 = sequence->ChildInstructions().at(0);
  auto message1 = sequence->ChildInstructions().at(1);
  auto message_item0 = sequence_item->GetInstructions().at(0);
  auto message_item1 = sequence_item->GetInstructions().at(1);

  // initialising reporter
  auto find_instruction_item = [&builder](const auto& instruction)
  { return builder.FindInstructionItem(&instruction); };
  ProcedureReporter reporter(find_instruction_item);

  QSignalSpy spy_next_leaves(&reporter, &ProcedureReporter::NextLeavesChanged);

  // preparing domain runner
  testutils::EmptyUserInterface empty_ui;
  sup::sequencer::Runner runner(empty_ui);
  EXPECT_NO_THROW(runner.SetProcedure(procedure.get()));
  auto tick_callback = [&reporter](const procedure_t& procedure)
  { reporter.OnDomainProcedureTick(procedure); };
  runner.SetTickCallback(tick_callback);

  // making one step, expecting to stop at the beginning of the second message instruction
  EXPECT_NO_THROW(runner.ExecuteSingle());
  EXPECT_FALSE(runner.IsFinished());
  EXPECT_FALSE(runner.IsRunning());

  EXPECT_EQ(sup::sequencer::GetNextLeaves(*procedure),
            std::vector<const instruction_t*>({message1}));

  // checking queued signal that reports next leaves InstructionItem
  auto predicate = [&spy_next_leaves]() { return spy_next_leaves.count() == 1; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  QList<QVariant> arguments = spy_next_leaves.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto leaves = arguments.at(0).value<std::vector<InstructionItem*>>();
  EXPECT_EQ(leaves, std::vector<InstructionItem*>({message_item1}));
}

//! Validating that call to DomainInstructionStatusChange triggers queued connection.

TEST_F(ProcedureReporterTests, OnDomainInstructionStatusChange)
{
  WaitItem wait_item;
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  std::map<const instruction_t*, InstructionItem*> instruction_map{{wait.get(), &wait_item}};

  // initialising reporter
  auto find_instruction_item = [&instruction_map](const auto& instruction)
  { return instruction_map.at(&instruction); };
  ProcedureReporter reporter(find_instruction_item);

  reporter.OnDomainInstructionStatusChange(wait.get(), "Success");

  QSignalSpy spy_instruction_status(&reporter, &ProcedureReporter::InstructionStatusChanged);

  auto predicate = [&spy_instruction_status]() { return spy_instruction_status.count() == 1; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  EXPECT_EQ(wait_item.GetStatus(), "Success");

  QList<QVariant> arguments = spy_instruction_status.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto reported_item = arguments.at(0).value<sequencergui::InstructionItem*>();
  EXPECT_EQ(reported_item, &wait_item);
}
