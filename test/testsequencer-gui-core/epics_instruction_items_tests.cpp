/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/epics_instruction_items.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/core/exceptions.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

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

  ::sup::sequencer::Procedure m_procedure;
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
  EXPECT_TRUE(item.GetJsonType().empty());
  EXPECT_TRUE(item.GetJsonValue().empty());

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);

  item.SetJsonType("json_type");
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));

  item.SetJsonValue("json_value");
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
}

TEST_F(EpicsInstructionItemsTest, ChannelAccessWriteInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kChannelAccessWriteInstructionType);
  input->AddAttribute(domainconstants::kGenericVariableNameAttribute, "abc");
  input->AddAttribute(domainconstants::kChannelAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, "json_type");
  input->AddAttribute(domainconstants::kValueAttribute, "json_value");

  ChannelAccessWriteInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_EQ(item.GetChannel(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
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
    
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kGenericVariableNameAttribute), "abc");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with type and value
    const std::string expected_type(R"RAW({"type":"uint32"})RAW");
    const std::string expected_value("42");

    ChannelAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetJsonType(expected_type);
    item.SetJsonValue(expected_value);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  EXPECT_TRUE(item.GetJsonType().empty());
  EXPECT_TRUE(item.GetJsonValue().empty());

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);

  item.SetJsonType("json_type");
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));

  item.SetJsonValue("json_value");
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
}

TEST_F(EpicsInstructionItemsTest, PvAccessWriteInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  input->AddAttribute(domainconstants::kGenericVariableNameAttribute, "abc");
  input->AddAttribute(domainconstants::kChannelAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, "json_type");
  input->AddAttribute(domainconstants::kValueAttribute, "json_value");

  PvAccessWriteInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_EQ(item.GetChannel(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
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
    
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kGenericVariableNameAttribute), "abc");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with type and value
    const std::string expected_type(R"RAW({"type":"uint32"})RAW");
    const std::string expected_value("42");

    PvAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetJsonType(expected_type);
    item.SetJsonValue(expected_value);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

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
  EXPECT_TRUE(item.GetJsonType().empty());
  EXPECT_TRUE(item.GetJsonValue().empty());
  EXPECT_TRUE(item.GetOutput().empty());

  item.SetService("service");
  EXPECT_EQ(item.GetService(), std::string("service"));

  item.SetRequestVar("request");
  EXPECT_EQ(item.GetRequestVar(), std::string("request"));

  item.SetTimeout(42.0);
  EXPECT_EQ(item.GetTimeout(), 42.0);

  item.SetJsonType("json_type");
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));

  item.SetJsonValue("json_value");
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));

  item.SetOutput("output");
  EXPECT_EQ(item.GetOutput(), std::string("output"));
}

TEST_F(EpicsInstructionItemsTest, RPCClientInstructionFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kRPCClientInstructionType);
  input->AddAttribute(domainconstants::kServiceAttribute, "service");
  input->AddAttribute(domainconstants::kRequestAttribute, "request");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, "json_type");
  input->AddAttribute(domainconstants::kValueAttribute, "json_value");
  input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "output");

  RPCClientInstruction item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetService(), std::string("service"));
  EXPECT_EQ(item.GetRequestVar(), std::string("request"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
  EXPECT_EQ(item.GetOutput(), std::string("output"));
}

TEST_F(EpicsInstructionItemsTest, RPCClientInstructionToDomain)
{
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  // either RequestVar or type and value. Testing Type And Value case
  RPCClientInstruction item;
  item.SetService("service");
  item.SetTimeout(42.0);
  item.SetIsRootFlag(true);
  item.SetJsonType(expected_type);
  item.SetJsonValue(expected_value);
  item.SetOutput("output");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kRPCClientInstructionType);

  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kServiceAttribute), "service");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kOutputVariableNameAttribute), "output");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
