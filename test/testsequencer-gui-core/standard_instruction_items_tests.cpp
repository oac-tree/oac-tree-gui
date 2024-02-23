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

#include "sequencergui/model/standard_instruction_items.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/model/item_utils.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

//! Tests for items from instructionitems.h

class StandardInstructionItemsTest : public ::testing::Test
{
public:
  ::sup::sequencer::Procedure m_procedure;
};

// ----------------------------------------------------------------------------
// IncludeItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, IncludeItem)
{
  // Correctly initialised item
  IncludeItem item;
  EXPECT_EQ(item.GetFileName(), std::string());
  EXPECT_EQ(item.GetPath(), std::string());
  EXPECT_TRUE(mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute));

  item.SetFileName("abc");
  EXPECT_EQ(item.GetFileName(), std::string("abc"));

  item.SetPath("def");
  EXPECT_EQ(item.GetPath(), std::string("def"));

  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to inverter
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

TEST_F(StandardInstructionItemsTest, IncludeItemFromDomain)
{
  {  // when no showCollapsed attribute is present on domain side
    auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
    input->AddAttribute(domainconstants::kFileNameAttribute, "abc");
    input->AddAttribute(domainconstants::kPathAttribute, "def");

    IncludeItem item;
    // default value for showCollapsed is true
    EXPECT_TRUE(IsCollapsed(item));

    item.InitFromDomain(input.get());

    EXPECT_EQ(item.GetFileName(), std::string("abc"));
    EXPECT_EQ(item.GetPath(), std::string("def"));

    // in the absence of showCollapsed on domain side, the value should be still true
    EXPECT_TRUE(IsCollapsed(item));
  }

  {  // when showCollapsed attribute is set to false on domain side
    auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
    input->AddAttribute(domainconstants::kFileNameAttribute, "abc");
    input->AddAttribute(domainconstants::kPathAttribute, "def");
    input->AddAttribute(domainconstants::kShowCollapsedAttribute, "false");

    IncludeItem item;
    // default value for showCollapsed is true
    EXPECT_TRUE(IsCollapsed(item));

    item.InitFromDomain(input.get());

    EXPECT_EQ(item.GetFileName(), std::string("abc"));
    EXPECT_EQ(item.GetPath(), std::string("def"));

    // after setup from domain
    EXPECT_FALSE(IsCollapsed(item));
  }

  {  // when showCollapsed attribute is set to true on domain side
    auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
    input->AddAttribute(domainconstants::kFileNameAttribute, "abc");
    input->AddAttribute(domainconstants::kPathAttribute, "def");
    input->AddAttribute(domainconstants::kShowCollapsedAttribute, "true");

    IncludeItem item;
    // default value for showCollapsed is true
    EXPECT_TRUE(IsCollapsed(item));

    item.InitFromDomain(input.get());

    EXPECT_EQ(item.GetFileName(), std::string("abc"));
    EXPECT_EQ(item.GetPath(), std::string("def"));

    // after setup from domain
    EXPECT_TRUE(IsCollapsed(item));
  }
}

//! Validating transformation of Include instruction from domain to GUI, when domain contains custom
//! parameter.

TEST_F(StandardInstructionItemsTest, IncludeItemFromDomainWithCustomAttributes)
{
  auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  input->AddAttribute(domainconstants::kFileNameAttribute, "abc");
  input->AddAttribute(domainconstants::kPathAttribute, "def");

  const std::string custom_name("par1");
  const std::string custom_value("1.0");
  input->AddAttribute(custom_name, custom_value);

  IncludeItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetFileName(), std::string("abc"));
  EXPECT_EQ(item.GetPath(), std::string("def"));

  EXPECT_TRUE(mvvm::utils::HasTag(item, custom_name));
  auto property_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(item.GetItem(custom_name));
  ASSERT_TRUE(property_item);
  EXPECT_EQ(property_item->GetAnyTypeName(), sup::dto::kStringTypeName);
  EXPECT_TRUE(std::holds_alternative<std::string>(property_item->Data()));
  EXPECT_EQ(property_item->Data<std::string>(), custom_value);
}

TEST_F(StandardInstructionItemsTest, IncludeItemToDomain)
{
  {  // is expanded
    IncludeItem item;
    item.SetPath("def");

    item.GetItem(domainconstants::kShowCollapsedAttribute)->SetData(false);

    EXPECT_FALSE(IsCollapsed(item));

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kPathAttribute), "def");
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileNameAttribute));
    EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kShowCollapsedAttribute));
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kShowCollapsedAttribute), "false");

    // Setup of Include instruction requires existance of instruction to include
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    wait->SetName("def");
    m_procedure.InsertInstruction(std::move(wait), 0);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }

  {  // is collapsed
    IncludeItem item;
    item.SetPath("def");

    EXPECT_TRUE(IsCollapsed(item));

    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kPathAttribute), "def");
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileNameAttribute));
    EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kShowCollapsedAttribute));
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kShowCollapsedAttribute), "true");

    // Setup of Include instruction requires existance of instruction to include
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    wait->SetName("def");
    m_procedure.InsertInstruction(std::move(wait), 0);

    EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  }
}

//! Validating transformation from GUI to domain, when Include instruction contains a custom
//! parameter.

TEST_F(StandardInstructionItemsTest, IncludeItemToDomainWithCustomAttribute)
{
  const std::string custom_name("par1");
  const std::string custom_value("1.0");

  // we are testing only "local include" instruction
  IncludeItem item;
  item.SetPath("def");

  auto& property = item.AddProperty<sup::gui::AnyValueScalarItem>(custom_name);
  property.SetAnyTypeName(sup::dto::kStringTypeName);
  property.SetData(custom_value);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kPathAttribute), "def");
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileNameAttribute));
  EXPECT_TRUE(domain_item->HasAttribute(custom_name));
  EXPECT_EQ(domain_item->GetAttributeString(custom_name), custom_value);

  // Setup of Input instruction requires existance of instruction to include
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(domainconstants::kTimeoutAttribute, "$par1");
  wait->SetName("def");
  auto wait_ptr = wait.get();
  m_procedure.InsertInstruction(std::move(wait), 0);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
  EXPECT_TRUE(wait_ptr->HasAttribute(domainconstants::kTimeoutAttribute));

  // FIXME Why it is $par1 and not 1.0?
  //  EXPECT_EQ(wait_ptr->GetAttributeString(domainconstants::kTimeoutAttribute), "1.0");
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, ParallelSequenceItem)
{
  // Correctly initialised item
  ParallelSequenceItem item;
  EXPECT_EQ(item.GetSuccessThreshold(), 0);
  EXPECT_EQ(item.GetFailureThreshold(), 0);

  EXPECT_FALSE(IsCollapsed(item));

  item.SetSuccessThreshold(42);
  EXPECT_EQ(item.GetSuccessThreshold(), 42);

  item.SetFailureThreshold(43);
  EXPECT_EQ(item.GetFailureThreshold(), 43);

  auto wait0 = item.InsertItem<WaitItem>({"", -1});
  auto wait1 = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait0, wait1}));
}

TEST_F(StandardInstructionItemsTest, ParallelSequenceFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kParallelInstructionType);
  input->AddAttribute(domainconstants::kSuccessThresholdAttribute, "42");
  input->AddAttribute(domainconstants::kFailureThresholdAttribute, "1");

  ParallelSequenceItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetSuccessThreshold(), 42);
  EXPECT_EQ(item.GetFailureThreshold(), 1);
}

TEST_F(StandardInstructionItemsTest, ParallelSequenceToDomain)
{
  ParallelSequenceItem item;
  item.SetSuccessThreshold(42);
  item.SetFailureThreshold(43);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kParallelInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kSuccessThresholdAttribute), "42");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFailureThresholdAttribute), "43");

  EXPECT_THROW(domain_item->Setup(m_procedure), sup::sequencer::InstructionSetupException);
}

//! This checks that no kSuccessThresholdAttribute attribute created on domain side,
//! when GUI defines this threshold -1. This is our way to deal with obscured initialisation
//! of ParallelSequence::SetupImpl

TEST_F(StandardInstructionItemsTest, ParallelSequenceToDomainWhenNoSuccessThresholdDefined)
{
  ParallelSequenceItem item;
  item.SetSuccessThreshold(0);
  item.SetFailureThreshold(43);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kParallelInstructionType);
  EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kSuccessThresholdAttribute));
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kSuccessThresholdAttribute), "0");
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFailureThresholdAttribute), "43");

  EXPECT_THROW(domain_item->Setup(m_procedure), sup::sequencer::InstructionSetupException);
}

// ----------------------------------------------------------------------------
// RepeatItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, RepeatItem)
{
  // Correctly initialised item
  sequencergui::RepeatItem item;
  EXPECT_EQ(item.GetRepeatCount(), -1);

  item.SetRepeatCount(42);
  EXPECT_EQ(item.GetRepeatCount(), 42);

  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to repeater
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

TEST_F(StandardInstructionItemsTest, RepeatItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  input->AddAttribute(domainconstants::kMaxCountAttribute, "42");

  RepeatItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetRepeatCount(), 42);
}

TEST_F(StandardInstructionItemsTest, RepeatItemToDomain)
{
  RepeatItem item;
  item.SetRepeatCount(42);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kRepeatInstructionType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kMaxCountAttribute), "42");

  EXPECT_THROW(domain_item->Setup(m_procedure), sup::sequencer::InstructionSetupException);
}

// ----------------------------------------------------------------------------
// SequenceItem tests
// ----------------------------------------------------------------------------

//! Sequence item.

TEST_F(StandardInstructionItemsTest, SequenceItem)
{
  // Correctly initialised item
  SequenceItem item;

  EXPECT_FALSE(item.IsRoot());

  auto wait0 = item.InsertItem<WaitItem>({"", -1});
  auto wait1 = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait0, wait1}));
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, SequenceItemToDomain)
{
  {  // when IsRoot and collapsed = true
    SequenceItem item;
    item.SetIsRootFlag(true);
    item.SetAttribute(domainconstants::kShowCollapsedAttribute, true);
    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kSequenceInstructionType);

    EXPECT_THROW(domain_item->Setup(m_procedure), sup::sequencer::InstructionSetupException);
    // validating that no SessionItem related properties were propagated to the domain
    std::vector<std::pair<std::string, std::string>> expected = {{"isRoot", "true"},
                                                                 {"showCollapsed", "true"}};
    EXPECT_EQ(domain_item->GetStringAttributes(), expected);
  }

  {  // when IsRoot and collapsed = false
    SequenceItem item;
    auto domain_item = item.CreateDomainInstruction();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kSequenceInstructionType);

    EXPECT_THROW(domain_item->Setup(m_procedure), sup::sequencer::InstructionSetupException);
    // validating that no SessionItem related properties were propagated to the domain
    std::vector<std::pair<std::string, std::string>> expected = {};
    EXPECT_EQ(domain_item->GetStringAttributes(), expected);
  }
}

// ----------------------------------------------------------------------------
// WaitItem tests
// ----------------------------------------------------------------------------

//! Validate WaitItem creation from the domain object.

TEST_F(StandardInstructionItemsTest, WaitItemFromDomain)
{
  // wait with timeout attribute
  {
    WaitItem wait_item;
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    wait->AddAttribute(domainconstants::kTimeoutAttribute, "42");

    wait_item.InitFromDomain(wait.get());
    EXPECT_EQ(wait_item.GetTimeout(), 42.0);
  }

  // Wait instruction without timeout without attribute
  {
    WaitItem wait_item;
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    EXPECT_NO_THROW(wait_item.InitFromDomain(wait.get()));
    EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  }

  // Wait instruction with name
  {
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    wait->AddAttribute(domainconstants::kNameAttribute, "First");

    WaitItem wait_item;
    wait_item.InitFromDomain(wait.get());

    EXPECT_EQ(wait_item.GetDisplayName(), "Wait");
    EXPECT_EQ(wait_item.Property<std::string>(itemconstants::kName), "First");
    EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  }
}

//! Validate WaitItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, WaitItemToDomain)
{
  // Correctly initialised item
  const WaitItem wait_item;
  EXPECT_EQ(wait_item.GetTimeout(), 0.0);

  auto domain_item = wait_item.CreateDomainInstruction();
  EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kTimeoutAttribute));
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "0.0");
  EXPECT_EQ(domain_item->GetType(), domainconstants::kWaitInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

//! Validate WaitItem conversion to the domain object, when timeout is defined as varying parameter.

TEST_F(StandardInstructionItemsTest, WaitItemToDomainVaryingTimeout)
{
  // Correctly initialised item
  const WaitItem item;

  auto property_item =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(item.GetItem(domainconstants::kTimeoutAttribute));
  ASSERT_TRUE(property_item);

  SetAttributeAsString("$par1", *property_item);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kTimeoutAttribute));
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTimeoutAttribute), "$par1");
  EXPECT_EQ(domain_item->GetType(), domainconstants::kWaitInstructionType);
}

//! Validate WaitItem IsRoot attribute convertion to the domain object.
//! We do not want IsRoot appears in the list of domain attribute, if it is false.

TEST_F(StandardInstructionItemsTest, WaitItemToDomainIsRootAttribute)
{
  {  // set as IsRoot
    WaitItem wait_item;
    wait_item.SetIsRootFlag(true);
    auto domain_item = wait_item.CreateDomainInstruction();
    EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kIsRootAttribute));
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kIsRootAttribute), "true");
  }

  {  // set as IsRoot
    WaitItem wait_item;
    wait_item.SetIsRootFlag(false);
    auto domain_item = wait_item.CreateDomainInstruction();
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kIsRootAttribute));
  }
}
