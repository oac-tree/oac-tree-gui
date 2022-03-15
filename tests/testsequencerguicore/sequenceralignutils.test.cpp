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

#include "sequencergui/algorithm/sequenceralignutils.h"

#include "sequencergui/algorithm/alignnode.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/standardinstructionitems.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class SequencerAlignUtilsTest : public ::testing::Test
{
};

TEST_F(SequencerAlignUtilsTest, CreateAlignTreeEmptyContainer)
{
  InstructionContainerItem container;
  auto node = algorithm::CreateAlignTree(&container);

  EXPECT_EQ(node->GetSize(), 0);
}

TEST_F(SequencerAlignUtilsTest, CreateAlignTreeSequenceWithChildren)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto sequence = model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  auto wait2 = model.InsertItem<WaitItem>(sequence);

  auto node = algorithm::CreateAlignTree(procedure->GetInstructionContainer());

  // root node
  EXPECT_EQ(node->GetSize(), 1);
  EXPECT_TRUE(node->GetIdentifier().empty());

  // node corresponding to sequence
  auto sequence_node = node->GetChildren().at(0);
  EXPECT_EQ(sequence_node->GetSize(), 3);
  EXPECT_EQ(sequence_node->GetIdentifier(), sequence->GetIdentifier());

  auto wait0_node = sequence_node->GetChildren().at(0);
  auto wait1_node = sequence_node->GetChildren().at(1);
  auto wait2_node = sequence_node->GetChildren().at(2);

  EXPECT_EQ(wait0_node->GetSize(), 0);
  EXPECT_EQ(wait0_node->GetIdentifier(), wait0->GetIdentifier());
  EXPECT_EQ(wait1_node->GetSize(), 0);
  EXPECT_EQ(wait1_node->GetIdentifier(), wait1->GetIdentifier());
  EXPECT_EQ(wait2_node->GetSize(), 0);
  EXPECT_EQ(wait2_node->GetIdentifier(), wait2->GetIdentifier());
}
