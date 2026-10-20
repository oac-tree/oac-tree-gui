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
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
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
#include <testutils/universal_items.h>

#include <algorithm>

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

//! Returns true if CreateInstructionItem builds InstructionItem with the given domain
//! type.
bool CanCreateInstruction(const std::string& domain_type)
{
  auto created = CreateInstructionItem(domain_type);
  return created->GetType() == mvvm::GetTypeName<InstructionItem>()
         && created->GetDomainType() == domain_type;
}

//! Builds an item tree from the domain instruction with the item builder.
InstructionTree CreateItemTreeFromDomain(const sup::oac_tree::Instruction& instruction)
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

TEST_F(InstructionItemTransformHelperTest, CreateStandardInstructionItem)
{
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kIncludeInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kSequenceInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kWaitInstructionType));
}

TEST_F(InstructionItemTransformHelperTest, CreateEpicsInstructionItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateInstruction(domainconstants::kChannelAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kChannelAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kPvAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kPvAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstruction(domainconstants::kRPCClientInstructionType));
}

//! An instruction registered at runtime, unknown to the GUI.
TEST_F(InstructionItemTransformHelperTest, CreateUnknownDomainInstructionItem)
{
  test::UnknownDomainInstruction::RegisterUnknownDomainInstruction();
  EXPECT_TRUE(CanCreateInstruction(test::UnknownDomainInstruction::Type));
}

TEST_F(InstructionItemTransformHelperTest, CreateUnregisteredInstructionItemThrows)
{
  EXPECT_THROW(CreateInstructionItem("UnregisteredInstruction"),
               sup::oac_tree::InvalidOperationException);
}

//! Item tree from a single Wait info. Attributes are strings, as reported by the server.
TEST_F(InstructionItemTransformHelperTest, ItemTreeFromWaitInfo)
{
  const sup::oac_tree::InstructionInfo info(domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction, 0,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto item_tree = CreateInstructionItemTree(CreateInstructionNodeTree(info));
  ASSERT_EQ(item_tree.indexes.size(), 1);

  auto* wait_item = item_tree.root.get();
  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(wait_item->GetAttribute<std::string>(domainconstants::kTimeoutAttribute), "42");
  EXPECT_EQ(item_tree.indexes[0], wait_item);
}

//! Item tree from a Sequence info with two Waits. Indexes follow the info.
TEST_F(InstructionItemTransformHelperTest, ItemTreeFromSequenceInfo)
{
  sup::oac_tree::InstructionInfo sequence_info(domainconstants::kSequenceInstructionType,
                                               sup::oac_tree::Instruction::Category::kCompound, 0,
                                               {});
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      domainconstants::kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 1,
      std::vector<sup::oac_tree::AttributeInfo>({{domainconstants::kTimeoutAttribute, "42"}})));
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      domainconstants::kWaitInstructionType, sup::oac_tree::Instruction::Category::kAction, 2,
      std::vector<sup::oac_tree::AttributeInfo>({{domainconstants::kTimeoutAttribute, "43"}})));

  auto item_tree = CreateInstructionItemTree(CreateInstructionNodeTree(sequence_info));
  ASSERT_EQ(item_tree.indexes.size(), 3);

  auto* sequence_item = item_tree.root.get();
  ASSERT_NE(sequence_item, nullptr);
  EXPECT_EQ(sequence_item->GetDomainType(), domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsCollapsed(*sequence_item));
  auto wait_items = sequence_item->GetInstructions();
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetAttribute<std::string>(domainconstants::kTimeoutAttribute), "42");
  EXPECT_EQ(wait_items[1]->GetAttribute<std::string>(domainconstants::kTimeoutAttribute), "43");

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

  auto item_tree = CreateItemTreeFromDomain(*procedure->RootInstruction());
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

//! Children tag follows the category: none for action, one child for decorator, any for compound.
TEST_F(InstructionItemTransformHelperTest, ItemTreeChildrenTag)
{
  auto create_item = [](sup::oac_tree::Instruction::Category category)
  { return CreateInstructionItemTree({{"UnknownInstruction", category, {}}, {}, 0, {}}).root; };

  auto get_children_taginfo = [](const InstructionItem& item)
  {
    const auto* tagged_items = item.GetTaggedItems();
    for (std::size_t index = 0; index < tagged_items->GetTagCount(); ++index)
    {
      if (tagged_items->ContainerAt(index).GetName() == itemconstants::kChildInstructions)
      {
        return tagged_items->ContainerAt(index).GetTagInfo();
      }
    }
    return mvvm::TagInfo(std::string(), 0, 0, {});
  };

  EXPECT_FALSE(mvvm::utils::HasTag(*create_item(sup::oac_tree::Instruction::kAction),
                                   itemconstants::kChildInstructions));

  auto compound = create_item(sup::oac_tree::Instruction::kCompound);
  ASSERT_TRUE(mvvm::utils::HasTag(*compound, itemconstants::kChildInstructions));
  auto compound_taginfo = get_children_taginfo(*compound);
  EXPECT_EQ(compound_taginfo.GetName(), itemconstants::kChildInstructions);
  EXPECT_EQ(compound_taginfo.GetMin(), 0);
  EXPECT_EQ(compound_taginfo.GetMax(), -1);

  auto decorator = create_item(sup::oac_tree::Instruction::kDecorator);
  ASSERT_TRUE(mvvm::utils::HasTag(*decorator, itemconstants::kChildInstructions));
  auto decorator_taginfo = get_children_taginfo(*decorator);
  EXPECT_EQ(decorator_taginfo.GetName(), itemconstants::kChildInstructions);
  EXPECT_EQ(decorator_taginfo.GetMin(), 0);
  EXPECT_EQ(decorator_taginfo.GetMax(), 1);
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

  auto item_tree = CreateItemTreeFromDomain(*input);
  auto item = item_tree.root.get();
  EXPECT_EQ(item->GetDomainType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute), "abc");
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kPathAttribute), "def");

  auto property = item->GetAttributes()->FindItem<AttributeItem>(custom_name);
  ASSERT_NE(property, nullptr);
  EXPECT_EQ(property->GetDefaultValue(), mvvm::variant_t{std::string()});
  EXPECT_EQ(property->Data<std::string>(), custom_value);
}

//! Include with a custom attribute, item to domain. Include is collapsed by default.
TEST_F(InstructionItemTransformHelperTest, IncludeToDomainWithCustomAttribute)
{
  const std::string custom_name("par1");
  const std::string custom_value("1.0");

  auto item = CreateInstructionItem(domainconstants::kIncludeInstructionType);
  item->SetAttribute(domainconstants::kPathAttribute, std::string("def"));
  auto& property = item->GetAttributes()->AddProperty<AttributeItem>(custom_name);
  property.SetAttributeType(std::string(), /*is_nullable*/ false);
  (void)property.SetData(custom_value);
  EXPECT_TRUE(IsCollapsed(*item));

  auto domain_item = CreateDomainInstruction(*item);
  EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kPathAttribute), "def");
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileNameAttribute));
  EXPECT_EQ(domain_item->GetAttributeString(custom_name), custom_value);
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kShowCollapsedAttribute));

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

  auto domain_item = CreateDomainInstruction(*item);
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
  auto item_tree = CreateItemTreeFromDomain(*input);
  EXPECT_EQ(item_tree.root->GetAttribute<mvvm::int32>(domainconstants::kMaxCountAttribute), 42);

  auto item = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  item->SetAttribute(domainconstants::kMaxCountAttribute, mvvm::int32{42});
  auto domain_item = CreateDomainInstruction(*item);
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

  auto item_tree = CreateItemTreeFromDomain(*input);
  auto item = item_tree.root.get();
  EXPECT_EQ(item->GetDisplayName(), "First");
  EXPECT_EQ(item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);
  EXPECT_TRUE(item->GetAttribute<bool>(domainconstants::kBlockingAttribute));
}

//! Wait timeout defined as a placeholder goes to the domain as is.
TEST_F(InstructionItemTransformHelperTest, WaitToDomainVaryingTimeout)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);
  item->SetAttributeText(domainconstants::kTimeoutAttribute, "$par1");

  auto domain_item = CreateDomainInstruction(*item);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "$par1");
}

//! Root flag goes to the domain only when set.
TEST_F(InstructionItemTransformHelperTest, WaitToDomainIsRootAttribute)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);

  item->SetIsRootFlag(true);
  auto domain_item = CreateDomainInstruction(*item);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

  item->SetIsRootFlag(false);
  domain_item = CreateDomainInstruction(*item);
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

    auto item_tree = CreateItemTreeFromDomain(*input);
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

    auto domain_item = CreateDomainInstruction(*item);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);
    EXPECT_NO_THROW(domain_item->Setup(m_procedure));

    // not exposed AnyValueItem does not go to the domain
    item->SetAttribute(domainconstants::kGenericVariableNameAttribute, std::string("var"));
    SetAttributeExposedFlag(false, *GetAnyValueItem(*item));
    domain_item = CreateDomainInstruction(*item);
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

    auto item_tree = CreateItemTreeFromDomain(*input);
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

    auto domain_item = CreateDomainInstruction(*item);
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

  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<InstructionItem>());
  EXPECT_EQ(item->GetDomainType(), "UnknownCompound");
  EXPECT_EQ(item->GetDisplayName(), "main");

  auto* threshold = item->GetAttributes()->FindItem<AttributeItem>("threshold");
  ASSERT_NE(threshold, nullptr);
  EXPECT_EQ(threshold->GetDefaultValue(), mvvm::variant_t{mvvm::uint32{0}});
  EXPECT_TRUE(threshold->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_EQ(threshold->Data<mvvm::uint32>(), 3);
  EXPECT_EQ(threshold->GetToolTip(), "Threshold tooltip");

  // mandatory attribute
  auto* path = item->GetAttributes()->FindItem<AttributeItem>("path");
  ASSERT_NE(path, nullptr);
  EXPECT_FALSE(path->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_EQ(path->Data<std::string>(), "default");

  // isRoot is not nullable, false by default
  auto* is_root = item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kIsRootAttribute);
  ASSERT_NE(is_root, nullptr);
  EXPECT_FALSE(is_root->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_FALSE(item->IsRoot());

  // attribute unknown to the descriptor is a string attribute
  auto* custom = item->GetAttributes()->FindItem<AttributeItem>("custom");
  ASSERT_NE(custom, nullptr);
  EXPECT_EQ(custom->GetDefaultValue(), mvvm::variant_t{std::string()});
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

//! Placeholders and references are stored as text.
TEST_F(InstructionItemTransformHelperTest, ItemTreeWithPlaceholderAndReference)
{
  auto node = CreateInstructionNode(domainconstants::kWaitInstructionType);
  node.attributes = {{domainconstants::kTimeoutAttribute, "$par"},
                     {domainconstants::kBlockingAttribute, "@var"}};

  auto tree = CreateInstructionItemTree(node);
  auto* timeout =
      tree.root->GetAttributes()->FindItem<AttributeItem>(domainconstants::kTimeoutAttribute);
  ASSERT_NE(timeout, nullptr);
  EXPECT_EQ(timeout->Data<std::string>(), "$par");
  EXPECT_TRUE(timeout->HasText());
  auto* blocking =
      tree.root->GetAttributes()->FindItem<AttributeItem>(domainconstants::kBlockingAttribute);
  ASSERT_NE(blocking, nullptr);
  EXPECT_EQ(blocking->Data<std::string>(), "@var");
  EXPECT_TRUE(blocking->HasText());

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
    auto* path =
        tree.root->GetAttributes()->FindItem<AttributeItem>(domainconstants::kPathAttribute);
    ASSERT_NE(path, nullptr);
    EXPECT_EQ(path->GetDefaultValue(), mvvm::variant_t{std::string()});
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
    EXPECT_EQ(tree.root->GetAttributes()->Property<std::string>(domainconstants::kTypeAttribute),
              type_json);
    EXPECT_EQ(tree.root->GetAttributes()->Property<std::string>(domainconstants::kValueAttribute),
              "42");
  }
}

//! Domain attributes are in the attribute container, GUI properties are direct children.
TEST_F(InstructionItemTransformHelperTest, ItemTreeAttributeContainer)
{
  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  ASSERT_NE(wait->GetAttributes(), nullptr);

  const std::vector<std::string> expected_tags{
      itemconstants::kAttributes, itemconstants::kBehaviorTag, itemconstants::kStatus,
      itemconstants::kXpos,       itemconstants::kYpos,        itemconstants::kBreakpoint};
  EXPECT_EQ(mvvm::utils::RegisteredTags(*wait), expected_tags);

  const std::vector<std::string> expected_attribute_tags{domainconstants::kIsRootAttribute,
                                                         domainconstants::kTimeoutAttribute,
                                                         domainconstants::kBlockingAttribute};
  EXPECT_EQ(mvvm::utils::RegisteredTags(*wait->GetAttributes()), expected_attribute_tags);
  EXPECT_FALSE(GetIsRootItem(*wait)->IsVisible());
  EXPECT_TRUE(wait->GetAttributes()->FindItem(domainconstants::kTimeoutAttribute)->IsVisible());

  auto include = CreateInstructionItem(domainconstants::kIncludeInstructionType);
  EXPECT_TRUE(
      mvvm::utils::HasTag(*include->GetAttributes(), domainconstants::kShowCollapsedAttribute));
  EXPECT_TRUE(mvvm::utils::HasTag(*include, itemconstants::kChildInstructions));
  EXPECT_FALSE(mvvm::utils::HasTag(*include, domainconstants::kShowCollapsedAttribute));
}

//! Repeat gets maxCount from the attribute presets. The preset goes to the domain.
TEST_F(InstructionItemTransformHelperTest, ItemTreeRepeatPreset)
{
  auto tree =
      CreateInstructionItemTree(CreateInstructionNode(domainconstants::kRepeatInstructionType));
  auto* item = tree.root.get();

  EXPECT_EQ(item->GetAttribute<mvvm::int32>(domainconstants::kMaxCountAttribute), -1);
  auto* property =
      item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kMaxCountAttribute);
  ASSERT_NE(property, nullptr);
  EXPECT_EQ(property->GetDefaultValue(), mvvm::variant_t{mvvm::int32{-1}});

  auto domain = CreateDomainInstruction(*item);
  EXPECT_EQ(domain->GetAttributeString(domainconstants::kMaxCountAttribute), "-1");
}

//! Instructions with JSON type and value attributes get the AnyValue tag.
TEST_F(InstructionItemTransformHelperTest, ItemTreeAnyValueTag)
{
  {
    auto tree =
        CreateInstructionItemTree(CreateInstructionNode(domainconstants::kWaitInstructionType));
    EXPECT_FALSE(mvvm::utils::HasTag(*tree.root->GetAttributes(), itemconstants::kAnyValueTag));
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
    EXPECT_TRUE(mvvm::utils::HasTag(*tree.root->GetAttributes(), itemconstants::kAnyValueTag))
        << domain_type;
    auto* anyvalue_item = GetAnyValueItem(*tree.root);
    ASSERT_NE(anyvalue_item, nullptr) << domain_type;
    EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
    EXPECT_FALSE(GetAttributeExposedFlag(*anyvalue_item));
  }
}

//! XML without showCollapsed round-trips exactly; an explicit value is kept.
TEST_F(InstructionItemTransformHelperTest, ShowCollapsedRoundTrip)
{
  for (const auto* domain_type :
       {domainconstants::kIncludeInstructionType, domainconstants::kSequenceInstructionType})
  {
    auto domain = CreateDomainInstruction(domain_type);
    auto tree = CreateItemTreeFromDomain(*domain);
    EXPECT_EQ(IsCollapsed(*tree.root), domain_type == domainconstants::kIncludeInstructionType);
    EXPECT_FALSE(
        CreateDomainInstruction(*tree.root)->HasAttribute(domainconstants::kShowCollapsedAttribute))
        << domain_type;

    for (const auto* value : {"true", "false"})
    {
      auto domain_with_value = CreateDomainInstruction(domain_type);
      (void)domain_with_value->AddAttribute(domainconstants::kShowCollapsedAttribute, value);
      auto tree_with_value = CreateItemTreeFromDomain(*domain_with_value);
      EXPECT_EQ(CreateDomainInstruction(*tree_with_value.root)
                    ->GetAttributeString(domainconstants::kShowCollapsedAttribute),
                value)
          << domain_type;
    }
  }
}

//! IncludeProcedure is collapsed by default; showCollapsed goes to the domain both ways.
TEST_F(InstructionItemTransformHelperTest, ItemTreeIncludeProcedureCollapsed)
{
  auto domain = CreateDomainInstruction(domainconstants::kIncludeProcedureInstructionType);
  (void)domain->AddAttribute(domainconstants::kFileNameAttribute, "abc");
  EXPECT_TRUE(IsCollapsed(*CreateItemTreeFromDomain(*domain).root));

  (void)domain->AddAttribute(domainconstants::kShowCollapsedAttribute, "false");
  auto tree = CreateItemTreeFromDomain(*domain);
  EXPECT_FALSE(IsCollapsed(*tree.root));
  EXPECT_EQ(CreateDomainInstruction(*tree.root)
                ->GetAttributeString(domainconstants::kShowCollapsedAttribute),
            "false");

  // collapsed without showCollapsed, nothing is written
  auto item_tree = CreateInstructionItemTree(
      CreateInstructionNode(domainconstants::kIncludeProcedureInstructionType));
  EXPECT_TRUE(IsCollapsed(*item_tree.root));
  EXPECT_FALSE(CreateDomainInstruction(*item_tree.root)
                   ->HasAttribute(domainconstants::kShowCollapsedAttribute));

  SetCollapsed(false, *item_tree.root);
  EXPECT_EQ(CreateDomainInstruction(*item_tree.root)
                ->GetAttributeString(domainconstants::kShowCollapsedAttribute),
            "false");
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

  auto restored = CreateItemTreeFromDomain(*domain);
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

  auto restored = CreateItemTreeFromDomain(*domain);
  auto* anyvalue_item = GetAnyValueItem(*restored.root);
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(CreateAnyValue(*anyvalue_item), expected_anyvalue);
  EXPECT_EQ(restored.root->GetAttribute<std::string>(domainconstants::kChannelAttribute), "def");
}

}  // namespace oac_tree_gui::test
