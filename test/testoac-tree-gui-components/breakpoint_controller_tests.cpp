/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/operation/breakpoint_controller.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/operation/breakpoint_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
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

TEST_F(BreakpointControllerTest, SetAsActiveBreakpoint)
{
  BreakpointController controller({});

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto container = procedure->GetInstructionContainer();
  auto sequence0 = model.InsertItem<SequenceItem>(container);
  auto sequence1 = model.InsertItem<SequenceItem>(container);
  auto wait0 = model.InsertItem<WaitItem>(sequence0);
  auto wait1 = model.InsertItem<WaitItem>(sequence1);

  EXPECT_NO_THROW(controller.SetAsActiveBreakpoint(sequence0));

  controller.SetAsActiveBreakpoint(sequence0);
  EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSetAndHit);

  controller.ResetCurrentActiveBreakpoint();
  EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSet);

  // second time doesn't change anything
  controller.ResetCurrentActiveBreakpoint();
  EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSet);

  // trying to set several active breakpoints
  EXPECT_NO_THROW(controller.SetAsActiveBreakpoint(sequence0));
  EXPECT_NO_THROW(controller.SetAsActiveBreakpoint(wait0));

  EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSet);
  EXPECT_EQ(GetBreakpointStatus(*wait0), BreakpointStatus::kSetAndHit);
}

}  // namespace oac_tree_gui::test
