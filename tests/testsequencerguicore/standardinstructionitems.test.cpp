/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standardinstructionitems.h"

#include "Instruction.h"
#include "Procedure.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for items from instructionitems.h

class StandardInstructionItemsTest : public ::testing::Test
{
public:
  //! Returns true if domain instruction is valid.
  bool IsValid(::sup::sequencer::Instruction* instruction)
  {
    return instruction->Setup(m_procedure);
  }

  ::sup::sequencer::Procedure m_procedure;
};

// ----------------------------------------------------------------------------
// ConditionItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, ConditionItem)
{
  // Correctly initialised item
  ConditionItem item;
  EXPECT_EQ(item.GetVariableName(), std::string());

  item.SetVariableName("abc");
  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
}

TEST_F(StandardInstructionItemsTest, ConditionItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kConditionInstructionType);
  input->AddAttribute(DomainConstants::kConditionVarNameAttribute, "abc");
  input->AddAttribute(DomainConstants::kIsRootAttribute, "true");

  ConditionItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVariableName(), std::string("abc"));
  EXPECT_TRUE(item.IsRoot());
}

TEST_F(StandardInstructionItemsTest, ConditionItemToDomain)
{
  ConditionItem item;
  item.SetVariableName("abc");
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kConditionInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kConditionVarNameAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kIsRootAttribute), "true");
}

// ----------------------------------------------------------------------------
// CopyItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, CopyItem)
{
  // Correctly initialised item
  sequencergui::CopyItem item;
  EXPECT_EQ(item.GetInput(), std::string());
  item.SetInput("abc");
  EXPECT_EQ(item.GetInput(), std::string("abc"));

  EXPECT_EQ(item.GetOutput(), std::string());
  item.SetOutput("cde");
  EXPECT_EQ(item.GetOutput(), std::string("cde"));
}

TEST_F(StandardInstructionItemsTest, CopyItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kCopyInstructionType);
  input->AddAttribute(DomainConstants::kInputAttribute, "abc");
  input->AddAttribute(DomainConstants::kOutputAttribute, "cde");

  CopyItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetInput(), std::string("abc"));
  EXPECT_EQ(item.GetOutput(), std::string("cde"));
}

TEST_F(StandardInstructionItemsTest, CopyItemToDomain)
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
// EqualsItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, EqualsItem)
{
  EqualsItem item;
  EXPECT_EQ(item.GetLeftHandSide(), std::string());
  item.SetLeftHandSide("abc");
  EXPECT_EQ(item.GetLeftHandSide(), std::string("abc"));

  EXPECT_EQ(item.GetRightHandSide(), std::string());
  item.SetRightHandSide("cde");
  EXPECT_EQ(item.GetRightHandSide(), std::string("cde"));
}

TEST_F(StandardInstructionItemsTest, EqualsItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kEqualsInstructionType);
  input->AddAttribute(DomainConstants::kLeftHandAttribute, "abc");
  input->AddAttribute(DomainConstants::kRightHandAttribute, "cde");

  EqualsItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetLeftHandSide(), std::string("abc"));
  EXPECT_EQ(item.GetRightHandSide(), std::string("cde"));
}

TEST_F(StandardInstructionItemsTest, EqualsItemToDomain)
{
  EqualsItem item;
  item.SetLeftHandSide("abc");
  item.SetRightHandSide("cde");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kEqualsInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kLeftHandAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kRightHandAttribute), "cde");
}

// ----------------------------------------------------------------------------
// FallbackItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, FallbackItem)
{
  // Correctly initialised item
  sequencergui::FallbackItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  auto sequence = item.InsertItem<SequenceItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait, sequence}));
}

// ----------------------------------------------------------------------------
// ForceSuccess tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, ForceSuccessItem)
{
  // Correctly initialised item
  ForceSuccessItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to inverter
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, ForceSuccessItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kForceSuccessInstructionType);
  input->AddAttribute(DomainConstants::kNameAttribute, "abc");

  // Correctly initialised item
  ForceSuccessItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetName(), std::string("abc"));
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, ForceSuccessItemToDomain)
{
  // Correctly initialised item
  ForceSuccessItem item;

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(),
            sequencergui::DomainConstants::kForceSuccessInstructionType);
}

// ----------------------------------------------------------------------------
// IncludeItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, IncludeItem)
{
  // Correctly initialised item
  IncludeItem item;
  EXPECT_EQ(item.GetFileName(), std::string());
  EXPECT_EQ(item.GetPath(), std::string());

  item.SetFileName("abc");
  EXPECT_EQ(item.GetFileName(), std::string("abc"));

  item.SetPath("def");
  EXPECT_EQ(item.GetPath(), std::string("def"));

  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to inverter
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

TEST_F(StandardInstructionItemsTest, IncludeItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kIncludeInstructionType);
  input->AddAttribute(DomainConstants::kFileAttribute, "abc");
  input->AddAttribute(DomainConstants::kPathAttribute, "def");

  IncludeItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetFileName(), std::string("abc"));
  EXPECT_EQ(item.GetPath(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, IncludeItemToDomain)
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

TEST_F(StandardInstructionItemsTest, InputItem)
{
  // Correctly initialised item
  sequencergui::InputItem item;
  EXPECT_TRUE(item.GetDescription().empty());
  EXPECT_TRUE(item.GetTargetVariableName().empty());
}

TEST_F(StandardInstructionItemsTest, InputItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kInputInstructionType);
  input->AddAttribute(DomainConstants::kDescriptionAttribute, "abc");
  input->AddAttribute(DomainConstants::kInputTargetAttribute, "var");

  InputItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
  EXPECT_EQ(item.GetTargetVariableName(), "var");
}

TEST_F(StandardInstructionItemsTest, InputItemToDomain)
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

TEST_F(StandardInstructionItemsTest, InverterItem)
{
  // Correctly initialised item
  sequencergui::InverterItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to inverter
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, InverterItemToDomain)
{
  // Correctly initialised item
  sequencergui::InverterItem item;

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), sequencergui::DomainConstants::kInverterInstructionType);
}

// ----------------------------------------------------------------------------
// ListenItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, ListenItem)
{
  ListenItem item;

  EXPECT_FALSE(item.IsForceSuccess());
  item.SetForceSuccess(true);
  EXPECT_TRUE(item.IsForceSuccess());

  EXPECT_TRUE(item.GetVarNames().empty());
  item.SetVarNames("abc");
  EXPECT_EQ(item.GetVarNames(), "abc");
}

TEST_F(StandardInstructionItemsTest, ListenItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kListenInstructionType);
  input->AddAttribute(DomainConstants::kListenVarNamesAttribute, "abc");
  input->AddAttribute(DomainConstants::kListenForceSuccessAttribute, "true");

  ListenItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetVarNames(), "abc");
  EXPECT_TRUE(item.IsForceSuccess());
}

TEST_F(StandardInstructionItemsTest, ListenItemToDomain)
{
  ListenItem item;
  item.SetForceSuccess(true);
  item.SetVarNames("abc");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kListenInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kListenVarNamesAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kListenForceSuccessAttribute), "true");

  EXPECT_TRUE(IsValid(domain_item.get()));
}

// ----------------------------------------------------------------------------
// MessageItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, MessageItem)
{
  // Correctly initialised item
  MessageItem item;
  EXPECT_EQ(item.GetText(), std::string());

  item.SetText("abc");
  EXPECT_EQ(item.GetText(), std::string("abc"));
}

TEST_F(StandardInstructionItemsTest, MessageItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kMessageInstructionType);
  input->AddAttribute(DomainConstants::kTextAttribute, "abc");
  input->AddAttribute(DomainConstants::kIsRootAttribute, "true");

  MessageItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetText(), std::string("abc"));
  EXPECT_TRUE(item.IsRoot());
}

TEST_F(StandardInstructionItemsTest, MessageItemToDomain)
{
  MessageItem item;
  item.SetText("abc");
  item.SetIsRootFlag(true);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kMessageInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kTextAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kIsRootAttribute), "true");
}

// ----------------------------------------------------------------------------
// OutputItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, OutputItem)
{
  // Correctly initialised item
  sequencergui::OutputItem item;
  EXPECT_TRUE(item.GetDescription().empty());
  EXPECT_TRUE(item.GetSourceVariableName().empty());
}

TEST_F(StandardInstructionItemsTest, OutputItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kOutputInstructionType);
  input->AddAttribute(DomainConstants::kDescriptionAttribute, "abc");
  input->AddAttribute(DomainConstants::kOutputSourceAttribute, "var");

  OutputItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
  EXPECT_EQ(item.GetSourceVariableName(), "var");
}

TEST_F(StandardInstructionItemsTest, OutputItemToDomain)
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

TEST_F(StandardInstructionItemsTest, ParallelSequenceItem)
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

TEST_F(StandardInstructionItemsTest, ParallelSequenceFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kParallelInstructionType);
  input->AddAttribute(DomainConstants::kSuccessThresholdAttribute, "42");
  input->AddAttribute(DomainConstants::kFailureThresholdAttribute, "1");

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
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kParallelInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kSuccessThresholdAttribute), "42");
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kFailureThresholdAttribute), "43");
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
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), std::runtime_error);
}

TEST_F(StandardInstructionItemsTest, RepeatItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kRepeatInstructionType);
  input->AddAttribute(DomainConstants::kMaxCountAttribute, "42");

  RepeatItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetRepeatCount(), 42);
}

TEST_F(StandardInstructionItemsTest, RepeatItemToDomain)
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
  // Correctly initialised item
  SequenceItem item;

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), DomainConstants::kSequenceInstructionType);
}

// ----------------------------------------------------------------------------
// UserChoiceItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, UserChoiceItem)
{
  // Correctly initialised item
  UserChoiceItem item;
  EXPECT_TRUE(item.GetDescription().empty());

  auto wait0 = item.InsertItem<WaitItem>({"", -1});
  auto wait1 = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait0, wait1}));

  item.SetDescription("abc");
  EXPECT_EQ(item.GetDescription(), std::string("abc"));
}

TEST_F(StandardInstructionItemsTest, UserChoiceItemFromDomain)
{
  auto input = DomainUtils::CreateDomainInstruction(DomainConstants::kUserChoiceInstructionType);
  input->AddAttribute(DomainConstants::kDescriptionAttribute, "abc");

  UserChoiceItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
}

TEST_F(StandardInstructionItemsTest, UserChoiceItemToDomain)
{
  UserChoiceItem item;
  item.SetDescription("abc");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kUserChoiceInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kDescriptionAttribute), "abc");
}

// ----------------------------------------------------------------------------
// WaitItem tests
// ----------------------------------------------------------------------------

//! Validate WaitItem creation from the domain object.

TEST_F(StandardInstructionItemsTest, WaitItemFromDomain)
{
  // Correctly initialised item
  WaitItem wait_item;
  EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  EXPECT_EQ(wait_item.GetDisplayName(), "Wait");

  // wait with timeout attribute
  {
    auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
    wait->AddAttribute(DomainConstants::kWaitTimeoutAttribute, "42");

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
    wait->AddAttribute(DomainConstants::kNameAttribute, "First");

    WaitItem wait_item;
    wait_item.InitFromDomain(wait.get());

    EXPECT_EQ(wait_item.GetDisplayName(), "Wait");
    EXPECT_EQ(wait_item.Property<std::string>("kName"), "First");
    EXPECT_EQ(wait_item.GetTimeout(), 0.0);
  }
}

//! Validate WaitItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, WaitItemToDomain)
{
  // Correctly initialised item
  WaitItem wait_item;
  EXPECT_EQ(wait_item.GetTimeout(), 0.0);

  auto domain_instruction = wait_item.CreateDomainInstruction();
  EXPECT_TRUE(domain_instruction->HasAttribute(DomainConstants::kWaitTimeoutAttribute));
  EXPECT_EQ(domain_instruction->GetAttribute(DomainConstants::kWaitTimeoutAttribute), "0.0");
  EXPECT_EQ(domain_instruction->GetType(), DomainConstants::kWaitInstructionType);
}
