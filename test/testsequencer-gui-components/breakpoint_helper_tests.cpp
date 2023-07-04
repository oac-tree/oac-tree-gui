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

#include "sequencergui/operation/breakpoint_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/model/model_utils.h>

#include <gtest/gtest.h>

using namespace sequencergui;

class BreakpointHelperTest : public ::testing::Test
{
};

TEST_F(BreakpointHelperTest, GetBreakpointStatus)
{
  SequenceItem item;
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kNotSet);

  SetBreakpointStatus(item, BreakpointStatus::kSet);
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kSet);

  SetBreakpointStatus(item, BreakpointStatus::kDisabled);
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kDisabled);
}

TEST_F(BreakpointHelperTest, ToggleBreakpointStatus)
{
  SequenceItem item;

  ToggleBreakpointStatus(item);
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kSet);

  ToggleBreakpointStatus(item);
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kDisabled);

  ToggleBreakpointStatus(item);
  EXPECT_EQ(GetBreakpointStatus(item), BreakpointStatus::kNotSet);
}

TEST_F(BreakpointHelperTest, CollectBreakpointInfo)
{
  // item should be part of the model
  SequenceItem item;
  EXPECT_THROW(CollectBreakpointInfo(item), LogicErrorException);

  SequencerModel model;

  // sequence without breakpoints
  auto sequence = model.InsertItem<SequenceItem>();
  EXPECT_TRUE(CollectBreakpointInfo(*sequence).empty());

  // breakpoint set to sequence
  SetBreakpointStatus(*sequence, BreakpointStatus::kSet);
  auto info = CollectBreakpointInfo(*sequence);
  ASSERT_EQ(info.size(), 1);
  EXPECT_EQ(info.at(0).status, BreakpointStatus::kSet);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(0).path), sequence);

  // adding a child with breakpoint
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  SetBreakpointStatus(*wait1, BreakpointStatus::kDisabled);
  info = CollectBreakpointInfo(*sequence);
  ASSERT_EQ(info.size(), 2);
  EXPECT_EQ(info.at(0).status, BreakpointStatus::kSet);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(0).path), sequence);
  EXPECT_EQ(info.at(1).status, BreakpointStatus::kDisabled);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(1).path), wait1);
}

TEST_F(BreakpointHelperTest, CollectBreakpointInfoFromContainer)
{
  SequencerModel model;

  auto container = model.InsertItem<InstructionContainerItem>();

  // sequence without breakpoints
  auto sequence0 = model.InsertItem<SequenceItem>(container);
  auto sequence1 = model.InsertItem<SequenceItem>(container);
  auto wait0 = model.InsertItem<WaitItem>(sequence0);
  auto wait1 = model.InsertItem<WaitItem>(sequence1);

  SetBreakpointStatus(*sequence0, BreakpointStatus::kSet);
  SetBreakpointStatus(*wait0, BreakpointStatus::kSet);
  SetBreakpointStatus(*wait1, BreakpointStatus::kDisabled);

  auto info = CollectBreakpointInfo(*container);

  ASSERT_EQ(info.size(), 3);
  EXPECT_EQ(info.at(0).status, BreakpointStatus::kSet);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(0).path), sequence0);
  EXPECT_EQ(info.at(1).status, BreakpointStatus::kSet);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(1).path), wait0);
  EXPECT_EQ(info.at(2).status, BreakpointStatus::kDisabled);
  EXPECT_EQ(mvvm::utils::ItemFromPath(model, info.at(2).path), wait1);
}

TEST_F(BreakpointHelperTest, SetBreakpointFromInfo)
{
  std::vector<BreakpointInfo> info;

  {
    SequencerModel model;

    auto container = model.InsertItem<InstructionContainerItem>();

    // sequence without breakpoints
    auto sequence0 = model.InsertItem<SequenceItem>(container);
    auto sequence1 = model.InsertItem<SequenceItem>(container);
    auto wait0 = model.InsertItem<WaitItem>(sequence0);
    auto wait1 = model.InsertItem<WaitItem>(sequence1);

    SetBreakpointStatus(*sequence0, BreakpointStatus::kSet);
    SetBreakpointStatus(*wait0, BreakpointStatus::kSet);
    SetBreakpointStatus(*wait1, BreakpointStatus::kDisabled);

    info = CollectBreakpointInfo(*container);
  }

  {  // creating second hierarchy
    SequencerModel model;

    auto container = model.InsertItem<InstructionContainerItem>();
    auto sequence0 = model.InsertItem<SequenceItem>(container);
    auto sequence1 = model.InsertItem<SequenceItem>(container);
    auto wait0 = model.InsertItem<WaitItem>(sequence0);
    auto wait1 = model.InsertItem<WaitItem>(sequence1);

    EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*sequence1), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait0), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait1), BreakpointStatus::kNotSet);

    // setting breakpoints
    SetBreakpointsFromInfo(info, *container);

    // validating breakpoints
    EXPECT_EQ(GetBreakpointStatus(*sequence0), BreakpointStatus::kSet);
    EXPECT_EQ(GetBreakpointStatus(*sequence1), BreakpointStatus::kNotSet);
    EXPECT_EQ(GetBreakpointStatus(*wait0), BreakpointStatus::kSet);
    EXPECT_EQ(GetBreakpointStatus(*wait1), BreakpointStatus::kDisabled);
  }

  {  // it is not possible to set info if it is not part of the model
    InstructionContainerItem container;

    // sequence without breakpoints
    auto sequence0 = container.InsertItem<SequenceItem>({});

    EXPECT_THROW(SetBreakpointsFromInfo(info, container), LogicErrorException);
  }

  {  // creating uncomplete hierarchy
    SequencerModel model;

    auto container = model.InsertItem<InstructionContainerItem>();
    auto sequence0 = model.InsertItem<SequenceItem>(container);
    auto wait0 = model.InsertItem<WaitItem>(sequence0);

    // our structure doesn't coincide with the original structure from where breakpoints have been
    // taken
    EXPECT_THROW(SetBreakpointsFromInfo(info, *container), RuntimeException);
  }
}
