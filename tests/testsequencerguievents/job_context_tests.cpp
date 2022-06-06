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

#include "sequencergui/monitor/job_context.h"

#include "Instruction.h"
#include "mock_item_listener.h"
#include "sequencergui/core/exceptions.h"
#include "sequencergui/model/application_models.h"
#include "sequencergui/model/instruction_container_item.h"
#include "sequencergui/model/item_constants.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_examples.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"
#include "sequencergui/model/standard_instruction_items.h"
#include "sequencergui/model/standard_variable_items.h"
#include "sequencergui/model/workspace_item.h"
#include "sequencergui/monitor/job_utils.h"

#include "mvvm/model/model_utils.h"
#include "mvvm/standarditems/container_item.h"

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>
#include <thread>
#include <tuple>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobContext class.

class JobContextTest : public ::testing::Test
{
public:
  JobContextTest() { m_job_item = m_models.GetJobModel()->InsertItem<JobItem>(); }

  //! Creates invalid procedure that will cause JobContext to crash during the preparation.
  static ProcedureItem* CreateInvalidProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    // InstructionItem that doesn't have counterpart in SequencerDomain
    auto item =
        model->InsertItem<UnknownInstructionItem>(procedure_item->GetInstructionContainer());
    return procedure_item;
  }

  //! Creates procedure with single wait instruction.
  static ProcedureItem* CreateSingleWaitProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto wait = model->InsertItem<WaitItem>(procedure_item->GetInstructionContainer());
    wait->SetTimeout(0.01);
    return procedure_item;
  }

  //! Creates procedure with two variables and single Copy instruction.
  static ProcedureItem* CreateCopyProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto copy = model->InsertItem<CopyItem>(procedure_item->GetInstructionContainer());
    copy->SetInput("var0");
    copy->SetOutput("var1");

    auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var0->SetName("var0");
    var0->SetJsonType(R"({"type":"uint32"})");
    var0->SetJsonValue("42");

    auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue("43");

    return procedure_item;
  }

  //! Creates procedure with local include.
  static ProcedureItem* CreateIncludeProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
    sequence->SetName("MySequence");
    model->InsertItem<WaitItem>(sequence);

    auto repeat = model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
    repeat->SetRepeatCount(1);
    repeat->SetIsRootFlag(true);
    auto include = model->InsertItem<IncludeItem>(repeat);
    include->SetPath("MySequence");

    return procedure_item;
  }

  static ProcedureItem* CreateInputProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
    sequence->SetName("MySequence");
    auto input0 = model->InsertItem<InputItem>(sequence);
    input0->SetTargetVariableName("var1");
    input0->SetDescription("Your ID");

    auto wait0 = model->InsertItem<WaitItem>(sequence);

    auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue(R"(0)");

    return procedure_item;
  }

  static ProcedureItem* CreateUserChoiceProcedure(SequencerModel* model)
  {
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto userchoice = model->InsertItem<UserChoiceItem>(procedure_item->GetInstructionContainer());
    auto wait0 = model->InsertItem<WaitItem>(userchoice);
    wait0->SetTimeout(10.0);

    auto copy = model->InsertItem<CopyItem>(userchoice);
    copy->SetInput("var0");
    copy->SetOutput("var1");

    auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var0->SetName("var0");
    var0->SetJsonType(R"({"type":"uint32"})");
    var0->SetJsonValue("42");

    auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue("0");

    return procedure_item;
  }

  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

TEST_F(JobContextTest, InitialState)
{
  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  EXPECT_EQ(job_context.GetExpandedProcedure(), nullptr);
}

//! Calling PrepareJobRequest.

TEST_F(JobContextTest, PrepareJobRequest)
{
  auto procedure = CreateSingleWaitProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  EXPECT_EQ(job_context.GetExpandedProcedure(), nullptr);

  job_context.onPrepareJobRequest();

  auto expanded_procedure = job_context.GetExpandedProcedure();
  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_EQ(job_context.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());
  EXPECT_TRUE(job_context.IsValid());

  // calling expanded second time (expanded should be rebuild)
  job_context.onPrepareJobRequest();

  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_NE(job_context.GetExpandedProcedure(),
            expanded_procedure);  // old procedure was regenerated
  EXPECT_EQ(job_context.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());
}

//! Attempt to use JobContext with invalid procedure.

TEST_F(JobContextTest, InvalidProcedure)
{
  auto procedure = CreateInvalidProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  EXPECT_THROW(job_context.onPrepareJobRequest(), TransformToDomainException);
  EXPECT_FALSE(job_context.IsValid());
}

//! Normal execution of the procedure with single wait instruction.

TEST_F(JobContextTest, PrematureDeletion)
{
  auto procedure = CreateSingleWaitProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  {
    JobContext job_context(m_job_item);
    job_context.onPrepareJobRequest();
    job_context.onStartRequest();
  }

  EXPECT_EQ(procedure->GetStatus(), std::string());
}

TEST_F(JobContextTest, ProcedureWithSingleWait)
{
  auto procedure = CreateSingleWaitProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(procedure->GetStatus(), std::string());
  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);
}

//! Normal execution of procedure with single wait.
//! Validating signaling going from expanded procedure (instruction status change).

TEST_F(JobContextTest, ProcedureWithSingleWaitStatusChangedSignals)
{
  auto procedure = CreateSingleWaitProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(procedure->GetStatus(), std::string());
  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobContext job_context(m_job_item);
  EXPECT_FALSE(job_context.IsValid());
  job_context.onPrepareJobRequest();
  EXPECT_TRUE(job_context.IsValid());

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  auto wait = instructions.at(0);

  MockItemListener listener(wait);

  EXPECT_CALL(listener, OnPropertyChanged(wait, itemconstants::kStatus)).Times(2);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);
}

TEST_F(JobContextTest, ProcedureWithVariableCopy)
{
  auto procedure = CreateCopyProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto vars = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars.size(), 2);
  EXPECT_EQ(vars.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars.at(1)->GetJsonValue(), std::string("43"));

  JobContext job_context(m_job_item);
  job_context.onPrepareJobRequest();

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));
}

TEST_F(JobContextTest, LocalIncludeScenario)
{
  auto procedure = CreateIncludeProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);
  job_context.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_context.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 8);  // Repeat, Include, Sequence, Wait x 2

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

TEST_F(JobContextTest, UserInputScenario)
{
  auto procedure = CreateInputProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  auto on_user_input = [](auto, auto) { return "42"; };
  job_context.SetUserContext({on_user_input});

  job_context.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job_context.IsRunning());
}

TEST_F(JobContextTest, UserChoiceScenario)
{
  auto procedure = CreateUserChoiceProcedure(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobContext job_context(m_job_item);

  // callback to select Copy instruction
  auto on_user_choice = [](auto, auto) { return 1; };
  job_context.SetUserContext({{}, on_user_choice});

  job_context.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job_context, &JobContext::InstructionStatusChanged);

  job_context.onStartRequest();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 4);

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job_context.IsRunning());
}
