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

#include "oac_tree_gui/transform/instruction_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/universal_instruction_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_descriptor_helper.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>
#include <testutils/cmake_info.h>

#include <algorithm>
#include <filesystem>

namespace oac_tree_gui::test
{

namespace
{

//! Returns data roles of the item, without the identifier, sorted.
std::vector<mvvm::role_t> GetRoles(const mvvm::SessionItem& item)
{
  auto result = item.GetItemData()->GetRoles();
  result.erase(std::remove(result.begin(), result.end(), mvvm::DataRole::kIdentifier),
               result.end());
  std::sort(result.begin(), result.end());
  return result;
}

//! Expects equal item trees: types, data of all roles except identifier, tags and children.
void ExpectSameItems(const mvvm::SessionItem& lhs, const mvvm::SessionItem& rhs,
                     const std::string& path)
{
  EXPECT_EQ(lhs.GetType(), rhs.GetType()) << path;
  const auto roles = GetRoles(lhs);
  EXPECT_EQ(roles, GetRoles(rhs)) << path;
  for (auto role : roles)
  {
    EXPECT_EQ(lhs.Data(role), rhs.Data(role)) << path << " role " << role;
  }

  const auto* lhs_tags = lhs.GetTaggedItems();
  const auto* rhs_tags = rhs.GetTaggedItems();
  EXPECT_EQ(lhs_tags->GetDefaultTag(), rhs_tags->GetDefaultTag()) << path;
  ASSERT_EQ(lhs_tags->GetTagCount(), rhs_tags->GetTagCount()) << path;
  for (std::size_t tag_index = 0; tag_index < lhs_tags->GetTagCount(); ++tag_index)
  {
    const auto& lhs_container = lhs_tags->ContainerAt(tag_index);
    const auto& rhs_container = rhs_tags->ContainerAt(tag_index);
    EXPECT_EQ(lhs_container.GetTagInfo(), rhs_container.GetTagInfo()) << path;
    ASSERT_EQ(lhs_container.GetItemCount(), rhs_container.GetItemCount())
        << path << "/" << lhs_container.GetName();
    for (std::size_t index = 0; index < lhs_container.GetItemCount(); ++index)
    {
      ExpectSameItems(*lhs_container.ItemAt(index), *rhs_container.ItemAt(index),
                      path + "/" + lhs_container.GetName() + "[" + std::to_string(index) + "]");
    }
  }
}

//! Builds an item tree from the domain instruction with the item builder.
InstructionTree CreateItemTreeFromNodes(const sup::oac_tree::Instruction& instruction)
{
  return CreateInstructionItemTree(CreateInstructionNodeTree(instruction));
}

}  // namespace

/**
 * @brief Tests of helper methods in instruction_item_transform_helper.h
 */
class InstructionItemTransformHelperTest : public ::testing::Test
{
public:
  sup::oac_tree::Procedure m_procedure;
};

//! Testing CreateInstructionItem helper method.
TEST_F(InstructionItemTransformHelperTest, CreateInstructionItemFromInfoObject)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction_item = CreateInstructionItem(info);

  auto wait_item = instruction_item.get();
  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(wait_item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);
}

//! Testing CreateInstructionInfoItem method.
TEST_F(InstructionItemTransformHelperTest, CreateInstructionInfoItemFromInfoObject)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction_item = CreateInstructionInfoItem(info);

  auto wait_item = dynamic_cast<InstructionInfoItem*>(instruction_item.get());
  ASSERT_NE(wait_item, nullptr);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a single
//! instruction.
TEST_F(InstructionItemTransformHelperTest, CreateInstructionItemTreeForWaitInfo)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto item_tree = CreateInstructionItemTree(info);
  ASSERT_EQ(item_tree.indexes.size(), 1);

  auto wait_item = item_tree.root.get();

  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(wait_item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);

  EXPECT_EQ(item_tree.indexes[0], wait_item);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a sequence with two
//! children.
TEST_F(InstructionItemTransformHelperTest, CreateInstructionItemTreeForSequenceInfo)
{
  using namespace oac_tree_gui::domainconstants;
  using sup::oac_tree::AttributeInfo;
  using sup::oac_tree::InstructionInfo;

  InstructionInfo sequence_info(kSequenceInstructionType,
                                sup::oac_tree::Instruction::Category::kCompound, 0, {});
  auto child0 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 1,
      std::vector<AttributeInfo>({{kTimeoutAttribute, "42"}}));
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 2,
      std::vector<AttributeInfo>({{kTimeoutAttribute, "43"}}));
  auto child1_ptr = child0.get();

  sequence_info.AppendChild(std::move(child0));
  sequence_info.AppendChild(std::move(child1));

  auto item_tree = CreateInstructionItemTree(sequence_info);

  ASSERT_EQ(item_tree.indexes.size(), 3);

  auto sequence_item = item_tree.root.get();
  ASSERT_NE(sequence_item, nullptr);
  EXPECT_EQ(sequence_item->GetDomainType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);
  auto wait_items = sequence_item->FindItems<InstructionItem>(mvvm::TagIndex::GetDefaultTag());
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42);
  EXPECT_EQ(wait_items[1]->GetAttribute<double>(domainconstants::kTimeoutAttribute), 43);

  EXPECT_EQ(item_tree.indexes[0], sequence_item);
  EXPECT_EQ(item_tree.indexes[1], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[2], wait_items[1]);
}

//! Creates simplified InstructionInfoItem tree.
TEST_F(InstructionItemTransformHelperTest, CreateInstructionInfoItemTreeForSequenceInfo)
{
  using namespace oac_tree_gui::domainconstants;
  using sup::oac_tree::AttributeInfo;
  using sup::oac_tree::InstructionInfo;

  InstructionInfo sequence_info(kSequenceInstructionType,
                                sup::oac_tree::Instruction::Category::kCompound, 0, {});
  auto child0 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 1,
      std::vector<AttributeInfo>({{kTimeoutAttribute, "42"}}));
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 2,
      std::vector<AttributeInfo>({{kTimeoutAttribute, "43"}}));
  auto child1_ptr = child0.get();

  sequence_info.AppendChild(std::move(child0));
  sequence_info.AppendChild(std::move(child1));

  auto item_tree = CreateInstructionInfoItemTree(sequence_info);

  ASSERT_EQ(item_tree.indexes.size(), 3);

  auto sequence_item = dynamic_cast<InstructionInfoItem*>(item_tree.root.get());
  ASSERT_NE(sequence_item, nullptr);
  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);
  auto wait_items = sequence_item->FindItems<InstructionInfoItem>(mvvm::TagIndex::GetDefaultTag());
  ASSERT_EQ(wait_items.size(), 2);

  EXPECT_EQ(item_tree.indexes[0], sequence_item);
  EXPECT_EQ(item_tree.indexes[1], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[2], wait_items[1]);
}

TEST_F(InstructionItemTransformHelperTest, CreateInstructionTreeFromRootInstruction)
{
  const std::string procedure_xml = R"RAW(
<Procedure>
  <Repeat maxCount="10">
    <Sequence>
      <Wait timeout="42"/>
      <Wait timeout="43"/>
    </Sequence>
  </Repeat>
</Procedure>
)RAW";
  auto procedure = sup::oac_tree::ParseProcedureString(procedure_xml);

  auto item_tree = CreateInstructionItemTree(*procedure->RootInstruction());
  ASSERT_EQ(item_tree.indexes.size(), 4);

  auto repeat_item = item_tree.root.get();
  ASSERT_NE(repeat_item, nullptr);
  EXPECT_EQ(repeat_item->GetDomainType(), domainconstants::kRepeatInstructionType);
  ASSERT_EQ(repeat_item->GetInstructions().size(), 1);
  auto* sequence_item = repeat_item->FindItem<InstructionItem>(mvvm::TagIndex::GetDefaultTag());
  ASSERT_NE(sequence_item, nullptr);
  EXPECT_EQ(sequence_item->GetDomainType(), domainconstants::kSequenceInstructionType);

  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);

  auto wait_items = sequence_item->FindItems<InstructionItem>(mvvm::TagIndex::GetDefaultTag());
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42);
  EXPECT_EQ(wait_items[1]->GetAttribute<double>(domainconstants::kTimeoutAttribute), 43);

  EXPECT_EQ(item_tree.indexes[0], repeat_item);
  EXPECT_EQ(item_tree.indexes[1], sequence_item);
  EXPECT_EQ(item_tree.indexes[2], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[3], wait_items[1]);
}

TEST_F(InstructionItemTransformHelperTest, RegisterChildrenTag)
{
  {
    auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_FALSE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));
  }

  {
    auto instruction = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), -1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }

  {
    auto instruction = CreateDomainInstruction(domainconstants::kInverterInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), 1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }
}

TEST_F(InstructionItemTransformHelperTest, RegisterChildrenTagUsingCategory)
{
  using Category = sup::oac_tree::Instruction::Category;

  {
    mvvm::CompoundItem item;
    RegisterChildrenTag(Category::kAction, item);
    EXPECT_FALSE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));
  }

  {
    mvvm::CompoundItem item;
    RegisterChildrenTag(Category::kCompound, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), -1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }

  {
    mvvm::CompoundItem item;
    RegisterChildrenTag(Category::kDecorator, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), 1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }
}

TEST_F(InstructionItemTransformHelperTest, RegisterShowCollapsedProperty)
{
  using Category = sup::oac_tree::Instruction::Category;

  {
    mvvm::CompoundItem item;
    RegisterShowCollapsedProperty(Category::kCompound, domainconstants::kSequenceInstructionType,
                                  item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute));
    auto property = item.FindItem<sup::gui::AnyValueItem>(domainconstants::kShowCollapsedAttribute);
    EXPECT_FALSE(property->Data<bool>());  // expanded by default
  }

  {
    mvvm::CompoundItem item;
    RegisterShowCollapsedProperty(Category::kDecorator, domainconstants::kIncludeInstructionType,
                                  item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute));
    auto property = item.FindItem<sup::gui::AnyValueItem>(domainconstants::kShowCollapsedAttribute);
    EXPECT_TRUE(property->Data<bool>());  // collapsed by default
  }

  {
    mvvm::CompoundItem item;
    RegisterShowCollapsedProperty(Category::kAction, domainconstants::kWaitInstructionType, item);
    EXPECT_FALSE(mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute));
  }
}

//! Include with a custom attribute, domain to item.
TEST_F(InstructionItemTransformHelperTest, IncludeFromDomainWithCustomAttribute)
{
  const std::string custom_name("par1");
  const std::string custom_value("1.0");

  auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  input->AddAttribute(domainconstants::kFileNameAttribute, "abc");
  input->AddAttribute(domainconstants::kPathAttribute, "def");
  input->AddAttribute(custom_name, custom_value);

  auto item_tree = CreateInstructionItemTree(*input);
  auto item = item_tree.root.get();
  EXPECT_EQ(item->GetDomainType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute), "abc");
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kPathAttribute), "def");

  auto property = item->FindItem<sup::gui::AnyValueScalarItem>(custom_name);
  ASSERT_NE(property, nullptr);
  EXPECT_EQ(property->GetAnyTypeName(), sup::dto::kStringTypeName);
  EXPECT_EQ(property->Data<std::string>(), custom_value);
}

//! Include with a custom attribute, item to domain. Include is collapsed by default.
TEST_F(InstructionItemTransformHelperTest, IncludeToDomainWithCustomAttribute)
{
  const std::string custom_name("par1");
  const std::string custom_value("1.0");

  auto item = CreateInstructionItem(domainconstants::kIncludeInstructionType);
  item->SetAttribute(domainconstants::kPathAttribute, std::string("def"));
  auto& property = item->AddProperty<sup::gui::AnyValueScalarItem>(custom_name);
  property.SetAnyTypeName(sup::dto::kStringTypeName);
  (void)property.SetData(custom_value);
  EXPECT_TRUE(IsCollapsed(*item));

  auto domain_item = item->CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kPathAttribute), "def");
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileNameAttribute));
  EXPECT_EQ(domain_item->GetAttributeString(custom_name), custom_value);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kShowCollapsedAttribute), "true");

  // Setup of Include requires the instruction to include
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->SetName("def");
  m_procedure.InsertInstruction(std::move(wait), 0);
  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

//! Zero success threshold of ParallelSequence goes to the domain.
TEST_F(InstructionItemTransformHelperTest, ParallelSequenceToDomainWithZeroSuccessThreshold)
{
  auto item = CreateInstructionItem(domainconstants::kParallelInstructionType);
  item->SetAttribute(domainconstants::kSuccessThresholdAttribute, mvvm::uint32{0});
  item->SetAttribute(domainconstants::kFailureThresholdAttribute, mvvm::uint32{43});

  auto domain_item = item->CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kParallelInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kSuccessThresholdAttribute), "0");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFailureThresholdAttribute), "43");

  // no children
  EXPECT_THROW(domain_item->Setup(m_procedure), sup::oac_tree::InstructionSetupException);
}

//! Repeat maxCount in both directions. Repeat accepts one child.
TEST_F(InstructionItemTransformHelperTest, RepeatRoundTrip)
{
  auto input = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  input->AddAttribute(domainconstants::kMaxCountAttribute, "42");
  auto item_tree = CreateInstructionItemTree(*input);
  EXPECT_EQ(item_tree.root->GetAttribute<mvvm::int32>(domainconstants::kMaxCountAttribute), 42);

  auto item = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  item->SetAttribute(domainconstants::kMaxCountAttribute, mvvm::int32{42});
  auto domain_item = item->CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kMaxCountAttribute), "42");

  // no child
  EXPECT_THROW(domain_item->Setup(m_procedure), sup::oac_tree::InstructionSetupException);

  (void)InsertInstruction(domainconstants::kWaitInstructionType, item.get());
  EXPECT_THROW(InsertInstruction(domainconstants::kWaitInstructionType, item.get()),
               mvvm::InvalidOperationException);
}

//! Wait with name, timeout and blocking, domain to item.
TEST_F(InstructionItemTransformHelperTest, WaitFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  input->AddAttribute(domainconstants::kNameAttribute, "First");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42");
  input->AddAttribute(domainconstants::kBlockingAttribute, "true");

  auto item_tree = CreateInstructionItemTree(*input);
  auto item = item_tree.root.get();
  EXPECT_EQ(item->GetDisplayName(), "First");
  EXPECT_EQ(item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);
  EXPECT_TRUE(item->GetAttribute<bool>(domainconstants::kBlockingAttribute));
}

//! Wait timeout defined as a placeholder goes to the domain as is.
TEST_F(InstructionItemTransformHelperTest, WaitToDomainVaryingTimeout)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);
  auto property = item->FindItem<sup::gui::AnyValueScalarItem>(domainconstants::kTimeoutAttribute);
  ASSERT_NE(property, nullptr);
  SetAttributeExposedFlag(true, *property);
  SetAttributeAsString("$par1", *property);

  auto domain_item = item->CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "$par1");
}

//! Root flag goes to the domain only when set.
TEST_F(InstructionItemTransformHelperTest, WaitToDomainIsRootAttribute)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);

  item->SetIsRootFlag(true);
  auto domain_item = item->CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

  item->SetIsRootFlag(false);
  domain_item = item->CreateDomainInstruction();
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kIsRootAttribute));
}

//! PvAccessWrite with JSON type and value in both directions.
TEST_F(InstructionItemTransformHelperTest, PvAccessWriteRoundTrip)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  {  // domain to item
    auto input = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
    input->AddAttribute(domainconstants::kGenericVariableNameAttribute, "abc");
    input->AddAttribute(domainconstants::kChannelAttribute, "def");
    input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
    input->AddAttribute(domainconstants::kTypeAttribute, expected_type);
    input->AddAttribute(domainconstants::kValueAttribute, expected_value);

    auto item_tree = CreateInstructionItemTree(*input);
    auto item = item_tree.root.get();
    auto anyvalue_item = GetAnyValueItem(*item);
    ASSERT_NE(anyvalue_item, nullptr);
    EXPECT_EQ(CreateAnyValue(*anyvalue_item), expected_anyvalue);
    EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kGenericVariableNameAttribute),
              "abc");
    EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);
  }

  {  // item to domain
    auto item = CreateInstructionItem(domainconstants::kPvAccessWriteInstructionType);
    item->SetAttribute(domainconstants::kChannelAttribute, std::string("def"));
    SetAnyValue(expected_anyvalue, *item);

    auto domain_item = item->CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);
    EXPECT_NO_THROW(domain_item->Setup(m_procedure));

    // not exposed AnyValueItem does not go to the domain
    item->SetAttribute(domainconstants::kGenericVariableNameAttribute, std::string("var"));
    SetAttributeExposedFlag(false, *GetAnyValueItem(*item));
    domain_item = item->CreateDomainInstruction();
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));
    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

//! RPCClient with JSON type and value in both directions.
TEST_F(InstructionItemTransformHelperTest, RPCClientRoundTrip)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  {  // domain to item
    auto input = CreateDomainInstruction(domainconstants::kRPCClientInstructionType);
    input->AddAttribute(domainconstants::kServiceAttribute, "service");
    input->AddAttribute(domainconstants::kTypeAttribute, expected_type);
    input->AddAttribute(domainconstants::kValueAttribute, expected_value);
    input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "output");

    auto item_tree = CreateInstructionItemTree(*input);
    auto item = item_tree.root.get();
    auto anyvalue_item = GetAnyValueItem(*item);
    ASSERT_NE(anyvalue_item, nullptr);
    EXPECT_EQ(CreateAnyValue(*anyvalue_item), expected_anyvalue);
    EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kServiceAttribute), "service");
    EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kOutputVariableNameAttribute),
              "output");
  }

  {  // item to domain
    auto item = CreateInstructionItem(domainconstants::kRPCClientInstructionType);
    item->SetAttribute(domainconstants::kServiceAttribute, std::string("service"));
    item->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("output"));
    SetAnyValue(expected_anyvalue, *item);

    auto domain_item = item->CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kServiceAttribute), "service");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);
    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

//! Item tree from a hand-written node, no plugin needed.
TEST_F(InstructionItemTransformHelperTest, ItemTreeFromHandWrittenNode)
{
  const AttributeDescriptor name_attribute = {
      domainconstants::kNameAttribute,           std::string{}, {}, false, false,
      sup::oac_tree::AttributeCategory::kLiteral};

  const AttributeDescriptor is_root_attribute = {
      domainconstants::kIsRootAttribute,         false, {}, false, true,
      sup::oac_tree::AttributeCategory::kLiteral};

  const AttributeDescriptor threshold_attribute = {
      "threshold", mvvm::uint32{0}, "Threshold tooltip",
      false,       false,           sup::oac_tree::AttributeCategory::kBoth};

  const AttributeDescriptor path_attribute{
      "path", std::string{"default"}, {}, true, true, sup::oac_tree::AttributeCategory::kLiteral};

  const std::vector<AttributeDescriptor> attributes = {name_attribute, is_root_attribute,
                                                       threshold_attribute, path_attribute};

  const InstructionDescriptor descriptor{"UnknownCompound", sup::oac_tree::Instruction::kCompound,
                                         attributes};

  const InstructionDescriptor child_descriptor{
      "UnknownAction", sup::oac_tree::Instruction::kAction, {}};

  std::vector<sup::oac_tree::AttributeInfo> attribute_info = {
      {domainconstants::kNameAttribute, "main"}, {"threshold", "3"}, {"custom", "abc"}};

  const InstructionNode node{descriptor, attribute_info, 1, {{child_descriptor, {}, 0, {}}}};

  auto tree = CreateInstructionItemTree(node);
  auto* item = tree.root.get();

  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<UniversalInstructionItem>());
  EXPECT_EQ(item->GetDomainType(), "UnknownCompound");
  EXPECT_EQ(item->GetDisplayName(), "main");

  auto* threshold = item->FindItem<sup::gui::AnyValueScalarItem>("threshold");
  ASSERT_NE(threshold, nullptr);
  EXPECT_EQ(threshold->GetAnyTypeName(), sup::dto::kUInt32TypeName);
  EXPECT_EQ(threshold->Data<mvvm::uint32>(), 3);
  EXPECT_EQ(threshold->GetToolTip(), "Threshold tooltip");
  EXPECT_TRUE(GetAttributeExposedFlag(*threshold));

  auto* path = item->FindItem<sup::gui::AnyValueScalarItem>("path");
  ASSERT_NE(path, nullptr);
  EXPECT_EQ(path->Data<std::string>(), "default");
  EXPECT_TRUE(GetAttributeExposedFlag(*path));
  EXPECT_FALSE(item->IsRoot());

  auto* custom = item->FindItem<sup::gui::AnyValueScalarItem>("custom");
  ASSERT_NE(custom, nullptr);
  EXPECT_EQ(custom->GetAnyTypeName(), sup::dto::kStringTypeName);
  EXPECT_EQ(custom->Data<std::string>(), "abc");

  EXPECT_FALSE(mvvm::utils::HasTag(*item, domainconstants::kNameAttribute));
  EXPECT_FALSE(mvvm::utils::HasTag(*item, domainconstants::kShowCollapsedAttribute));
  for (const auto* tag : {itemconstants::kBehaviorTag, itemconstants::kStatus, itemconstants::kXpos,
                          itemconstants::kYpos, itemconstants::kBreakpoint})
  {
    EXPECT_TRUE(mvvm::utils::HasTag(*item, tag)) << tag;
  }
  EXPECT_EQ(item->GetBehavior(), itemconstants::kNativeBehavior);

  ASSERT_EQ(item->GetInstructions().size(), 1);
  auto* child = item->GetInstructions().at(0);
  EXPECT_EQ(child->GetDomainType(), "UnknownAction");
  EXPECT_EQ(child->GetDisplayName(), "UnknownAction");

  const std::vector<InstructionItem*> expected_indexes{child, item};
  EXPECT_EQ(tree.indexes, expected_indexes);
}

TEST_F(InstructionItemTransformHelperTest, ItemTreeIndexOutOfRange)
{
  const InstructionNode node{{"UnknownAction", sup::oac_tree::Instruction::kAction, {}}, {}, 1, {}};
  EXPECT_THROW(CreateInstructionItemTree(node), RuntimeException);
}

//! Placeholders and references are stored as strings and exposed.
TEST_F(InstructionItemTransformHelperTest, ItemTreeWithPlaceholderAndReference)
{
  auto node = CreateInstructionNode(domainconstants::kWaitInstructionType);
  node.attributes = {{domainconstants::kTimeoutAttribute, "$par"},
                     {domainconstants::kBlockingAttribute, "@var"}};

  auto tree = CreateInstructionItemTree(node);
  auto* timeout = tree.root->FindItem<sup::gui::AnyValueItem>(domainconstants::kTimeoutAttribute);
  EXPECT_EQ(timeout->Data<std::string>(), "$par");
  EXPECT_TRUE(GetAttributeExposedFlag(*timeout));
  auto* blocking = tree.root->FindItem<sup::gui::AnyValueItem>(domainconstants::kBlockingAttribute);
  EXPECT_EQ(blocking->Data<std::string>(), "@var");
  EXPECT_TRUE(GetAttributeExposedFlag(*blocking));

  auto domain = CreateDomainInstruction(*tree.root);
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kTimeoutAttribute), "$par");
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kBlockingAttribute), "@var");
}

//! Item from info: typed base attributes, other attributes as strings, no AnyValue tag.
TEST_F(InstructionItemTransformHelperTest, ItemTreeFromInfoNode)
{
  {  // Include is collapsed without showCollapsed
    const sup::oac_tree::InstructionInfo info(domainconstants::kIncludeInstructionType,
                                              sup::oac_tree::Instruction::kDecorator, 0,
                                              {{domainconstants::kPathAttribute, "abc"}});
    auto tree = CreateInstructionItemTree(CreateInstructionNodeTree(info));
    EXPECT_TRUE(IsCollapsed(*tree.root));
    auto* path = tree.root->FindItem<sup::gui::AnyValueScalarItem>(domainconstants::kPathAttribute);
    ASSERT_NE(path, nullptr);
    EXPECT_EQ(path->GetAnyTypeName(), sup::dto::kStringTypeName);
    EXPECT_EQ(path->Data<std::string>(), "abc");
  }

  {  // showCollapsed string is parsed as bool
    const sup::oac_tree::InstructionInfo info(domainconstants::kIncludeInstructionType,
                                              sup::oac_tree::Instruction::kDecorator, 0,
                                              {{domainconstants::kShowCollapsedAttribute, "false"},
                                               {domainconstants::kIsRootAttribute, "true"}});
    auto tree = CreateInstructionItemTree(CreateInstructionNodeTree(info));
    EXPECT_FALSE(IsCollapsed(*tree.root));
    EXPECT_TRUE(tree.root->IsRoot());
  }

  {  // JSON pair stays as strings
    const std::string type_json(R"RAW({"type":"int32"})RAW");
    const sup::oac_tree::InstructionInfo info(
        domainconstants::kPvAccessWriteInstructionType, sup::oac_tree::Instruction::kAction, 0,
        {{domainconstants::kTypeAttribute, type_json}, {domainconstants::kValueAttribute, "42"}});
    auto tree = CreateInstructionItemTree(CreateInstructionNodeTree(info));
    EXPECT_FALSE(mvvm::utils::HasTag(*tree.root, itemconstants::kAnyValueTag));
    EXPECT_EQ(tree.root->Property<std::string>(domainconstants::kTypeAttribute), type_json);
    EXPECT_EQ(tree.root->Property<std::string>(domainconstants::kValueAttribute), "42");
  }
}

//! Repeat gets maxCount from the attribute presets. The preset goes to the domain.
TEST_F(InstructionItemTransformHelperTest, ItemTreeRepeatPreset)
{
  auto tree =
      CreateInstructionItemTree(CreateInstructionNode(domainconstants::kRepeatInstructionType));
  auto* item = tree.root.get();

  EXPECT_EQ(item->GetAttribute<mvvm::int32>(domainconstants::kMaxCountAttribute), -1);
  auto* property = item->FindItem<sup::gui::AnyValueItem>(domainconstants::kMaxCountAttribute);
  ASSERT_NE(property, nullptr);
  EXPECT_TRUE(GetAttributeExposedFlag(*property));

  auto domain = CreateDomainInstruction(*item);
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kMaxCountAttribute), "-1");
}

//! Instructions with JSON type and value attributes get the AnyValue tag.
TEST_F(InstructionItemTransformHelperTest, ItemTreeAnyValueTag)
{
  {
    auto tree =
        CreateInstructionItemTree(CreateInstructionNode(domainconstants::kWaitInstructionType));
    EXPECT_FALSE(mvvm::utils::HasTag(*tree.root, itemconstants::kAnyValueTag));
    EXPECT_EQ(GetAnyValueItem(*tree.root), nullptr);
  }

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  for (const auto* domain_type :
       {domainconstants::kChannelAccessWriteInstructionType,
        domainconstants::kPvAccessWriteInstructionType, domainconstants::kRPCClientInstructionType})
  {
    auto tree = CreateInstructionItemTree(CreateInstructionNode(domain_type));
    EXPECT_TRUE(mvvm::utils::HasTag(*tree.root, itemconstants::kAnyValueTag)) << domain_type;
    auto* anyvalue_item = GetAnyValueItem(*tree.root);
    ASSERT_NE(anyvalue_item, nullptr) << domain_type;
    EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
    EXPECT_FALSE(GetAttributeExposedFlag(*anyvalue_item));
  }
}

//! IncludeProcedure is collapsed by default; showCollapsed goes to the domain both ways.
TEST_F(InstructionItemTransformHelperTest, ItemTreeIncludeProcedureCollapsed)
{
  auto domain = CreateDomainInstruction(domainconstants::kIncludeProcedureInstructionType);
  (void)domain->AddAttribute(domainconstants::kFileNameAttribute, "abc");
  EXPECT_TRUE(IsCollapsed(*CreateItemTreeFromNodes(*domain).root));

  (void)domain->AddAttribute(domainconstants::kShowCollapsedAttribute, "false");
  auto tree = CreateItemTreeFromNodes(*domain);
  EXPECT_FALSE(IsCollapsed(*tree.root));
  EXPECT_EQ(CreateDomainInstruction(*tree.root)
                ->GetAttributeString(domainconstants::kShowCollapsedAttribute),
            "false");

  auto item_tree = CreateInstructionItemTree(
      CreateInstructionNode(domainconstants::kIncludeProcedureInstructionType));
  EXPECT_TRUE(IsCollapsed(*item_tree.root));
  EXPECT_EQ(CreateDomainInstruction(*item_tree.root)
                ->GetAttributeString(domainconstants::kShowCollapsedAttribute),
            "true");
}

//! Wait with name and attributes: item to domain to item.
TEST_F(InstructionItemTransformHelperTest, ItemTreeWaitRoundTrip)
{
  auto tree =
      CreateInstructionItemTree(CreateInstructionNode(domainconstants::kWaitInstructionType));
  auto* item = tree.root.get();
  item->SetName("First");
  item->SetAttribute(domainconstants::kTimeoutAttribute, 42.0);
  item->SetIsRootFlag(true);

  auto domain = CreateDomainInstruction(*item);
  EXPECT_EQ(domain->GetName(), "First");
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kIsRootAttribute), "true");
  EXPECT_FALSE(domain->HasAttribute(domainconstants::kBlockingAttribute));

  auto restored = CreateItemTreeFromNodes(*domain);
  ExpectSameItems(*item, *restored.root, "Wait");
}

//! PvAccessWrite with JSON type and value: item to domain to item.
TEST_F(InstructionItemTransformHelperTest, ItemTreePvAccessWriteRoundTrip)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  auto tree = CreateInstructionItemTree(
      CreateInstructionNode(domainconstants::kPvAccessWriteInstructionType));
  auto* item = tree.root.get();
  item->SetAttribute(domainconstants::kChannelAttribute, std::string("def"));
  SetAnyValue(expected_anyvalue, *item);

  auto domain = CreateDomainInstruction(*item);
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kValueAttribute), "42");
  EXPECT_NO_THROW(domain->Setup(m_procedure));

  auto restored = CreateItemTreeFromNodes(*domain);
  auto* anyvalue_item = GetAnyValueItem(*restored.root);
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(CreateAnyValue(*anyvalue_item), expected_anyvalue);
  EXPECT_EQ(restored.root->GetAttribute<std::string>(domainconstants::kChannelAttribute), "def");
}

//! Temporary: the item builder reproduces UniversalInstructionItem::SetDomainType.
TEST_F(InstructionItemTransformHelperTest, ItemTreeMatchesDomainTypeSetup)
{
  for (const auto& domain_type : GetDomainInstructionNames())
  {
    UniversalInstructionItem expected;
    expected.SetDomainType(domain_type);
    auto tree = CreateInstructionItemTree(CreateInstructionNode(domain_type));
    ExpectSameItems(expected, *tree.root, domain_type);
  }
}

//! Temporary: the item builder reproduces CreateInstructionItemTree(Instruction) for XML files.
TEST_F(InstructionItemTransformHelperTest, ItemTreeMatchesXmlLoad)
{
  std::size_t compared_count{0};
  const auto resource_dir = std::filesystem::path(ProjectSourceDir()) / "test" / "resources";
  for (const auto& entry : std::filesystem::recursive_directory_iterator(resource_dir))
  {
    if (entry.path().extension() != ".xml")
    {
      continue;
    }
    std::unique_ptr<sup::oac_tree::Procedure> procedure;
    try
    {
      procedure = sup::oac_tree::ParseProcedureFile(entry.path().string());
    }
    catch (const std::exception&)
    {
      continue;  // not a procedure, or plugin not available
    }

    for (const auto* instruction : procedure->GetTopInstructions())
    {
      const auto path = entry.path().filename().string() + ":" + instruction->GetType();
      std::unique_ptr<InstructionTree> expected;
      try
      {
        expected = std::make_unique<InstructionTree>(CreateInstructionItemTree(*instruction));
      }
      catch (const std::exception&)
      {
        EXPECT_ANY_THROW(CreateItemTreeFromNodes(*instruction)) << path;
        continue;
      }
      auto tree = CreateItemTreeFromNodes(*instruction);
      ExpectSameItems(*expected->root, *tree.root, path);
      EXPECT_EQ(expected->indexes.size(), tree.indexes.size()) << path;
      ++compared_count;
    }
  }
  EXPECT_GT(compared_count, 20);
}

}  // namespace oac_tree_gui::test
