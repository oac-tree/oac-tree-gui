/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencergui/operation/breakpoint_controller.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/operation/breakpoint_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

namespace sequencergui::test
{

/**
 * @brief Tests for BreakpointController class.
 */
class BreakpointControllerTest : public ::testing::Test
{
};

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

}  // namespace sequencergui::test
