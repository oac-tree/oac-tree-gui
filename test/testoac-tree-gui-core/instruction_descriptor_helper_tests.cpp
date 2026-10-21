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

#include "oac_tree_gui/transform/instruction_descriptor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>

#include <sup/dto/anytype.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>

#include <algorithm>

namespace oac_tree_gui::test
{

namespace
{

//! Domain instruction with a non-scalar attribute definition.
class NonScalarAttributeInstruction : public sup::oac_tree::Instruction
{
public:
  NonScalarAttributeInstruction() : sup::oac_tree::Instruction("NonScalarAttributeInstruction")
  {
    (void)AddAttributeDefinition("data",
                                 sup::dto::AnyType{{"field", sup::dto::SignedInteger32Type}});
  }

private:
  sup::oac_tree::ExecutionStatus ExecuteSingleImpl(sup::oac_tree::UserInterface& ui,
                                                   sup::oac_tree::Workspace& ws) override
  {
    (void)ui;
    (void)ws;
    return sup::oac_tree::ExecutionStatus::SUCCESS;
  }
};

//! Returns attribute names of the descriptor.
std::vector<std::string> GetAttributeNames(const InstructionDescriptor& descriptor)
{
  std::vector<std::string> result;
  for (const auto& attribute : descriptor.attributes)
  {
    result.push_back(attribute.name);
  }
  return result;
}

//! Returns the attribute with the given name, or nullptr.
const AttributeDescriptor* FindAttribute(const InstructionDescriptor& descriptor,
                                         const std::string& name)
{
  auto iter = std::find_if(descriptor.attributes.begin(), descriptor.attributes.end(),
                           [&name](const auto& attribute) { return attribute.name == name; });
  return iter == descriptor.attributes.end() ? nullptr : &*iter;
}

//! Creates a typed descriptor for the given domain type.
InstructionDescriptor CreateTypedDescriptor(const std::string& domain_type)
{
  return CreateInstructionDescriptor(*CreateDomainInstruction(domain_type));
}

}  // namespace

//! Tests for functions from instruction_descriptor_helper.h.
class InstructionDescriptorHelperTest : public ::testing::Test
{
};

TEST_F(InstructionDescriptorHelperTest, CreateInstructionDescriptorFromWait)
{
  const auto descriptor = CreateTypedDescriptor(domainconstants::kWaitInstructionType);

  EXPECT_EQ(descriptor.domain_type, domainconstants::kWaitInstructionType);
  EXPECT_EQ(descriptor.category, sup::oac_tree::Instruction::kAction);
  const std::vector<std::string> expected_names{
      domainconstants::kNameAttribute, domainconstants::kIsRootAttribute,
      domainconstants::kTimeoutAttribute, domainconstants::kBlockingAttribute};
  EXPECT_EQ(GetAttributeNames(descriptor), expected_names);

  const auto* name = FindAttribute(descriptor, domainconstants::kNameAttribute);
  EXPECT_EQ(name->default_value, mvvm::variant_t{std::string{}});
  EXPECT_FALSE(name->is_set);
  EXPECT_FALSE(name->allow_text);

  const auto* is_root = FindAttribute(descriptor, domainconstants::kIsRootAttribute);
  EXPECT_EQ(is_root->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(is_root->mandatory);
  EXPECT_TRUE(is_root->is_set);
  EXPECT_FALSE(is_root->visible);
  EXPECT_FALSE(is_root->allow_text);

  const auto* timeout = FindAttribute(descriptor, domainconstants::kTimeoutAttribute);
  EXPECT_EQ(timeout->default_value, mvvm::variant_t{0.0});
  EXPECT_TRUE(timeout->tooltip.empty());
  EXPECT_FALSE(timeout->mandatory);
  EXPECT_FALSE(timeout->is_set);
  EXPECT_EQ(timeout->category, sup::oac_tree::AttributeCategory::kBoth);
  EXPECT_TRUE(timeout->visible);
  EXPECT_TRUE(timeout->allow_text);

  const auto* blocking = FindAttribute(descriptor, domainconstants::kBlockingAttribute);
  EXPECT_EQ(blocking->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(blocking->is_set);
  EXPECT_TRUE(blocking->allow_text);
}

TEST_F(InstructionDescriptorHelperTest, CreateInstructionDescriptorFromSequence)
{
  const auto descriptor = CreateTypedDescriptor(domainconstants::kSequenceInstructionType);

  EXPECT_EQ(descriptor.category, sup::oac_tree::Instruction::kCompound);
  const auto* show_collapsed = FindAttribute(descriptor, domainconstants::kShowCollapsedAttribute);
  ASSERT_NE(show_collapsed, nullptr);
  EXPECT_EQ(show_collapsed->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(show_collapsed->is_set);
  EXPECT_FALSE(show_collapsed->allow_text);
}

TEST_F(InstructionDescriptorHelperTest, CreateInstructionDescriptorFromRepeat)
{
  const auto descriptor = CreateTypedDescriptor(domainconstants::kRepeatInstructionType);

  EXPECT_EQ(descriptor.category, sup::oac_tree::Instruction::kDecorator);
  const auto* max_count = FindAttribute(descriptor, domainconstants::kMaxCountAttribute);
  ASSERT_NE(max_count, nullptr);
  EXPECT_EQ(max_count->default_value, mvvm::variant_t{mvvm::int32{-1}});
  EXPECT_TRUE(max_count->is_set);
}

TEST_F(InstructionDescriptorHelperTest, CreateInstructionDescriptorFromInclude)
{
  for (const auto* domain_type : {domainconstants::kIncludeInstructionType,
                                  domainconstants::kIncludeProcedureInstructionType})
  {
    const auto descriptor = CreateTypedDescriptor(domain_type);

    const auto* show_collapsed =
        FindAttribute(descriptor, domainconstants::kShowCollapsedAttribute);
    ASSERT_NE(show_collapsed, nullptr) << domain_type;
    EXPECT_EQ(show_collapsed->default_value, mvvm::variant_t{false}) << domain_type;
    EXPECT_FALSE(show_collapsed->is_set) << domain_type;
    EXPECT_FALSE(show_collapsed->allow_text) << domain_type;
  }

  const auto descriptor = CreateTypedDescriptor(domainconstants::kIncludeInstructionType);
  const auto* path = FindAttribute(descriptor, domainconstants::kPathAttribute);
  ASSERT_NE(path, nullptr);
  EXPECT_TRUE(path->mandatory);
  EXPECT_TRUE(path->is_set);
  const auto* file = FindAttribute(descriptor, domainconstants::kFileNameAttribute);
  ASSERT_NE(file, nullptr);
  EXPECT_FALSE(file->mandatory);
  EXPECT_FALSE(file->is_set);
}

TEST_F(InstructionDescriptorHelperTest, CreateInstructionDescriptorWhenTypeAndValue)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  for (const auto* domain_type :
       {domainconstants::kChannelAccessWriteInstructionType,
        domainconstants::kPvAccessWriteInstructionType, domainconstants::kRPCClientInstructionType})
  {
    const auto descriptor = CreateTypedDescriptor(domain_type);

    const auto* type = FindAttribute(descriptor, domainconstants::kTypeAttribute);
    ASSERT_NE(type, nullptr) << domain_type;
    EXPECT_EQ(type->default_value, mvvm::variant_t{std::string{}}) << domain_type;
    const auto* value = FindAttribute(descriptor, domainconstants::kValueAttribute);
    ASSERT_NE(value, nullptr) << domain_type;
    EXPECT_EQ(value->default_value, mvvm::variant_t{std::string{}}) << domain_type;
  }
}

TEST_F(InstructionDescriptorHelperTest, NonScalarDefinitionThrows)
{
  const NonScalarAttributeInstruction instruction;
  EXPECT_THROW(CreateInstructionDescriptor(instruction), RuntimeException);
}

//! All registered instruction types have scalar attribute definitions.
TEST_F(InstructionDescriptorHelperTest, AllRegisteredInstructionTypes)
{
  for (const auto& domain_type : GetDomainInstructionNames())
  {
    InstructionDescriptor descriptor;
    EXPECT_NO_THROW(descriptor = CreateTypedDescriptor(domain_type)) << domain_type;
    EXPECT_EQ(descriptor.domain_type, domain_type);
  }
}

TEST_F(InstructionDescriptorHelperTest, InfoWait)
{
  const sup::oac_tree::InstructionInfo info(domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::kAction, 0,
                                            {{domainconstants::kTimeoutAttribute, "42"}});
  const auto descriptor = CreateInstructionDescriptor(info);

  EXPECT_EQ(descriptor.domain_type, domainconstants::kWaitInstructionType);
  EXPECT_EQ(descriptor.category, sup::oac_tree::Instruction::kAction);
  const std::vector<std::string> expected_names{domainconstants::kNameAttribute,
                                                domainconstants::kIsRootAttribute};
  EXPECT_EQ(GetAttributeNames(descriptor), expected_names);
  EXPECT_TRUE(FindAttribute(descriptor, domainconstants::kIsRootAttribute)->is_set);
  EXPECT_FALSE(FindAttribute(descriptor, domainconstants::kIsRootAttribute)->allow_text);
}

TEST_F(InstructionDescriptorHelperTest, InfoSequenceAndInclude)
{
  const sup::oac_tree::InstructionInfo sequence_info(domainconstants::kSequenceInstructionType,
                                                     sup::oac_tree::Instruction::kCompound, 0, {});
  const auto sequence = CreateInstructionDescriptor(sequence_info);
  const auto* sequence_collapsed =
      FindAttribute(sequence, domainconstants::kShowCollapsedAttribute);
  ASSERT_NE(sequence_collapsed, nullptr);
  EXPECT_EQ(sequence_collapsed->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(sequence_collapsed->is_set);
  EXPECT_FALSE(sequence_collapsed->allow_text);

  const sup::oac_tree::InstructionInfo include_info(domainconstants::kIncludeInstructionType,
                                                    sup::oac_tree::Instruction::kDecorator, 0, {});
  const auto include = CreateInstructionDescriptor(include_info);
  const auto* include_collapsed = FindAttribute(include, domainconstants::kShowCollapsedAttribute);
  ASSERT_NE(include_collapsed, nullptr);
  EXPECT_EQ(include_collapsed->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(include_collapsed->is_set);
}

//! Info descriptor needs no plugin.
TEST_F(InstructionDescriptorHelperTest, InfoUnknownType)
{
  const sup::oac_tree::InstructionInfo info("UnknownInstruction",
                                            sup::oac_tree::Instruction::kCompound, 0, {});
  const auto descriptor = CreateInstructionDescriptor(info);

  EXPECT_EQ(descriptor.domain_type, "UnknownInstruction");
  const std::vector<std::string> expected_names{domainconstants::kNameAttribute,
                                                domainconstants::kIsRootAttribute,
                                                domainconstants::kShowCollapsedAttribute};
  EXPECT_EQ(GetAttributeNames(descriptor), expected_names);
}

//! Info descriptor attributes match the definitions of the domain base classes.
TEST_F(InstructionDescriptorHelperTest, InfoMatchesTypedBaseAttributes)
{
  for (const auto* domain_type :
       {domainconstants::kWaitInstructionType, domainconstants::kSequenceInstructionType,
        domainconstants::kRepeatInstructionType, domainconstants::kIncludeInstructionType})
  {
    const auto typed = CreateTypedDescriptor(domain_type);
    const sup::oac_tree::InstructionInfo info(domain_type, typed.category, 0, {});
    const auto descriptor = CreateInstructionDescriptor(info);

    for (const auto& attribute : descriptor.attributes)
    {
      const auto* expected = FindAttribute(typed, attribute.name);
      ASSERT_NE(expected, nullptr) << domain_type << "/" << attribute.name;
      EXPECT_EQ(attribute.default_value, expected->default_value)
          << domain_type << "/" << attribute.name;
      EXPECT_EQ(attribute.mandatory, expected->mandatory) << domain_type << "/" << attribute.name;
      EXPECT_EQ(attribute.is_set, expected->is_set) << domain_type << "/" << attribute.name;
      EXPECT_EQ(attribute.category, expected->category) << domain_type << "/" << attribute.name;
      EXPECT_EQ(attribute.visible, expected->visible) << domain_type << "/" << attribute.name;
    }
  }
}

TEST_F(InstructionDescriptorHelperTest, NodeFromDomainType)
{
  const auto node = CreateInstructionNode(domainconstants::kWaitInstructionType);

  EXPECT_EQ(node.descriptor.domain_type, domainconstants::kWaitInstructionType);
  EXPECT_EQ(GetAttributeNames(node.descriptor),
            GetAttributeNames(CreateTypedDescriptor(domainconstants::kWaitInstructionType)));
  EXPECT_TRUE(node.attributes.empty());
  EXPECT_EQ(node.index, 0);
  EXPECT_TRUE(node.children.empty());
}

TEST_F(InstructionDescriptorHelperTest, NodeFromUnknownDomainTypeThrows)
{
  EXPECT_THROW(CreateInstructionNode("UnknownInstruction"),
               sup::oac_tree::InvalidOperationException);
}

//! Node tree from a domain instruction: typed descriptors, values, depth-first indexes.
TEST_F(InstructionDescriptorHelperTest, NodeTreeFromDomainInstruction)
{
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  (void)sequence->AddAttribute(domainconstants::kNameAttribute, "main");
  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  (void)repeat->AddAttribute(domainconstants::kMaxCountAttribute, "3");
  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  (void)wait0->AddAttribute(domainconstants::kTimeoutAttribute, "42");
  (void)repeat->InsertInstruction(std::move(wait0), 0);
  (void)sequence->InsertInstruction(std::move(repeat), 0);
  auto wait1 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  (void)wait1->AddAttribute(domainconstants::kTimeoutAttribute, "$par");
  (void)sequence->InsertInstruction(std::move(wait1), 1);

  const auto node = CreateInstructionNodeTree(*sequence);

  EXPECT_EQ(node.descriptor.domain_type, domainconstants::kSequenceInstructionType);
  const std::vector<sup::oac_tree::AttributeInfo> expected_sequence_attributes{
      {domainconstants::kNameAttribute, "main"}};
  EXPECT_EQ(node.attributes, expected_sequence_attributes);
  EXPECT_EQ(node.index, 0);
  ASSERT_EQ(node.children.size(), 2);

  const auto& repeat_node = node.children.at(0);
  EXPECT_EQ(repeat_node.descriptor.domain_type, domainconstants::kRepeatInstructionType);
  const std::vector<sup::oac_tree::AttributeInfo> expected_repeat_attributes{
      {domainconstants::kMaxCountAttribute, "3"}};
  EXPECT_EQ(repeat_node.attributes, expected_repeat_attributes);
  EXPECT_EQ(repeat_node.index, 1);
  ASSERT_EQ(repeat_node.children.size(), 1);

  const auto& wait0_node = repeat_node.children.at(0);
  EXPECT_EQ(wait0_node.descriptor.domain_type, domainconstants::kWaitInstructionType);
  const std::vector<sup::oac_tree::AttributeInfo> expected_wait0_attributes{
      {domainconstants::kTimeoutAttribute, "42"}};
  EXPECT_EQ(wait0_node.attributes, expected_wait0_attributes);
  EXPECT_EQ(wait0_node.index, 2);
  EXPECT_TRUE(wait0_node.children.empty());

  const auto& wait1_node = node.children.at(1);
  const std::vector<sup::oac_tree::AttributeInfo> expected_wait1_attributes{
      {domainconstants::kTimeoutAttribute, "$par"}};
  EXPECT_EQ(wait1_node.attributes, expected_wait1_attributes);
  EXPECT_EQ(wait1_node.index, 3);

  // typed descriptor
  const auto* timeout = FindAttribute(wait1_node.descriptor, domainconstants::kTimeoutAttribute);
  ASSERT_NE(timeout, nullptr);
  EXPECT_EQ(timeout->default_value, mvvm::variant_t{0.0});
}

//! Node tree from instruction info: info descriptors, values and indexes as given. No plugin.
TEST_F(InstructionDescriptorHelperTest, NodeTreeFromInstructionInfo)
{
  sup::oac_tree::InstructionInfo sequence_info(domainconstants::kSequenceInstructionType,
                                               sup::oac_tree::Instruction::kCompound, 2,
                                               {{domainconstants::kNameAttribute, "main"}});
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      "UnknownInstruction", sup::oac_tree::Instruction::kAction, 0,
      std::vector<sup::oac_tree::AttributeInfo>{{"custom", "1"}}));
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      domainconstants::kWaitInstructionType, sup::oac_tree::Instruction::kAction, 1,
      std::vector<sup::oac_tree::AttributeInfo>{{domainconstants::kTimeoutAttribute, "42"}}));

  const auto node = CreateInstructionNodeTree(sequence_info);

  EXPECT_EQ(node.descriptor.domain_type, domainconstants::kSequenceInstructionType);
  EXPECT_EQ(node.descriptor.category, sup::oac_tree::Instruction::kCompound);
  const std::vector<sup::oac_tree::AttributeInfo> expected_sequence_attributes{
      {domainconstants::kNameAttribute, "main"}};
  EXPECT_EQ(node.attributes, expected_sequence_attributes);
  EXPECT_EQ(node.index, 2);
  ASSERT_EQ(node.children.size(), 2);

  const auto& unknown_node = node.children.at(0);
  EXPECT_EQ(unknown_node.descriptor.domain_type, "UnknownInstruction");
  const std::vector<std::string> expected_names{domainconstants::kNameAttribute,
                                                domainconstants::kIsRootAttribute};
  EXPECT_EQ(GetAttributeNames(unknown_node.descriptor), expected_names);
  const std::vector<sup::oac_tree::AttributeInfo> expected_unknown_attributes{{"custom", "1"}};
  EXPECT_EQ(unknown_node.attributes, expected_unknown_attributes);
  EXPECT_EQ(unknown_node.index, 0);

  const auto& wait_node = node.children.at(1);
  EXPECT_EQ(wait_node.descriptor.domain_type, domainconstants::kWaitInstructionType);
  EXPECT_EQ(GetAttributeNames(wait_node.descriptor), expected_names);
  const std::vector<sup::oac_tree::AttributeInfo> expected_wait_attributes{
      {domainconstants::kTimeoutAttribute, "42"}};
  EXPECT_EQ(wait_node.attributes, expected_wait_attributes);
  EXPECT_EQ(wait_node.index, 1);
}

}  // namespace oac_tree_gui::test
