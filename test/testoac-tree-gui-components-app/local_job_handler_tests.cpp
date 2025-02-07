/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/jobsystem/local_job_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/job_log.h>
#include <oac_tree_gui/jobsystem/job_utils.h>
#include <oac_tree_gui/jobsystem/request_types.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/operation/breakpoint_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/standard_procedure_items.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>

using msec = std::chrono::milliseconds;

namespace oac_tree_gui::test
{

/**
 * @brief Tests for LocalJobHandler class.
 */
class LocalJobHandlerTest : public ::testing::Test
{
public:
  LocalJobHandlerTest()
  {
    m_models.CreateEmpty();
    m_job_item = m_models.GetJobModel()->InsertItem<LocalJobItem>();
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
  LocalJobItem* m_job_item{nullptr};
};

TEST_F(LocalJobHandlerTest, EmptyJobItem)
{
  EXPECT_THROW(LocalJobHandler(nullptr, UserContext{}), RuntimeException);
}

//! Initial JobItem setup.
TEST_F(LocalJobHandlerTest, JobItemWithProcedure)
{
  auto procedure = test::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  const LocalJobHandler job_handler(m_job_item, UserContext{});

  auto expanded_procedure = job_handler.GetExpandedProcedure();
  EXPECT_NE(expanded_procedure, nullptr);
  EXPECT_EQ(job_handler.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(job_handler.GetRunnerStatus(), RunnerStatus::kInitial);
}

//! Creating expanded procedure upfront and populating it with breakpoints. JobItem should preserve
//! breakpoints after initial setup.
TEST_F(LocalJobHandlerTest, PrepareJobRequestBreakpoints)
{
  auto procedure = test::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  // adding expanded procedure to the existing JobItem
  auto expanded_procedure = test::CreateSingleWaitProcedureItem(msec(10));
  auto expanded_procedure_ptr = expanded_procedure.get();
  m_job_item->InsertItem(std::move(expanded_procedure), mvvm::TagIndex::Append());

  // populating it with breakpoints
  auto wait_item = expanded_procedure_ptr->GetInstructionContainer()->GetInstructions().at(0);
  SetBreakpointStatus(*wait_item, BreakpointStatus::kSet);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  // at this point expanded procedure should be regenerated
  auto new_expanded_procedure = job_handler.GetExpandedProcedure();
  EXPECT_NE(new_expanded_procedure, expanded_procedure_ptr);
  EXPECT_EQ(job_handler.GetExpandedProcedure(), m_job_item->GetExpandedProcedure());

  // validating that breakpoints have breen preserved
  auto new_wait_item =
      job_handler.GetExpandedProcedure()->GetInstructionContainer()->GetInstructions().at(0);
  EXPECT_NE(wait_item, new_wait_item);
  EXPECT_EQ(GetBreakpointStatus(*new_wait_item), BreakpointStatus::kSet);

  // toggling breakpoint
  job_handler.OnToggleBreakpointRequest(new_wait_item);
  EXPECT_EQ(GetBreakpointStatus(*new_wait_item), BreakpointStatus::kDisabled);
  job_handler.OnToggleBreakpointRequest(new_wait_item);
  EXPECT_EQ(GetBreakpointStatus(*new_wait_item), BreakpointStatus::kNotSet);
}

//! Attempt to use LocalJobHandler with invalid procedure.
TEST_F(LocalJobHandlerTest, InvalidProcedure)
{
  auto procedure = test::CreateInvalidProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  EXPECT_THROW(LocalJobHandler(m_job_item, UserContext{}),
               sup::oac_tree::InvalidOperationException);
}

//! Delete JobHanlder after procedure start.
TEST_F(LocalJobHandlerTest, PrematureDeletion)
{
  auto procedure = test::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10));
  m_job_item->SetProcedure(procedure);

  {
    LocalJobHandler job_handler(m_job_item, UserContext{});
    job_handler.Start();
  }

  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kUndefined);
}

//! Normal execution of the procedure with single message instruction.
TEST_F(LocalJobHandlerTest, ProcedureWithSingleMessage)
{
  auto procedure = test::CreateMessageProcedureItem(m_models.GetSequencerModel(), "abc");
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kUndefined);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  job_handler.Start();

  auto predicate = [this, &job_handler, &spy_instruction_status]()
  { return !job_handler.IsRunning() && spy_instruction_status.count() == 2; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kSucceeded);
}

//! Normal execution of procedure with single wait. Validating signaling going from expanded
//! procedure (instruction status change).
TEST_F(LocalJobHandlerTest, ProcedureWithSingleMessageStatusChangedSignals)
{
  auto procedure = test::CreateMessageProcedureItem(m_models.GetSequencerModel(), "abc");
  m_job_item->SetProcedure(procedure);

  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kUndefined);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  auto message_item = instructions.at(0);

  mvvm::test::MockItemListener listener(message_item);

  const mvvm::PropertyChangedEvent expected_event{message_item, itemconstants::kStatus};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(2);

  job_handler.Start();

  QTest::qWait(50);

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kSucceeded);
}

TEST_F(LocalJobHandlerTest, ProcedureWithVariableCopy)
{
  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  auto procedure = test::CreateCopyProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto vars = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars.size(), 2);
  auto var0 = vars.at(0);
  auto var1 = vars.at(1);

  EXPECT_TRUE(test::IsEqual(*var0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var1, anyvalue1));

  LocalJobHandler job_handler(m_job_item, UserContext{});

  // expanded procedure has different variables
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue1));

  job_handler.Start();
  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue0));  // value was changed
}

TEST_F(LocalJobHandlerTest, LocalIncludeScenario)
{
  auto procedure = test::CreateIncludeProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  const QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  job_handler.Start();
  // We are testing here queued signals, need special waiting
  QTest::qWait(50);

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 8);  // Repeat, Include, Sequence, Wait x 2

  auto instructions = mvvm::utils::FindItems<WaitItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

TEST_F(LocalJobHandlerTest, UserInputScenario)
{
  // value defined in test::CreateInputProcedureItem
  const sup::dto::AnyValue initial_value{sup::dto::SignedInteger32Type, 0};

  auto procedure = test::CreateInputProcedureItem(m_models.GetSequencerModel());

  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetSequencerModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_TRUE(test::IsEqual(*vars_inside.at(0), initial_value));

  m_job_item->SetProcedure(procedure);

  const sup::dto::AnyValue new_value{sup::dto::SignedInteger32Type, 42};
  auto on_user_input = [new_value](auto) { return UserInputResult{new_value, true}; };

  LocalJobHandler job_handler(m_job_item, UserContext{on_user_input});

  const QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  job_handler.Start();
  QTest::qWait(100);

  EXPECT_EQ(spy_instruction_status.count(), 6);  // 3 instructions

  EXPECT_FALSE(job_handler.IsRunning());

  vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  ASSERT_EQ(vars_inside.size(), 1);
  EXPECT_TRUE(test::IsEqual(*vars_inside.at(0), new_value));
}

TEST_F(LocalJobHandlerTest, UserChoiceScenario)
{
  // value defined in test::CreateUserChoiceProcedureItem
  const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};

  // creating test procedure offering user the choice between two instructions: 0) wait 1) copy
  auto procedure = test::CreateUserChoiceProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  // callback to select Copy instruction
  auto on_user_choice = [](auto) { return UserChoiceResult{1, true}; };

  LocalJobHandler job_handler(m_job_item, {{}, on_user_choice});

  const QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  job_handler.Start();

  auto predicate = [this, &expected_anyvalue]()
  {
    auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
    return test::IsEqual(*vars_inside.at(1), expected_anyvalue);
  };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 100));

  EXPECT_EQ(spy_instruction_status.count(), 4);

  // validating that the copy instruction worked, i.e. that is has successfully copied var0 into
  // var1
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  EXPECT_TRUE(test::IsEqual(*vars_inside.at(1), expected_anyvalue));
  EXPECT_FALSE(job_handler.IsRunning());
}

//! Stop long running job.
TEST_F(LocalJobHandlerTest, StopLongRunningJob)
{
  auto procedure = test::CreateSingleWaitProcedureItem(m_models.GetSequencerModel(), msec(10000));
  m_job_item->SetProcedure(procedure);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  job_handler.Start();

  EXPECT_TRUE(QTest::qWaitFor(
      [&spy_instruction_status]() { return spy_instruction_status.count() == 1; }, 100));

  EXPECT_TRUE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 1);

  job_handler.Stop();

  EXPECT_TRUE(QTest::qWaitFor(
      [&spy_instruction_status]() { return spy_instruction_status.count() == 2; }, 100));

  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);
}

//! Control log events with the help of MessageInstruction.
TEST_F(LocalJobHandlerTest, LogEvents)
{
  const std::string expected_message("abc");
  auto procedure = test::CreateMessageProcedureItem(m_models.GetSequencerModel(), expected_message);
  m_job_item->SetProcedure(procedure);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  const QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  // Put something in the log, to validate that it will be cleaned up before job start
  job_handler.GetJobLog()->Append({});

  job_handler.Start();
  QTest::qWait(50);
  //  EXPECT_TRUE(QTest::qWaitFor([&job_handler]() { return !job_handler.IsRunning(); }, 10));

  EXPECT_FALSE(job_handler.IsRunning());

  EXPECT_EQ(spy_instruction_status.count(), 2);

  auto instructions = FindExpandedInstructions(domainconstants::kMessageInstructionType);
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");

  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kSucceeded);

  ASSERT_EQ(job_handler.GetJobLog()->GetSize(), 1);

  auto event = job_handler.GetJobLog()->At(0);
  EXPECT_EQ(event.severity, Severity::kInfo);
  EXPECT_EQ(event.message, expected_message);
}

TEST_F(LocalJobHandlerTest, ProcedureWithResetVariableInstruction)
{
  auto procedure = test::CreateVariableResetProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  LocalJobHandler job_handler(m_job_item, UserContext{});

  // expanded procedure has different variables
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);
  auto var_inside2 = vars_inside.at(2);

  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::StringType, "abc"};

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue1));

  job_handler.Start();
  // We are testing here queued signals, need special waiting
  EXPECT_TRUE(QTest::qWaitFor(
      [&var_inside2, anyvalue1]() { return test::IsEqual(*var_inside2, anyvalue1); }, 100));

  EXPECT_FALSE(job_handler.IsRunning());

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue1));
  EXPECT_TRUE(test::IsEqual(*var_inside2, anyvalue1));  // value was changed
}

//! Testing propagation of breakpoints to the domain.
TEST_F(LocalJobHandlerTest, SetBreakpoint)
{
  auto procedure = test::CreateIncrementProcedureItem(m_models.GetSequencerModel());
  m_job_item->SetProcedure(procedure);

  auto instructions = mvvm::utils::FindItems<InstructionItem>(m_models.GetSequencerModel());
  ASSERT_EQ(instructions.size(), 3);  // sequence, increment0, increment1

  LocalJobHandler job_handler(m_job_item, UserContext{});

  // expanded procedure has different variables and instructions
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(m_models.GetJobModel());
  auto instructions_inside = mvvm::utils::FindItems<InstructionItem>(m_models.GetJobModel());
  ASSERT_EQ(vars_inside.size(), 2);          // var0, var1
  ASSERT_EQ(instructions_inside.size(), 3);  // sequence, increment0, increment1

  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);

  {  // initial values in expanded procedure
    const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 0};
    const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 10};
    EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
    EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue1));
  }

  // breakpoint on second increment will make it stop just before
  job_handler.OnToggleBreakpointRequest(instructions_inside.at(2));
  EXPECT_EQ(GetBreakpointStatus(*instructions_inside.at(2)), BreakpointStatus::kSet);

  job_handler.Start();

  auto predicate = [&job_handler]()
  { return job_handler.GetRunnerStatus() == RunnerStatus::kPaused; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 50));

  {  // values when on pause
    const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 1};
    const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 10};
    EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));  // value incremented
    EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue1));  // same as before
  }

  // run till the end
  job_handler.Start();

  auto predicate2 = [&job_handler]()
  { return job_handler.GetRunnerStatus() == RunnerStatus::kSucceeded; };
  EXPECT_TRUE(QTest::qWaitFor(predicate2, 50));

  {  // values at the end
    const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 1};
    const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 11};
    EXPECT_TRUE(test::IsEqual(*vars_inside.at(0), anyvalue0));

    // it takes time to propagate values to the domain
    auto predicate3 = [&vars_inside, &anyvalue1]()
    { return test::IsEqual(*vars_inside.at(1), anyvalue1); };
    EXPECT_TRUE(QTest::qWaitFor(predicate3, 50));
  }
}

}  // namespace oac_tree_gui::test
