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

#include "oac_tree_gui/model/instruction_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods from instruction_item_helper.h
 */
class InstructionItemHelperTest : public ::testing::Test
{
public:
  //! Inserts an instruction with the given name.
  static InstructionItem* InsertNamed(const std::string& domain_type, const std::string& name,
                                      mvvm::SessionItem* parent)
  {
    auto result = InsertInstruction(domain_type, parent);
    result->SetName(name);
    return result;
  }

  //! Inserts an Include with the given name and path.
  static InstructionItem* InsertInclude(const std::string& name, const std::string& path,
                                        mvvm::SessionItem* parent)
  {
    auto result = InsertNamed(domainconstants::kIncludeInstructionType, name, parent);
    result->SetAttribute(domainconstants::kPathAttribute, path);
    return result;
  }
};

TEST_F(InstructionItemHelperTest, AddKnownInstructionToSequencerModel)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = InsertInstruction(domainconstants::kWaitInstructionType,
                                procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(InstructionItemHelperTest, AddUnknownDomainInstructionToTheGUI)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item =
      InsertInstruction(UnknownDomainInstruction::Type, procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<InstructionItem>());
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(InstructionItemHelperTest, IsCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsCollapsed(*item));

  SetCollapsed(true, *item);
  EXPECT_TRUE(IsCollapsed(*item));

  SetCollapsed(false, *item);
  EXPECT_FALSE(IsCollapsed(*item));
}

//! Without showCollapsed, Include is collapsed and Sequence is expanded.
TEST_F(InstructionItemHelperTest, IsCollapsedByDefault)
{
  auto include = CreateInstructionItem(domainconstants::kIncludeInstructionType);
  EXPECT_FALSE(
      include->GetAttributes()->FindItem(domainconstants::kShowCollapsedAttribute)->HasData());
  EXPECT_TRUE(IsCollapsed(*include));

  SetCollapsed(false, *include);
  EXPECT_FALSE(IsCollapsed(*include));

  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(
      sequence->GetAttributes()->FindItem(domainconstants::kShowCollapsedAttribute)->HasData());
  EXPECT_FALSE(IsCollapsed(*sequence));
}

TEST_F(InstructionItemHelperTest, ToggleCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);

  EXPECT_FALSE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_TRUE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_FALSE(IsCollapsed(*item));
}

TEST_F(InstructionItemHelperTest, IsCollapsible)
{
  EXPECT_TRUE(IsCollapsible(*CreateInstructionItem(domainconstants::kSequenceInstructionType)));
  EXPECT_FALSE(IsCollapsible(*CreateInstructionItem(domainconstants::kWaitInstructionType)));
}

TEST_F(InstructionItemHelperTest, GetCollapsedItems)
{
  InstructionContainerItem container;
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, &container);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, sequence0);

  auto collapsed = GetCollapsedItems(container);
  EXPECT_TRUE(collapsed.empty());

  SetCollapsed(true, *sequence0);
  SetCollapsed(true, *sequence1);

  collapsed = GetCollapsedItems(container);
  EXPECT_EQ(collapsed, std::vector<const InstructionItem*>({sequence0, sequence1}));
}

TEST_F(InstructionItemHelperTest, IsCompoundInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsCompoundInstruction(wait.get()));
}

TEST_F(InstructionItemHelperTest, IsDecoratorInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsDecoratorInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(wait.get()));
}

TEST_F(InstructionItemHelperTest, IsPotentialRootInstruction)
{
  {  // instruction outside container
    const auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
    EXPECT_FALSE(IsPotentialRootInstruction(*sequence));
  }

  {  // two instructions in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence1));
  }

  {  // instruction with child in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_FALSE(IsPotentialRootInstruction(*wait0));
  }
}

TEST_F(InstructionItemHelperTest, GetIsRootItem)
{
  const InstructionItem item;
  EXPECT_EQ(GetIsRootItem(item), nullptr);

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_NE(GetIsRootItem(*wait), nullptr);
}

TEST_F(InstructionItemHelperTest, GetInstructionFromProperty)
{
  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);

  auto* timeout = wait->GetAttributes()->FindItem(domainconstants::kTimeoutAttribute);
  ASSERT_NE(timeout, nullptr);
  EXPECT_EQ(GetInstructionFromProperty(*timeout), wait.get());

  auto* status = wait->FindItem(itemconstants::kStatus);
  ASSERT_NE(status, nullptr);
  EXPECT_EQ(GetInstructionFromProperty(*status), wait.get());

  const mvvm::SessionItem item;
  EXPECT_EQ(GetInstructionFromProperty(item), nullptr);
}

TEST_F(InstructionItemHelperTest, IsIncludeInstruction)
{
  InstructionContainerItem container;
  auto include0 = InsertInclude("Local", "Target", &container);
  EXPECT_TRUE(IsIncludeInstruction(*include0));

  auto include1 = InsertInclude("External", "Target", &container);
  include1->SetAttribute(domainconstants::kFileNameAttribute, std::string("other.xml"));
  EXPECT_TRUE(IsIncludeInstruction(*include1));

  auto include_procedure0 =
      InsertInstruction(domainconstants::kIncludeProcedureInstructionType, &container);
  EXPECT_TRUE(IsIncludeInstruction(*include_procedure0));

  auto repeat0 = InsertInstruction(domainconstants::kRepeatInstructionType, &container);
  EXPECT_FALSE(IsIncludeInstruction(*repeat0));
}

TEST_F(InstructionItemHelperTest, IsLocalInclude)
{
  InstructionContainerItem container;
  auto include0 = InsertInclude("Local", "Target", &container);
  EXPECT_TRUE(IsLocalInclude(*include0));

  auto include1 = InsertInclude("External", "Target", &container);
  include1->SetAttribute(domainconstants::kFileNameAttribute, std::string("other.xml"));
  EXPECT_FALSE(IsLocalInclude(*include1));

  auto include_procedure0 =
      InsertInstruction(domainconstants::kIncludeProcedureInstructionType, &container);
  EXPECT_FALSE(IsLocalInclude(*include_procedure0));

  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, &container);
  EXPECT_FALSE(IsLocalInclude(*wait0));
}

//! Layout of oac-tree test/resources/local_include.xml.
TEST_F(InstructionItemHelperTest, FindIncludeTarget)
{
  InstructionContainerItem container;
  auto wait0 = InsertNamed(domainconstants::kWaitInstructionType, "SingleWait", &container);
  auto sequence0 =
      InsertNamed(domainconstants::kSequenceInstructionType, "WaitLongEnough", &container);
  auto include0 = InsertInclude("FirstWait", "SingleWait", sequence0);
  (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto include1 = InsertInclude("Wait Sequence", "WaitLongEnough", sequence1);
  auto include2 = InsertInclude("Wait one second", "WaitLongEnough.FirstWait", sequence1);

  EXPECT_EQ(FindIncludeTarget(*include0, container), wait0);
  EXPECT_EQ(FindIncludeTarget(*include1, container), sequence0);

  // nested target, which is an Include itself
  EXPECT_EQ(FindIncludeTarget(*include2, container), include0);

  // trailing delimiter is ignored, as in oac-tree
  auto include3 = InsertInclude("Trailing", "WaitLongEnough.", sequence1);
  EXPECT_EQ(FindIncludeTarget(*include3, container), sequence0);
}

TEST_F(InstructionItemHelperTest, FindIncludeTargetNotResolved)
{
  InstructionContainerItem container;
  auto sequence0 = InsertNamed(domainconstants::kSequenceInstructionType, "Top", &container);
  (void)InsertNamed(domainconstants::kWaitInstructionType, "Child", sequence0);

  auto include0 = InsertInclude("MissingName", "Missing", sequence0);
  EXPECT_EQ(FindIncludeTarget(*include0, container), nullptr);

  auto include1 = InsertInclude("MissingChild", "Top.Missing", sequence0);
  EXPECT_EQ(FindIncludeTarget(*include1, container), nullptr);

  auto include2 = InsertInclude("EmptyPath", "", sequence0);
  EXPECT_EQ(FindIncludeTarget(*include2, container), nullptr);

  auto include3 = InsertInclude("External", "Top", sequence0);
  include3->SetAttribute(domainconstants::kFileNameAttribute, std::string("other.xml"));
  EXPECT_EQ(FindIncludeTarget(*include3, container), nullptr);

  auto include_procedure0 =
      InsertInstruction(domainconstants::kIncludeProcedureInstructionType, sequence0);
  include_procedure0->SetAttribute(domainconstants::kPathAttribute, std::string("Top"));
  EXPECT_EQ(FindIncludeTarget(*include_procedure0, container), nullptr);

  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
  EXPECT_EQ(FindIncludeTarget(*wait0, container), nullptr);
}

//! The first name match wins at each level, without backtracking.
TEST_F(InstructionItemHelperTest, FindIncludeTargetDuplicateNames)
{
  InstructionContainerItem container;
  auto sequence0 = InsertNamed(domainconstants::kSequenceInstructionType, "A", &container);
  auto sequence1 = InsertNamed(domainconstants::kSequenceInstructionType, "A", &container);
  (void)InsertNamed(domainconstants::kWaitInstructionType, "B", sequence1);
  auto sequence2 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);

  auto include0 = InsertInclude("First", "A", sequence2);
  EXPECT_EQ(FindIncludeTarget(*include0, container), sequence0);

  // only the second "A" has "B"
  auto include1 = InsertInclude("Nested", "A.B", sequence2);
  EXPECT_EQ(FindIncludeTarget(*include1, container), nullptr);
}

TEST_F(InstructionItemHelperTest, GetTopLevelInstruction)
{
  InstructionContainerItem container;
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, sequence0);
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence1);

  EXPECT_EQ(GetTopLevelInstruction(*sequence0), sequence0);
  EXPECT_EQ(GetTopLevelInstruction(*sequence1), sequence0);
  EXPECT_EQ(GetTopLevelInstruction(*wait0), sequence0);

  // outside of a container
  auto sequence2 = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  auto wait1 = InsertInstruction(domainconstants::kWaitInstructionType, sequence2.get());
  EXPECT_EQ(GetTopLevelInstruction(*sequence2), nullptr);
  EXPECT_EQ(GetTopLevelInstruction(*wait1), nullptr);
}

TEST_F(InstructionItemHelperTest, FindRootInstruction)
{
  InstructionContainerItem container;
  EXPECT_EQ(FindRootInstruction(container), nullptr);

  // single instruction without isRoot
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  EXPECT_EQ(FindRootInstruction(container), sequence0);

  // no isRoot: the first one
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto sequence2 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  EXPECT_EQ(FindRootInstruction(container), sequence0);

  sequence1->SetIsRootFlag(true);
  EXPECT_EQ(FindRootInstruction(container), sequence1);

  // the first one with isRoot
  sequence2->SetIsRootFlag(true);
  EXPECT_EQ(FindRootInstruction(container), sequence1);
}

}  // namespace oac_tree_gui::test
