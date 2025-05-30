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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/align_node.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QPointF>

#include "oac_tree_gui/nodeeditor/sequencer_align_utils.h"

namespace oac_tree_gui::test
{

//! Tests for SequencerObserver class.

class SequencerAlignUtilsTest : public ::testing::Test
{
};

//! Validate CreateAlignTree method on empty container.

TEST_F(SequencerAlignUtilsTest, CreateAlignTreeFromContainerEmptyContainer)
{
  InstructionContainerItem container;
  auto node = algorithm::CreateAlignTree(container.GetInstructions());

  EXPECT_EQ(node->GetSize(), 0);
}

//! Validate CreateAlignTree method using sequence with 3 children.

TEST_F(SequencerAlignUtilsTest, CreateAlignTreeFromContainerSequenceWithChildren)
{
  // creating test instruction tree
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto sequence = model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  auto wait2 = model.InsertItem<WaitItem>(sequence);

  // creating corresponding AlignTree
  auto node = algorithm::CreateAlignTree(procedure->GetInstructionContainer()->GetInstructions());

  // root node
  EXPECT_EQ(node->GetSize(), 1);
  EXPECT_TRUE(node->GetIdentifier().empty());

  // node corresponding to sequence
  auto sequence_node = node->GetChildren().at(0);
  EXPECT_EQ(sequence_node->GetSize(), 3);
  EXPECT_EQ(sequence_node->GetIdentifier(), sequence->GetIdentifier());

  // nodes corresponding to wait instructions
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

//! Validate CreateAlignTree method using sequence with 3 children.

TEST_F(SequencerAlignUtilsTest, CreateAlignTreeSequenceWithChildren)
{
  // creating test instruction tree
  SequencerModel model;
  auto sequence = model.InsertItem<SequenceItem>();
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  auto wait2 = model.InsertItem<WaitItem>(sequence);

  // creating corresponding AlignTree
  auto sequence_node = algorithm::CreateAlignTree(sequence);

  // node corresponding to sequence
  EXPECT_EQ(sequence_node->GetSize(), 3);
  EXPECT_EQ(sequence_node->GetIdentifier(), sequence->GetIdentifier());

  // nodes corresponding to wait instructions
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

TEST_F(SequencerAlignUtilsTest, UpdatePositionsInTheContainer)
{
  // creating test instruction tree
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto sequence = model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  auto wait2 = model.InsertItem<WaitItem>(sequence);

  // creating corresponding AlignTree
  auto instructions = procedure->GetInstructionContainer()->GetInstructions();
  auto node = algorithm::CreateAlignTree(instructions);

  // manually setting  coordinates to nodes
  auto sequence_node = node->GetChildren().at(0);
  sequence_node->SetPos(1.0, 2.0);
  auto wait0_node = sequence_node->GetChildren().at(0);
  wait0_node->SetPos(3.0, 4.0);
  auto wait1_node = sequence_node->GetChildren().at(1);
  wait1_node->SetPos(5.0, 6.0);
  auto wait2_node = sequence_node->GetChildren().at(2);
  wait2_node->SetPos(7.0, 8.0);

  // updating positions of the instruction tree from the AlignmentTree
  algorithm::UpdatePositions(node.get(), procedure->GetInstructionContainer()->GetInstructions());

  // validationg coordinates of instructions
  EXPECT_EQ(sequence->GetX(), 1.0);
  EXPECT_EQ(sequence->GetY(), 2.0);
  EXPECT_EQ(wait0->GetX(), 3.0);
  EXPECT_EQ(wait0->GetY(), 4.0);
  EXPECT_EQ(wait1->GetX(), 5.0);
  EXPECT_EQ(wait1->GetY(), 6.0);
  EXPECT_EQ(wait2->GetX(), 7.0);
  EXPECT_EQ(wait2->GetY(), 8.0);
}

TEST_F(SequencerAlignUtilsTest, UpdatePositionsOfInstructionTree)
{
  // creating test instruction tree
  SequencerModel model;
  auto sequence = model.InsertItem<SequenceItem>();
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);
  auto wait2 = model.InsertItem<WaitItem>(sequence);

  // creating corresponding AlignTree
  auto sequence_node = algorithm::CreateAlignTree(sequence);

  // manually setting  coordinates to nodes
  sequence_node->SetPos(1.0, 2.0);
  auto wait0_node = sequence_node->GetChildren().at(0);
  wait0_node->SetPos(3.0, 4.0);
  auto wait1_node = sequence_node->GetChildren().at(1);
  wait1_node->SetPos(5.0, 6.0);
  auto wait2_node = sequence_node->GetChildren().at(2);
  wait2_node->SetPos(7.0, 8.0);

  // updating positions of the instruction tree from the AlignmentTree
  algorithm::UpdatePositions(sequence_node.get(), sequence);

  // validationg coordinates of instructions
  EXPECT_EQ(sequence->GetX(), 1.0);
  EXPECT_EQ(sequence->GetY(), 2.0);
  EXPECT_EQ(wait0->GetX(), 3.0);
  EXPECT_EQ(wait0->GetY(), 4.0);
  EXPECT_EQ(wait1->GetX(), 5.0);
  EXPECT_EQ(wait1->GetY(), 6.0);
  EXPECT_EQ(wait2->GetX(), 7.0);
  EXPECT_EQ(wait2->GetY(), 8.0);
}

TEST_F(SequencerAlignUtilsTest, AlignTreeWithSingleInstruction)
{
  SequencerModel model;
  auto instruction = model.InsertItem<SequenceItem>();
  algorithm::AlignInstructionTreeWalker(QPointF(1, 2), instruction);
  EXPECT_FLOAT_EQ(instruction->GetX(), 1.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 2.0);
}

TEST_F(SequencerAlignUtilsTest, AlignTreeWithTwoInstructions)
{
  SequencerModel model;
  auto sequence = model.InsertItem<SequenceItem>();

  auto instruction0 = sequence->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto instruction1 = sequence->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();
  const double step_height = GetAlignmentGridHeight();

  algorithm::AlignInstructionTreeWalker(QPointF(1, 2), sequence);

  EXPECT_FLOAT_EQ(sequence->GetX(), 1.0);
  EXPECT_FLOAT_EQ(sequence->GetY(), 2.0);

  EXPECT_FLOAT_EQ(instruction0->GetX(), 1.0 - step_width / 2.0);
  EXPECT_FLOAT_EQ(instruction0->GetY(), 2.0 + step_height);
  EXPECT_FLOAT_EQ(instruction1->GetX(), 1.0 + step_width / 2.0);
  EXPECT_FLOAT_EQ(instruction1->GetY(), 2.0 + step_height);
}

TEST_F(SequencerAlignUtilsTest, RequiresInitialAlignment)
{
  SequenceItem item;
  EXPECT_TRUE(algorithm::RequiresInitialAlignment(item));
  EXPECT_TRUE(algorithm::RequiresInitialAlignment({&item}));
  EXPECT_TRUE(algorithm::RequiresInitialAlignment({&item, &item}));

  item.SetX(42.0);
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(item));
  EXPECT_FALSE(algorithm::RequiresInitialAlignment({&item, &item}));
}

}  // namespace oac_tree_gui::test
