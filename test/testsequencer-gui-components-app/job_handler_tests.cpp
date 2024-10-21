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

#include "sequencergui/jobsystem/job_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/job_log.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/request_types.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/operation/breakpoint_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobHandler class.

class JobHandlerTest : public ::testing::Test
{
public:
  JobHandlerTest()
  {
    m_models.CreateNewProject();
    m_job_item = m_models.GetJobModel()->InsertItem<JobItem>();
  }

  /**
   * @brief Finds UniversalInstructionItem representing given domain type.
   *
   * Search is performed in JobModel, where JobHandler builds expanded procedure).
   */
  std::vector<UniversalInstructionItem*> FindExpandedInstructions(const std::string& domain_type)
  {
    std::vector<UniversalInstructionItem*> result;
    auto candidates = mvvm::utils::FindItems<UniversalInstructionItem>(m_models.GetJobModel());
    for (auto universal_instruction : candidates)
    {
      if (universal_instruction->GetDomainType() == domain_type)
      {
        result.push_back(universal_instruction);
      }
    }
    return result;
  }

  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

TEST_F(JobHandlerTest, EmptyJobItem)
{
  EXPECT_THROW(JobHandler(nullptr), RuntimeException);
}

//! Initial JobItem setup.

TEST_F(JobHandlerTest, JobItemWithProcedure)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  JobHandler job_handler(m_job_item);

  auto expanded_procedure = job_handler.GetExpandedProcedure();
  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_EQ(job_handler.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(job_handler.GetRunnerStatus(), RunnerStatus::kInitial);
}

//! Creating expanded procedure upfront and populating it with breakpoints.
//! JobItem should preserve breakpoints after initial setup.

TEST_F(JobHandlerTest, PrepareJobRequestBreakpoints)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  // adding expanded procedure to the existing JobItem
  auto expanded_procedure = testutils::CreateSingleWaitProcedureItem(msec(10));
  auto expanded_procedure_ptr = expanded_procedure.get();
  m_job_item->InsertItem(std::move(expanded_procedure), mvvm::TagIndex::Append());

  // populating it with breakpoints
  auto wait_item = expanded_procedure_ptr->GetInstructionContainer()->GetInstructions().at(0);
  SetBreakpointStatus(*wait_item, BreakpointStatus::kSet);

  JobHandler job_handler(m_job_item);

  // at this point expanded procedure should be regenerated
  auto new_expanded_procedure = job_handler.GetExpandedProcedure();
  EXPECT_NE(new_expanded_procedure, expanded_procedure_ptr);
  EXPECT_EQ(job_handler.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());

  // validating that breakpoints have breen preserved
  auto new_wait_item =
      job_handler.GetExpandedProcedure()->GetInstructionContainer()->GetInstructions().at(0);
  EXPECT_NE(wait_item, new_wait_item);
  EXPECT_EQ(GetBreakpointStatus(*new_wait_item), BreakpointStatus::kSet);
}

//! Attempt to use JobHandler with invalid procedure.

TEST_F(JobHandlerTest, InvalidProcedure)
{
  auto procedure = testutils::CreateInvalidProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  EXPECT_THROW(JobHandler(m_job_item, {}), sup::sequencer::InvalidOperationException);
}

//! Delete JobHanlder after procedure start.

TEST_F(JobHandlerTest, PrematureDeletion)
{
  auto procedure = testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  {
    JobHandler job_handler(m_job_item);
    job_handler.OnStartRequest();
  }

  EXPECT_EQ(m_job_item->GetStatus(), std::string());
}

//! Normal execution of the procedure with single message instruction.

TEST_F(JobHandlerTest, ProcedureWithSingleMessage)
{
  auto procedure = testutils::CreateMessageProcedureItem(m_models.GetSequencerModel(), "abc");
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobHandler job_handler(m_job_item);

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();

  auto predicate = [this, &job_handler, &spy_instruction_status]()
  { return !job_handler.IsRunning() && spy_instruction_status.count() == 2; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kSucceeded);
}

//! Normal execution of procedure with single wait.
//! Validating signaling going from expanded procedure (instruction status change).

TEST_F(JobHandlerTest, ProcedureWithSingleMessageStatusChangedSignals)
{
  auto procedure = testutils::CreateMessageProcedureItem(m_models.GetSequencerModel(), "abc");
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), std::string());

  JobHandler job_handler(m_job_item);

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  auto message_item = instructions.at(0);

  mvvm::test::MockItemListener listener(message_item);

  mvvm::PropertyChangedEvent expected_event{message_item, itemconstants::kStatus};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(2);

  job_handler.OnStartRequest();

  QTest::qWait(50);

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kSucceeded);
}

TEST_F(JobHandlerTest, ProcedureWithVariableCopy)
{
  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  auto procedure = testutils::CreateCopyProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto vars = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars.size(), 2);

  auto initial_anyvalue_item0 = vars.at(0)->GetAnyValueItem();
  auto initial_anyvalue_item1 = vars.at(1)->GetAnyValueItem();

  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item1), anyvalue1);

  JobHandler job_handler(m_job_item);

  // expanded procedure has different variables
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto new_anyvalue_item0 = vars_inside.at(0)->GetAnyValueItem();
  auto new_anyvalue_item1 = vars_inside.at(1)->GetAnyValueItem();

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue1);

  job_handler.OnStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue0);  // value was changed
}

TEST_F(JobHandlerTest, LocalIncludeScenario)
{
  auto procedure = testutils::CreateIncludeProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobHandler job_handler(m_job_item);

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 8);  // Repeat, Include, Sequence, Wait x 2

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

TEST_F(JobHandlerTest, UserInputScenario)
{
  // value defined in testutils::CreateInputProcedureItem
  const sup::dto::AnyValue initial_value{sup::dto::SignedInteger32Type, 0};

  auto procedure = testutils::CreateInputProcedureItem(m_models.GetSequencerModel());

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_EQ(sup::gui::CreateAnyValue(*vars_inside.at(0)->GetAnyValueItem()), initial_value);

  m_job_item->SetProcedure(procedure);

  const sup::dto::AnyValue new_value{sup::dto::SignedInteger32Type, 42};
  auto on_user_input = [new_value](auto) { return UserInputResult{new_value, true}; };

  JobHandler job_handler(m_job_item, {on_user_input});

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();
  QTest::qWait(50);

  EXPECT_EQ(spy_instruction_status.count(), 6);  // 3 instructions

  EXPECT_FALSE(job_handler.IsRunning());

  vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_EQ(sup::gui::CreateAnyValue(*vars_inside.at(0)->GetAnyValueItem()), new_value);
}

TEST_F(JobHandlerTest, UserChoiceScenario)
{
  // value defined in testutils::CreateUserChoiceProcedureItem
  const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};

  // creating test procedure offering user the choice between two instructions: 0) wait 1) copy
  auto procedure = testutils::CreateUserChoiceProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  // callback to select Copy instruction
  auto on_user_choice = [](auto) { return UserChoiceResult{1, true}; };

  JobHandler job_handler(m_job_item, {{}, on_user_choice});

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();
  QTest::qWait(50);

  EXPECT_EQ(spy_instruction_status.count(), 4);

  // validating that the copy instruction worked, i.e. that is has successfully copied var0 into
  // var1
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto anyvalue_item = vars_inside.at(1)->GetAnyValueItem();
  EXPECT_EQ(sup::gui::CreateAnyValue(*anyvalue_item), expected_anyvalue);
  EXPECT_FALSE(job_handler.IsRunning());
}

//! Stop long running job.

TEST_F(JobHandlerTest, StopLongRunningJob)
{
  auto procedure =
      testutils::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10000));
  m_job_item->SetProcedure(procedure);

  JobHandler job_handler(m_job_item);

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();

  EXPECT_TRUE(QTest::qWaitFor(
      [&spy_instruction_status]() { return spy_instruction_status.count() == 1; }, 100));

  EXPECT_TRUE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 1);

  job_handler.OnStopRequest();

  EXPECT_TRUE(QTest::qWaitFor(
      [&spy_instruction_status]() { return spy_instruction_status.count() == 2; }, 100));

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);
}

//! Control log events with the help of MessageInstruction.

TEST_F(JobHandlerTest, LogEvents)
{
  const std::string expected_message("abc");
  auto procedure =
      testutils::CreateMessageProcedureItem(m_models.GetSequencerModel(), expected_message);
  m_job_item->SetProcedure(procedure);

  JobHandler job_handler(m_job_item);

  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  job_handler.OnStartRequest();
  QTest::qWait(50);
  //  EXPECT_TRUE(QTest::qWaitFor([&job_handler]() { return !job_handler.IsRunning(); }, 10));

  EXPECT_FALSE(job_handler.IsRunning());

  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kSucceeded);

  ASSERT_EQ(job_handler.GetJobLog()->GetSize(), 1);

  auto event = job_handler.GetJobLog()->At(0);
  EXPECT_EQ(event.severity, Severity::kInfo);
  EXPECT_EQ(event.message, expected_message);
}

TEST_F(JobHandlerTest, ProcedureWithResetVariableInstruction)
{
  auto procedure = testutils::CreateVariableResetProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  JobHandler job_handler(m_job_item);

  // expanded procedure has different variables
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto new_anyvalue_item0 = vars_inside.at(0)->GetAnyValueItem();
  auto new_anyvalue_item1 = vars_inside.at(1)->GetAnyValueItem();
  auto new_anyvalue_item2 = vars_inside.at(2)->GetAnyValueItem();

  ASSERT_NE(new_anyvalue_item0, nullptr);
  ASSERT_NE(new_anyvalue_item1, nullptr);
  ASSERT_EQ(new_anyvalue_item2, nullptr);

  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::StringType, "abc"};

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue1);

  job_handler.OnStartRequest();
  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_FALSE(job_handler.IsRunning());

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue1);

  // AnyValueItem on board was regenerated
  auto regenerated_anyvalue_item2 = vars_inside.at(2)->GetAnyValueItem();
  EXPECT_EQ(sup::gui::CreateAnyValue(*regenerated_anyvalue_item2), anyvalue1);  // value was changed
}
