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

#include "sequencergui/operation/breakpoint_controller.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/operation/breakpoint_helper.h>
#include <sequencergui/transform/gui_object_builder.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/sequencer/application_utils.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/async_runner.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <gtest/gtest.h>
#include <testutils/mock_sequencer_observer.h>
#include <testutils/standard_procedures.h>

using namespace sequencergui;

class BreakpointControllerTest : public ::testing::Test
{
};

//! Testing FindDomainInstruction method.

TEST_F(BreakpointControllerTest, FindDomainInstruction)
{
  WaitItem wait_item;
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  {  // no callback set
    BreakpointController controller({});
    EXPECT_THROW(controller.FindDomainInstruction(wait_item), RuntimeException);
  }

  {  // normal instruction find
    auto find_instruction = [&wait](const auto& item) { return wait.get(); };
    BreakpointController controller(find_instruction);
    EXPECT_EQ(controller.FindDomainInstruction(wait_item), wait.get());
  }
}

//! Testing methods to save and restore breakpoint information in the instruction tree of
//! ProcedureItem.

TEST_F(BreakpointControllerTest, SaveAndRestoreBreakpoints)
{
  BreakpointController controller({});

  {  // saving breakpoint information
    SequencerModel model;
    auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

    auto container = procedure->GetInstructionContainer();
    auto sequence0 = model.InsertItem<SequenceItem>(container);
    auto sequence1 = model.InsertItem<SequenceItem>(container);
    auto wait0 = model.InsertItem<WaitItem>(sequence0);
    auto wait1 = model.InsertItem<WaitItem>(sequence1);

    SetBreakpointStatus(*sequence0, BreakpointStatus::kSet);
    SetBreakpointStatus(*wait0, BreakpointStatus::kSet);
    SetBreakpointStatus(*wait1, BreakpointStatus::kDisabled);

    controller.SaveBreakpoints(*procedure);
  }

  {  // restoring information in parallel hierarchy
    SequencerModel model;
    auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

    auto container = procedure->GetInstructionContainer();
    auto sequence0 = model.InsertItem<SequenceItem>(container);
    auto sequence1 = model.InsertItem<SequenceItem>(container);
    auto wait0 = model.InsertItem<WaitItem>(sequence0);
    auto wait1 = model.InsertItem<WaitItem>(sequence1);

    EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*sequence1), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait0), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait1), BreakpointStatus::kNotSet);

    // setting breakpoints
    controller.RestoreBreakpoints(*procedure);

    // validating breakpoints
    EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSet);
    EXPECT_EQ(GetBreakpointStatus(*sequence1), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait0), BreakpointStatus::kSet);
    EXPECT_EQ(GetBreakpointStatus(*wait1), BreakpointStatus::kDisabled);
  }
}

//! Validating method UpdateDomainBreakpoint. Version which operates on JobController.

TEST_F(BreakpointControllerTest, PropagateBreakpointsToDomain)
{
  testutils::EmptyUserInterface empty_ui;
  sup::sequencer::utils::SimpleJobStateMonitor monitor;

  // building domain and GUI procedures
  GUIObjectBuilder builder;
  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  EXPECT_NO_THROW(procedure->Setup());
  auto procedure_item = builder.CreateProcedureItem(procedure.get(), /*root_only*/ false);

  sup::sequencer::AsyncRunner async_runner(*procedure, empty_ui, monitor);

  // accessing instructions in GUI and domain
  auto sequence = procedure->GetTopInstructions().at(0);
  auto sequence_item = procedure_item->GetInstructionContainer()->GetInstructions().at(0);
  auto message0 = sequence->ChildInstructions().at(0);
  auto message1 = sequence->ChildInstructions().at(1);
  auto message_item0 = sequence_item->GetInstructions().at(0);
  auto message_item1 = sequence_item->GetInstructions().at(1);

  // preparing controller
  auto find_instruction = [&builder](const auto& instruction_item)
  { return builder.FindInstruction(&instruction_item); };
  BreakpointController breakpoint_controller(find_instruction);

  // setting breakpoints in GUI
  SetBreakpointStatus(*message_item1, BreakpointStatus::kSet);

  EXPECT_TRUE(breakpoint_controller.PropagateBreakpointsToDomain(*procedure_item, async_runner));

  // FIXME restore after job_controller fixes GetBreakpoints
  // auto breakpoints = job_controller.GetBreakpoints();
  // ASSERT_EQ(breakpoints.size(), 1);
  // EXPECT_EQ(breakpoints.at(0).GetInstruction(), message1);

  // execution
  async_runner.Start();
  EXPECT_TRUE(monitor.WaitForState(::sup::sequencer::JobState::kPaused, 1));

  EXPECT_EQ(monitor.GetCurrentState(), ::sup::sequencer::JobState::kPaused);
  // EXPECT_FALSE(runner.IsFinished());

  ASSERT_EQ(sup::sequencer::GetNextInstructions(*procedure).size(), 2);
  EXPECT_EQ(sup::sequencer::GetNextInstructions(*procedure).at(0), sequence);
  EXPECT_EQ(sup::sequencer::GetNextInstructions(*procedure).at(1), message1);

  ASSERT_EQ(sup::sequencer::GetNextLeaves(*procedure).size(), 1);
  EXPECT_EQ(sup::sequencer::GetNextLeaves(*procedure).at(0), message1);

  // seems we were lazy to write the test till procedure is completed
}
