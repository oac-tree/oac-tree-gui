/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/instructionitems.h"

#include "Instruction.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for items from instructionitems.h

class InstructionItemsTest : public ::testing::Test
{
};

// ----------------------------------------------------------------------------
// ConditionItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, ConditionItem)
{
  // Correctly initialised item
  ConditionItem item;
  EXPECT_EQ(item.GetVariableName(), std::string());

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
}

TEST_F(InstructionItemsTest, ConditionItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kConditionInstructionType);
  input->AddAttribute(DomainConstants::kConditionVarNameAttribute, "abc");

  ConditionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
}

TEST_F(InstructionItemsTest, ConditionItemToDomain)
{
  ConditionItem item;
  item.SetVariableName("abc");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kConditionInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kConditionVarNameAttribute), "abc");
}

// ----------------------------------------------------------------------------
// CopyItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, CopyItem)
{
  // Correctly initialised item
  sequi::CopyItem item;
  EXPECT_EQ(item.GetInput(), std::string());
  item.SetInput("abc");
  EXPECT_EQ(item.GetInput(), std::string("abc"));

  EXPECT_EQ(item.GetOutput(), std::string());
  item.SetOutput("cde");
  EXPECT_EQ(item.GetOutput(), std::string("cde"));
}

TEST_F(InstructionItemsTest, CopyItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kCopyInstructionType);
  input->AddAttribute(DomainConstants::kInputAttribute, "abc");
  input->AddAttribute(DomainConstants::kOutputAttribute, "cde");

  CopyItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetInput(), std::string("abc"));
  EXPECT_EQ(item.GetOutput(), std::string("cde"));
}

TEST_F(InstructionItemsTest, CopyItemToDomain)
{
  CopyItem item;
  item.SetInput("abc");
  item.SetOutput("cde");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kCopyInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kInputAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kOutputAttribute), "cde");
}

// ----------------------------------------------------------------------------
// FallbackItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, FallbackItem)
{
  // Correctly initialised item
  sequi::FallbackItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  auto sequence = item.InsertItem<SequenceItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait, sequence}));
}

// ----------------------------------------------------------------------------
// IncludeItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, IncludeItem)
{
  // Correctly initialised item
  IncludeItem item;
  EXPECT_EQ(item.GetFileName(), std::string());
  EXPECT_EQ(item.GetPath(), std::string());

  item.SetFileName("abc");
  EXPECT_EQ(item.GetFileName(), std::string("abc"));

  item.SetPath("def");
  EXPECT_EQ(item.GetPath(), std::string("def"));
}

TEST_F(InstructionItemsTest, IncludeItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kIncludeInstructionType);
  input->AddAttribute(DomainConstants::kFileAttribute, "abc");
  input->AddAttribute(DomainConstants::kPathAttribute, "def");

  IncludeItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetFileName(), std::string("abc"));
  EXPECT_EQ(item.GetPath(), std::string("def"));
}

TEST_F(InstructionItemsTest, IncludeItemToDomain)
{
  IncludeItem item;
  item.SetFileName("abc");
  item.SetPath("def");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kIncludeInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kFileAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kPathAttribute), "def");
}

// ----------------------------------------------------------------------------
// InputItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, InputItem)
{
  // Correctly initialised item
  sequi::InputItem item;
  EXPECT_TRUE(item.GetDescription().empty());
  EXPECT_TRUE(item.GetTargetVariableName().empty());
}

TEST_F(InstructionItemsTest, InputItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kInputInstructionType);
  input->AddAttribute(DomainConstants::kDescriptionAttribute, "abc");
  input->AddAttribute(DomainConstants::kInputTargetAttribute, "var");

  InputItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
  EXPECT_EQ(item.GetTargetVariableName(), "var");
}

TEST_F(InstructionItemsTest, InputItemToDomain)
{
  InputItem item;
  item.SetDescription("abc");
  item.SetTargetVariableName("var");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kInputInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kDescriptionAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kInputTargetAttribute), "var");
}

// ----------------------------------------------------------------------------
// InverterItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, InverterItem)
{
  // Correctly initialised item
  sequi::InverterItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to inverter
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(InstructionItemsTest, InverterItemToDomain)
{
  // Correctly initialised item
  sequi::InverterItem item;

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), sequi::DomainConstants::kInverterInstructionType);
}

// ----------------------------------------------------------------------------
// OutputItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, OutputItem)
{
  // Correctly initialised item
  sequi::OutputItem item;
  EXPECT_TRUE(item.GetDescription().empty());
  EXPECT_TRUE(item.GetSourceVariableName().empty());
}

TEST_F(InstructionItemsTest, OutputItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kOutputInstructionType);
  input->AddAttribute(DomainConstants::kDescriptionAttribute, "abc");
  input->AddAttribute(DomainConstants::kOutputSourceAttribute, "var");

  OutputItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
  EXPECT_EQ(item.GetSourceVariableName(), "var");
}

TEST_F(InstructionItemsTest, OutputItemToDomain)
{
  OutputItem item;
  item.SetDescription("abc");
  item.SetSourceVariableName("var");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kOutputInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kDescriptionAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kOutputSourceAttribute), "var");
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, ParallelSequenceItem)
{
  // Correctly initialised item
  ParallelSequenceItem item;
  EXPECT_EQ(item.GetSuccessThreshold(), 0);
  EXPECT_EQ(item.GetFailureThreshold(), 0);

  item.SetSuccessThreshold(42);
  EXPECT_EQ(item.GetSuccessThreshold(), 42);

  item.SetFailureThreshold(43);
  EXPECT_EQ(item.GetFailureThreshold(), 43);

  auto wait0 = item.InsertItem<WaitItem>({"", -1});
  auto wait1 = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait0, wait1}));
}

TEST_F(InstructionItemsTest, ParallelSequenceFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kParallelInstructionType);
  input->AddAttribute(DomainConstants::kSuccessThresholdAttribute, "42");
  input->AddAttribute(DomainConstants::kFailureThresholdAttribute, "1");

  ParallelSequenceItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetSuccessThreshold(), 42);
  EXPECT_EQ(item.GetFailureThreshold(), 1);
}

TEST_F(InstructionItemsTest, ParallelSequenceToDomain)
{
  ParallelSequenceItem item;
  item.SetSuccessThreshold(42);
  item.SetFailureThreshold(43);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kParallelInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kSuccessThresholdAttribute), "42");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kFailureThresholdAttribute), "43");
}

// ----------------------------------------------------------------------------
// RepeatItem tests
// ----------------------------------------------------------------------------

TEST_F(InstructionItemsTest, RepeatItem)
{
  // Correctly initialised item
  sequi::RepeatItem item;
  EXPECT_EQ(item.GetRepeatCount(), -1);

  item.SetRepeatCount(42);
  EXPECT_EQ(item.GetRepeatCount(), 42);

  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to repeater
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

TEST_F(InstructionItemsTest, RepeatItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kRepeatInstructionType);
  input->AddAttribute(DomainConstants::kMaxCountAttribute, "42");

  RepeatItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetRepeatCount(), 42);
}

TEST_F(InstructionItemsTest, RepeatItemToDomain)
{
  RepeatItem item;
  item.SetRepeatCount(42);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kRepeatInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kMaxCountAttribute), "42");
}

// ----------------------------------------------------------------------------
// SequenceItem tests
// ----------------------------------------------------------------------------

//! Sequence item.

TEST_F(InstructionItemsTest, SequenceItem)
{
  // Correctly initialised item
  sequi::SequenceItem item;
  auto wait0 = item.InsertItem<WaitItem>({"", -1});
  auto wait1 = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait0, wait1}));
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(InstructionItemsTest, SequenceItemToDomain)
{
  // Correctly initialised item
  sequi::SequenceItem item;

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), sequi::DomainConstants::kSequenceInstructionType);
}

// ----------------------------------------------------------------------------
// WaitItem tests
// ----------------------------------------------------------------------------

//! Validate WaitItem creation from the domain object.

TEST_F(InstructionItemsTest, WaitItemFromDomain)
{
  // Correctly initialised item
  WaitItem wait_item;
  EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  EXPECT_EQ(wait_item.GetDisplayName(), "Wait");

  // wait with timeout attribute
  {
    auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "42");

    wait_item.InitFromDomain(wait.get());
    EXPECT_EQ(wait_item.GetTimeout(), 42.0);
  }

  // Wait instruction without timeout without attribute
  {
    auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    EXPECT_NO_THROW(wait_item.InitFromDomain(wait.get()));
    EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  }

  // Wait instruction with name
  {
    auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait->AddAttribute(sequi::DomainConstants::kNameAttribute, "First");

    WaitItem wait_item;
    wait_item.InitFromDomain(wait.get());

    EXPECT_EQ(wait_item.GetDisplayName(), "Wait");
    EXPECT_EQ(wait_item.Property<std::string>("kName"), "First");
    EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  }
}

//! Validate WaitItem convertion to the domain object.

TEST_F(InstructionItemsTest, WaitItemToDomain)
{
  // Correctly initialised item
  sequi::WaitItem wait_item;
  EXPECT_EQ(wait_item.GetTimeout(), 0.0);

  auto domain_instruction = wait_item.CreateDomainInstruction();
  EXPECT_TRUE(domain_instruction->HasAttribute(sequi::DomainConstants::kWaitTimeoutAttribute));
  EXPECT_EQ(domain_instruction->GetAttribute(sequi::DomainConstants::kWaitTimeoutAttribute), "0.0");
  EXPECT_EQ(domain_instruction->GetType(), sequi::DomainConstants::kWaitInstructionType);
}
