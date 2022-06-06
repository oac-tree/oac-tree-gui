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

#include "sequencergui/nodeeditor/scene_utils.h"

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/instruction_container_item.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"
#include "sequencergui/model/standard_instruction_items.h"

#include "mvvm/standarditems/container_item.h"
#include "mvvm/utils/numeric_utils.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QPointF>
#include <QRectF>

using namespace sequencergui;

namespace
{
bool operator==(const std::vector<QPointF>& lhs, const std::vector<QPointF>& rhs)
{
  using mvvm::utils::AreAlmostEqual;
  if (lhs.size() != rhs.size())
  {
    return false;
  }
  for (size_t i = 0; i < lhs.size(); ++i)
  {
    if (lhs[i] != rhs[i])
    {
      return false;
    }
  }
  return true;
}

}  // namespace

//! Tests for items from instructionitems.h

class SceneUtilsTest : public ::testing::Test
{
public:
  using points_t = std::vector<QPointF>;

  //! Test instruction playing the role of domain instruction unknown to the GUI.
  class UnknownDomainInstruction : public ::sup::sequencer::Instruction
  {
  public:
    UnknownDomainInstruction() : Instruction(Type) {}

    ::sup::sequencer::ExecutionStatus ExecuteSingleImpl(::sup::sequencer::UserInterface* ui,
                                                        ::sup::sequencer::Workspace* ws) override
    {
      return {};
    }
    static const std::string Type;
  };

  void RegisterUnknownDomainInstruction()
  {
    sup::sequencer::RegisterGlobalInstruction<UnknownDomainInstruction>();
  }
};
const std::string SceneUtilsTest::UnknownDomainInstruction::Type = "UnknownDomainInstruction";

TEST_F(SceneUtilsTest, VectorOfPointsEquality)
{
  using points_t = std::vector<QPointF>;
  EXPECT_TRUE(points_t{} == points_t{});
  EXPECT_FALSE(points_t{QPointF(1.0, 2.0)} == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)}));
  EXPECT_TRUE(points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)})
              == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)}));
  EXPECT_FALSE(points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)})
               == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.01)}));
}

TEST_F(SceneUtilsTest, GetPositions)
{
  const double width{10};
  auto positions = GetPositions(QPointF(1.0, 2.0), 1, width);
  EXPECT_TRUE(positions == points_t({QPointF(1.0, 2.0)}));

  positions = GetPositions(QPointF(1.0, 2.0), 2, width);
  ASSERT_EQ(positions.size(), 2);
  EXPECT_EQ(positions[0], QPointF(-4.0, 2.0));
  EXPECT_EQ(positions[1], QPointF(6.0, 2.0));

  positions = GetPositions(QPointF(1.0, 2.0), 3, width);
  ASSERT_EQ(positions.size(), 3);
  EXPECT_EQ(positions[0], QPointF(-9.0, 2.0));
  EXPECT_EQ(positions[1], QPointF(1.0, 2.0));
  EXPECT_EQ(positions[2], QPointF(11.0, 2.0));
}

TEST_F(SceneUtilsTest, AlignTreeWithSingleInstruction)
{
  InstructionContainerItem container;
  auto instruction = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  AlignTree(QPointF(1, 2), &container);
  EXPECT_FLOAT_EQ(instruction->GetX(), 1.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 2.0);
}

TEST_F(SceneUtilsTest, AlignTreeWithTwoInstructions)
{
  InstructionContainerItem container;
  auto instruction0 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto instruction1 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();

  AlignTree(QPointF(1, 2), &container);
  EXPECT_FLOAT_EQ(instruction0->GetX(), 1.0 - step_width / 2.0);
  EXPECT_FLOAT_EQ(instruction0->GetY(), 2.0);
  EXPECT_FLOAT_EQ(instruction1->GetX(), 1.0 + step_width / 2.0);
  EXPECT_FLOAT_EQ(instruction1->GetY(), 2.0);
}

TEST_F(SceneUtilsTest, AlignTreeWithParentAndChild)
{
  InstructionContainerItem container;
  auto instruction = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();
  const double step_height = GetAlignmentGridHeight();

  AlignTree(QPointF(1, 2), &container);
  EXPECT_FLOAT_EQ(instruction->GetX(), 1.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 2.0);
  EXPECT_FLOAT_EQ(wait->GetX(), 1.0);
  EXPECT_FLOAT_EQ(wait->GetY(), 2.0 + step_height);
}

TEST_F(SceneUtilsTest, AlignTreeWithParentAndTwoChildren)
{
  InstructionContainerItem container;
  auto instruction = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto wait1 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();
  const double step_height = GetAlignmentGridHeight();

  AlignTree(QPointF(1, 2), &container);
  EXPECT_FLOAT_EQ(instruction->GetX(), 1.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 2.0);
  EXPECT_FLOAT_EQ(wait0->GetX(), 1.0 - step_width / 2.);
  EXPECT_FLOAT_EQ(wait0->GetY(), 2.0 + step_height);
  EXPECT_FLOAT_EQ(wait1->GetX(), 1.0 + step_width / 2.);
  EXPECT_FLOAT_EQ(wait1->GetY(), 2.0 + step_height);
}

//! Alignment in force mode when items have already some coordinates

TEST_F(SceneUtilsTest, NonForceAlignTreeWithParentAndTwoChildren)
{
  InstructionContainerItem container;
  auto instruction = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto wait1 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  // giving non-default coordinates
  wait0->SetX(1.1);
  wait0->SetY(1.2);
  wait1->SetX(2.1);
  wait1->SetY(2.2);

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();
  const double step_height = GetAlignmentGridHeight();

  AlignInstructionTree(QPointF(1, 2), instruction, /*force*/ false);
  // position of parent instruction remains unchanged
  EXPECT_FLOAT_EQ(instruction->GetX(), 0.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 0.0);

  // checking that ncoordinates remained unchanged
  EXPECT_FLOAT_EQ(wait0->GetX(), 1.1);
  EXPECT_FLOAT_EQ(wait0->GetY(), 1.2);
  EXPECT_FLOAT_EQ(wait1->GetX(), 2.1);
  EXPECT_FLOAT_EQ(wait1->GetY(), 2.2);
}

//! Alignment in force mode when items have already some coordinates

TEST_F(SceneUtilsTest, ForceAlignTreeWithParentAndTwoChildren)
{
  InstructionContainerItem container;
  auto instruction = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto wait1 = instruction->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  // giving non-default coordinates
  wait0->SetX(1.0);
  wait1->SetX(1.1);
  wait0->SetY(2.1);
  wait1->SetY(2.2);

  // number used in scenutils.cpp
  const double step_width = GetAlignmentGridWidth();
  const double step_height = GetAlignmentGridHeight();

  AlignInstructionTree(QPointF(1, 2), instruction, /*force*/ true);
  // position of parent instruction remains unchanged
  EXPECT_FLOAT_EQ(instruction->GetX(), 0.0);
  EXPECT_FLOAT_EQ(instruction->GetY(), 0.0);

  // checking that non-default coordinates have been overwritten
  EXPECT_FLOAT_EQ(wait0->GetX(), 1.0 - step_width / 2.);
  EXPECT_FLOAT_EQ(wait0->GetY(), 2.0 + step_height);
  EXPECT_FLOAT_EQ(wait1->GetX(), 1.0 + step_width / 2.);
  EXPECT_FLOAT_EQ(wait1->GetY(), 2.0 + step_height);
}

//! Checking adding known domain_type instruction to SequencerModel.

TEST_F(SceneUtilsTest, AddKnownInstruction)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = AddInstruction(&model, procedure->GetInstructionContainer(),
                             domainconstants::kWaitInstructionType);

  EXPECT_EQ(item->GetType(), WaitItem::Type);
  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

//! Checking adding unknown domain_type instruction to the model.

TEST_F(SceneUtilsTest, AddUnknownInstruction)
{
  // Registering domain instruction that will play the role of the instruction unknown to the model
  RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  // adding unknown domain instruction
  auto item =
      AddInstruction(&model, procedure->GetInstructionContainer(), UnknownDomainInstruction::Type);

  // It should result to UnknownInstructionItem with proper DomainType on board
  EXPECT_EQ(item->GetType(), UnknownInstructionItem::Type);
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}
