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

#include "sequencergui/monitor/jobcontext.h"

#include "Instruction.h"
#include "sequencergui/model/procedureexamples.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/modelutils.h"

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>
#include <thread>
#include <tuple>

using namespace sequi;
using msec = std::chrono::milliseconds;

//! Tests for JobContext class.

class JobContextTest : public ::testing::Test
{
public:
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

  SequencerModel m_model;
};

//! Normal execution of the procedure with single wait instruction.

TEST_F(JobContextTest, PrematureDeletion)
{
  auto procedure = CreateSingleWaitProcedure(&m_model);
  {
    JobContext job(procedure);
    job.onPrepareJobRequest();
    job.onStartRequest();
  }

  // FIXME add tests for ProcedureItem status
}

TEST_F(JobContextTest, ProcedureWithSingleWait)
{
  auto procedure = CreateSingleWaitProcedure(&m_model);

  JobContext job(procedure);
  job.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = ModelView::Utils::FindItems<WaitItem>(job.GetExpandedModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Not started");
}

TEST_F(JobContextTest, ProcedureWithVariableCopy)
{
  auto procedure = CreateCopyProcedure(&m_model);

  auto vars = ModelView::Utils::FindItems<LocalVariableItem>(&m_model);
  ASSERT_EQ(vars.size(), 2);
  EXPECT_EQ(vars.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars.at(1)->GetJsonValue(), std::string("43"));

  JobContext job(procedure);
  job.onPrepareJobRequest();

  auto vars_inside = ModelView::Utils::FindItems<LocalVariableItem>(job.GetExpandedModel());

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));
}

TEST_F(JobContextTest, LocalIncludeScenario)
{
  auto procedure = CreateIncludeProcedure(&m_model);

  JobContext job(procedure);
  job.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 8);  // Repeat, Include, Sequence, Wait x 2

  auto instructions = ModelView::Utils::FindItems<WaitItem>(job.GetExpandedModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Not started");
}

TEST_F(JobContextTest, UserInputScenario)
{
  auto procedure = CreateInputProcedure(&m_model);

  JobContext job(procedure);

  auto on_user_input = [](auto, auto) { return "42"; };
  job.SetUserContext({on_user_input});

  job.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  QTest::qWait(100);

  auto vars_inside = ModelView::Utils::FindItems<LocalVariableItem>(job.GetExpandedModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job.IsRunning());
}

TEST_F(JobContextTest, UserChoiceScenario)
{
  auto procedure = CreateUserChoiceProcedure(&m_model);

  JobContext job(procedure);

  // callback to select Copy instruction
  auto on_user_choice = [](auto, auto) { return 1; };
  job.SetUserContext({{}, on_user_choice});

  job.onPrepareJobRequest();

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 4);

  auto vars_inside = ModelView::Utils::FindItems<LocalVariableItem>(job.GetExpandedModel());
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  EXPECT_FALSE(job.IsRunning());
}
