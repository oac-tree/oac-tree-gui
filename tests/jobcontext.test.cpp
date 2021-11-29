/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/jobcontext.h"

#include "Instruction.h"
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
  JobContextTest() {}

  using test_data_t = std::tuple<std::unique_ptr<SequencerModel>, ProcedureItem*>;

  test_data_t CreateSingleWaitProcedure()
  {
    auto model = std::make_unique<SequencerModel>();
    auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto wait = model->InsertItem<WaitItem>(procedure_item->GetInstructionContainer());
    wait->SetTimeout(0.01);
    return std::make_tuple(std::move(model), procedure_item);
  }

  test_data_t CreateCopyProcedure()
  {
    auto model = std::make_unique<SequencerModel>();
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

    return std::make_tuple(std::move(model), procedure_item);
  }
};

//! Normal execution of the procedure with single wait instruction.

TEST_F(JobContextTest, PrematureDeletion)
{
  auto [model, procedure] = CreateSingleWaitProcedure();

  {
    JobContext job(model.get(), procedure);
    job.onStartRequest();
  }

  // FIXME add tests for ProcedureItem status
}

TEST_F(JobContextTest, ProcedureWithSingleWait)
{
  auto [model, procedure] = CreateSingleWaitProcedure();

  JobContext job(model.get(), procedure);

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = ModelView::Utils::FindItems<WaitItem>(model.get());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Not started");
}

TEST_F(JobContextTest, ProcedureWithVariableCopy)
{
  auto [model, procedure] = CreateCopyProcedure();

  auto vars = ModelView::Utils::FindItems<LocalVariableItem>(model.get());
  ASSERT_EQ(vars.size(), 2);
  EXPECT_EQ(vars.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars.at(1)->GetJsonValue(), std::string("43"));

  JobContext job(model.get(), procedure);

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(vars.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars.at(1)->GetJsonValue(), std::string("42"));
}

TEST_F(JobContextTest, ExecutionInStepMode)
{
  auto [model, procedure] = CreateSingleWaitProcedure();

  JobContext job(model.get(), procedure);
  job.SetWaitingMode(WaitingMode::kWaitForRelease);

  QSignalSpy spy_instruction_status(&job, &JobContext::InstructionStatusChanged);

  job.onStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

  EXPECT_TRUE(job.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 1);

  job.onMakeStepRequest();
  QTest::qWait(50);

  EXPECT_TRUE(job.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  job.onMakeStepRequest();
  QTest::qWait(50);

  EXPECT_FALSE(job.IsRunning());
}

