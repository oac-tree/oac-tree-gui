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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac-tree-gui/model/epics_instruction_items.h"

#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/universal_item_helper.h>
#include <oac-tree-gui/transform/anyvalue_item_transform_helper.h>
#include <oac-tree-gui/transform/attribute_item_transform_helper.h>
#include <oac-tree-gui/transform/transform_from_domain.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for items from instructionitems.h

class EpicsInstructionItemsTest : public ::testing::Test
{
public:
  //! Disables all tests in the fixture if plugin is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  ::sup::oac_tree::Procedure m_procedure;
};

// ----------------------------------------------------------------------------
// ChannelAccessReadInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, ChannelAccessReadInstructionItem)
{
  ChannelAccessReadInstructionItem item;
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_TRUE(item.GetOutput().empty());
  EXPECT_EQ(item.GetTimeout(), 1.0);

  item.SetChannel("abc");
  EXPECT_EQ(item.GetChannel(), std::string("abc"));

  item.SetOutput("def");
  EXPECT_EQ(item.GetOutput(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, ChannelAccessReadInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kChannelAccessReadInstructionType);
  input->AddAttribute(domainconstants::kChannelAttribute, "abc");
  input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");

  ChannelAccessReadInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetChannel(), std::string("abc"));
  EXPECT_EQ(item.GetOutput(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, ChannelAccessReadInstructionItemToDomain)
{
  ChannelAccessReadInstructionItem item;
  item.SetChannel("abc");
  item.SetOutput("def");
  item.SetTimeout(42.0);
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessReadInstructionType);

  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kOutputVariableNameAttribute), "def");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// ChannelAccessWriteInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, ChannelAccessWriteInstructionItem)
{
  ChannelAccessWriteInstructionItem item;
  EXPECT_TRUE(item.GetVariableName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_EQ(item.GetTimeout(), 1.0);
  EXPECT_TRUE(mvvm::utils::HasTag(item, oac_tree_gui::itemconstants::kAnyValueTag));
  EXPECT_NE(GetAnyValueItem(item), nullptr);  // by default we create empty AnyValue

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, ChannelAccessWriteInstructionItemFromDomain)
{
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto input = CreateDomainInstruction(domainconstants::kChannelAccessWriteInstructionType);
  input->AddAttribute(domainconstants::kGenericVariableNameAttribute, "abc");
  input->AddAttribute(domainconstants::kChannelAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  input->AddAttribute(domainconstants::kValueAttribute, expected_value);

  ChannelAccessWriteInstructionItem item;
  item.InitFromDomain(input.get());

  auto anyvalue_item = GetAnyValueItem(item);
  ASSERT_NE(anyvalue_item, nullptr);

  auto stored_anyvalue = CreateAnyValue(*anyvalue_item);
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_EQ(item.GetChannel(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, ChannelAccessWriteInstructionItemToDomain)
{
  {  // case with variable name
    ChannelAccessWriteInstructionItem item;
    item.SetVariableName("abc");
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetIsRootFlag(true);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kGenericVariableNameAttribute),
              "abc");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with type and value
    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
    const std::string expected_type(R"RAW({"type":"int32"})RAW");
    const std::string expected_value("42");

    ChannelAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    SetAnyValue(expected_anyvalue, item);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case when AnyValueItem marked as non-present
    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
    const std::string expected_type(R"RAW({"type":"int32"})RAW");
    const std::string expected_value("42");

    ChannelAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    SetAnyValue(expected_anyvalue, item);
    SetAttributeExposedFlag(false, *GetAnyValueItem(item));

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));

    // we have disabled AnyValueItem and didn't provide variable reference
    EXPECT_THROW(domain_item->Setup(m_procedure), sup::oac_tree::InstructionSetupException);
  }
}

// ----------------------------------------------------------------------------
// PvAccessReadInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, PvAccessReadInstructionItem)
{
  PvAccessReadInstructionItem item;
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_TRUE(item.GetOutput().empty());
  EXPECT_EQ(item.GetTimeout(), 1.0);

  item.SetChannel("abc");
  EXPECT_EQ(item.GetChannel(), std::string("abc"));

  item.SetOutput("def");
  EXPECT_EQ(item.GetOutput(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, PvAccessReadInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kPvAccessReadInstructionType);
  input->AddAttribute(domainconstants::kChannelAttribute, "abc");
  input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");

  PvAccessReadInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetChannel(), std::string("abc"));
  EXPECT_EQ(item.GetOutput(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, PvAccessReadInstructionItemToDomain)
{
  PvAccessReadInstructionItem item;
  item.SetChannel("abc");
  item.SetOutput("def");
  item.SetTimeout(42.0);
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessReadInstructionType);

  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kOutputVariableNameAttribute), "def");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, PvAccessWriteInstructionItem)
{
  PvAccessWriteInstructionItem item;
  EXPECT_TRUE(item.GetVariableName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_EQ(item.GetTimeout(), 1.0);
  EXPECT_TRUE(mvvm::utils::HasTag(item, oac_tree_gui::itemconstants::kAnyValueTag));
  EXPECT_NE(GetAnyValueItem(item), nullptr);  // by default we create empty AnyValue

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, PvAccessWriteInstructionItemFromDomain)
{
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto input = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  input->AddAttribute(domainconstants::kGenericVariableNameAttribute, "abc");
  input->AddAttribute(domainconstants::kChannelAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  input->AddAttribute(domainconstants::kValueAttribute, expected_value);

  PvAccessWriteInstructionItem item;
  item.InitFromDomain(input.get());

  auto anyvalue_item = GetAnyValueItem(item);
  ASSERT_NE(anyvalue_item, nullptr);

  auto stored_anyvalue = CreateAnyValue(*anyvalue_item);
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_EQ(item.GetChannel(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, PvAccessWriteInstructionItemToDomain)
{
  {  // case with variable name
    PvAccessWriteInstructionItem item;
    item.SetVariableName("abc");
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetIsRootFlag(true);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kGenericVariableNameAttribute),
              "abc");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with type and value
    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
    const std::string expected_type(R"RAW({"type":"int32"})RAW");
    const std::string expected_value("42");

    PvAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    SetAnyValue(expected_anyvalue, item);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case when AnyValueItem marked as non-present
    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
    const std::string expected_type(R"RAW({"type":"int32"})RAW");
    const std::string expected_value("42");

    PvAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);

    // setting variable name but disabling AnyValueItem
    item.SetVariableName("var");
    SetAnyValue(expected_anyvalue, item);
    SetAttributeExposedFlag(false, *GetAnyValueItem(item));

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

// ----------------------------------------------------------------------------
// RPCClientInstruction tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, RPCClientInstruction)
{
  RPCClientInstruction item;
  EXPECT_TRUE(item.GetService().empty());
  EXPECT_TRUE(item.GetRequestVar().empty());
  EXPECT_EQ(item.GetTimeout(), 1.0);
  EXPECT_TRUE(item.GetOutput().empty());
  EXPECT_TRUE(mvvm::utils::HasTag(item, oac_tree_gui::itemconstants::kAnyValueTag));
  EXPECT_NE(GetAnyValueItem(item), nullptr);  // by default we create empty AnyValue

  item.SetService("service");
  EXPECT_EQ(item.GetService(), std::string("service"));

  item.SetRequestVar("request");
  EXPECT_EQ(item.GetRequestVar(), std::string("request"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);

  item.SetOutput("output");
  EXPECT_EQ(item.GetOutput(), std::string("output"));
}

TEST_F(EpicsInstructionItemsTest, RPCClientInstructionFromDomain)
{
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto input = CreateDomainInstruction(domainconstants::kRPCClientInstructionType);
  input->AddAttribute(domainconstants::kServiceAttribute, "service");
  input->AddAttribute(domainconstants::kRequestAttribute, "request");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  input->AddAttribute(domainconstants::kValueAttribute, expected_value);
  input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "output");

  RPCClientInstruction item;
  item.InitFromDomain(input.get());

  auto anyvalue_item = GetAnyValueItem(item);
  ASSERT_NE(anyvalue_item, nullptr);

  EXPECT_EQ(item.GetService(), std::string("service"));
  EXPECT_EQ(item.GetRequestVar(), std::string("request"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
  EXPECT_EQ(item.GetOutput(), std::string("output"));
}

TEST_F(EpicsInstructionItemsTest, RPCClientInstructionToDomain)
{
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  {  // case with type and value
    RPCClientInstruction item;
    item.SetService("service");
    item.SetTimeout(42.0);
    item.SetIsRootFlag(true);
    SetAnyValue(expected_anyvalue, item);
    item.SetOutput("output");

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kRPCClientInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kServiceAttribute), "service");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kOutputVariableNameAttribute),
              "output");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with variable name
    RPCClientInstruction item;
    item.SetService("service");
    item.SetTimeout(42.0);
    item.SetIsRootFlag(true);
    SetAnyValue(expected_anyvalue, item);
    item.SetOutput("output");

    // setting variable name but disabling AnyValueItem
    item.SetRequestVar("abc");
    SetAnyValue(expected_anyvalue, item);
    SetAttributeExposedFlag(false, *GetAnyValueItem(item));

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kRPCClientInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kServiceAttribute), "service");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kOutputVariableNameAttribute),
              "output");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kRequestAttribute), "abc");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

// ----------------------------------------------------------------------------
// SystemCallInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, SystemCallInstructionItem)
{
  SystemCallInstructionItem item;
  EXPECT_TRUE(item.GetCommand().empty());

  item.SetCommand("abc");
  EXPECT_EQ(item.GetCommand(), std::string("abc"));
}

TEST_F(EpicsInstructionItemsTest, SystemCallInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kSystemCallInstructionType);
  input->AddAttribute(domainconstants::kCommandAttribute, "abc");

  SystemCallInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetCommand(), std::string("abc"));
}

TEST_F(EpicsInstructionItemsTest, SystemCallInstructionItemToDomain)
{
  SystemCallInstructionItem item;
  item.SetCommand("abc");
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kSystemCallInstructionType);

  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kCommandAttribute), "abc");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// LogInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, LogInstructionItem)
{
  LogInstructionItem item;
  EXPECT_TRUE(item.GetMessage().empty());
  EXPECT_TRUE(item.GetInput().empty());
  EXPECT_TRUE(item.GetSeverity().empty());

  item.SetMessage("message");
  EXPECT_EQ(item.GetMessage(), std::string("message"));

  item.SetInput("input");
  EXPECT_EQ(item.GetInput(), std::string("input"));

  item.SetSeverity("severity");
  EXPECT_EQ(item.GetSeverity(), std::string("severity"));
}

TEST_F(EpicsInstructionItemsTest, LogInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kLogInstructionType);
  input->AddAttribute(domainconstants::kMessageAttribute, "message");
  input->AddAttribute(domainconstants::kInputVariableNameAttribute, "input");
  input->AddAttribute(domainconstants::kSeverityAttribute, "severity");

  LogInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetMessage(), std::string("message"));
  EXPECT_EQ(item.GetInput(), std::string("input"));
  EXPECT_EQ(item.GetSeverity(), std::string("severity"));
}

TEST_F(EpicsInstructionItemsTest, LogInstructionItemToDomain)
{
  LogInstructionItem item;
  item.SetMessage("message");
  item.SetInput("input");
  item.SetSeverity("severity");

  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kLogInstructionType);

  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kMessageAttribute), "message");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kInputVariableNameAttribute), "input");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kSeverityAttribute), "severity");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

}  // namespace oac_tree_gui::test
