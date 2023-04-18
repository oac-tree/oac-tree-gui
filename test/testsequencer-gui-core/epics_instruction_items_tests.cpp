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
  input->AddAttribute(domainconstants::kOutputAttribute, "def");
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

  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kOutputAttribute), "def");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

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
  input->AddAttribute(domainconstants::kVarNameAttribute, "abc");
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

    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kVarNameAttribute), "abc");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

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

    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

// ----------------------------------------------------------------------------
// PVAccessReadInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, PVAccessReadInstructionItem)
{
  PVAccessReadInstructionItem item;
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

TEST_F(EpicsInstructionItemsTest, PVAccessReadInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kPVAccessReadInstructionType);
  input->AddAttribute(domainconstants::kChannelAttribute, "abc");
  input->AddAttribute(domainconstants::kOutputAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");

  PVAccessReadInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetChannel(), std::string("abc"));
  EXPECT_EQ(item.GetOutput(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
}

TEST_F(EpicsInstructionItemsTest, PVAccessReadInstructionItemToDomain)
{
  PVAccessReadInstructionItem item;
  item.SetChannel("abc");
  item.SetOutput("def");
  item.SetTimeout(42.0);
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kPVAccessReadInstructionType);

  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kOutputAttribute), "def");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// PVAccessWriteInstructionItem tests
// ----------------------------------------------------------------------------

TEST_F(EpicsInstructionItemsTest, PVAccessWriteInstructionItem)
{
  PVAccessWriteInstructionItem item;
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

TEST_F(EpicsInstructionItemsTest, PVAccessWriteInstructionItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kPVAccessWriteInstructionType);
  input->AddAttribute(domainconstants::kVarNameAttribute, "abc");
  input->AddAttribute(domainconstants::kChannelAttribute, "def");
  input->AddAttribute(domainconstants::kTimeoutAttribute, "42.0");
  input->AddAttribute(domainconstants::kTypeAttribute, "json_type");
  input->AddAttribute(domainconstants::kValueAttribute, "json_value");

  PVAccessWriteInstructionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_EQ(item.GetChannel(), std::string("def"));
  EXPECT_EQ(item.GetTimeout(), 42.0);
  EXPECT_EQ(item.GetJsonType(), std::string("json_type"));
  EXPECT_EQ(item.GetJsonValue(), std::string("json_value"));
}

TEST_F(EpicsInstructionItemsTest, PVAccessWriteInstructionItemToDomain)
{
  {  // case with variable name
    PVAccessWriteInstructionItem item;
    item.SetVariableName("abc");
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetIsRootFlag(true);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPVAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kVarNameAttribute), "abc");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // case with type and value
    const std::string expected_type(R"RAW({"type":"uint32"})RAW");
    const std::string expected_value("42");

    PVAccessWriteInstructionItem item;
    item.SetChannel("def");
    item.SetTimeout(42.0);
    item.SetJsonType(expected_type);
    item.SetJsonValue(expected_value);

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPVAccessWriteInstructionType);

    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), "def");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTimeoutAttribute), "42.0");
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}
