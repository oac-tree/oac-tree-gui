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

#include "sequencergui/nodeeditor/scene_utils.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/numeric_utils.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>

#include <gtest/gtest.h>

#include <QDebug>
#include <QPointF>
#include <QRectF>

#include <iostream>

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

    ::sup::sequencer::ExecutionStatus ExecuteSingleImpl(::sup::sequencer::UserInterface& ui,
                                                        ::sup::sequencer::Workspace& ws) override
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

//! Checking adding known domain_type instruction to SequencerModel.
//! This validates drag-and-drop behavior.

TEST_F(SceneUtilsTest, AddKnownInstruction)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = AddSingleInstruction(&model, procedure->GetInstructionContainer(),
                                   domainconstants::kWaitInstructionType);

  EXPECT_EQ(item->GetType(), WaitItem::Type);
  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

//! Checking adding unknown domain_type instruction to SequencerModel.
//! This validates drag-and-drop behavior.

TEST_F(SceneUtilsTest, AddUnknownInstruction)
{
  RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = AddSingleInstruction(&model, procedure->GetInstructionContainer(),
                                   UnknownDomainInstruction::Type);

  EXPECT_EQ(item->GetType(), UniversalInstructionItem::Type);
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

//! Checking adding instruction aggregate.

TEST_F(SceneUtilsTest, AddAggregate)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = AddAggregate(&model, procedure->GetInstructionContainer(), "if-then-else");

  EXPECT_EQ(item->GetType(), FallbackItem::Type);
}

TEST_F(SceneUtilsTest, InsertSpaceAtCamelCase)
{
  EXPECT_EQ(InsertSpaceAtCamelCase(""), std::string(""));
  EXPECT_EQ(InsertSpaceAtCamelCase(" "), std::string(" "));
  EXPECT_EQ(InsertSpaceAtCamelCase("abc"), std::string("abc"));
  EXPECT_EQ(InsertSpaceAtCamelCase("abc def"), std::string("abc def"));
  EXPECT_EQ(InsertSpaceAtCamelCase("Access"), std::string("Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("PVAccess"), std::string("PVAccess"));

  EXPECT_EQ(InsertSpaceAtCamelCase("ChannelAccess"), std::string("Channel Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("PvAccess"), std::string("Pv Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("NoCamelCase"), std::string("No Camel Case"));
  EXPECT_EQ(InsertSpaceAtCamelCase("ChannelAccessRead"), std::string("Channel Access Read"));
}
