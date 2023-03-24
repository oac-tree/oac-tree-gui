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

#include "sequencergui/model/standard_instruction_items.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/core/exceptions.h>

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
  auto input = CreateDomainInstruction(domainconstants::kConditionInstructionType);
  input->AddAttribute(domainconstants::kConditionVarNameAttribute, "abc");
  input->AddAttribute(domainconstants::kIsRootAttribute, "true");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kConditionInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kConditionVarNameAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kCopyInstructionType);
  input->AddAttribute(domainconstants::kInputAttribute, "abc");
  input->AddAttribute(domainconstants::kOutputAttribute, "cde");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kCopyInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kInputAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kOutputAttribute), "cde");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kEqualsInstructionType);
  input->AddAttribute(domainconstants::kLeftHandAttribute, "abc");
  input->AddAttribute(domainconstants::kRightHandAttribute, "cde");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kEqualsInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kLeftHandAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kRightHandAttribute), "cde");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// FallbackItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, FallbackItem)
{
  // Correctly initialised item
  FallbackItem item;
  auto wait = item.InsertItem<WaitItem>({"", -1});
  auto sequence = item.InsertItem<SequenceItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait, sequence}));
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, FallbackItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kFallbackInstructionType);
  input->AddAttribute(domainconstants::kNameAttribute, "abc");

  // Correctly initialised item
  FallbackItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetName(), std::string("abc"));
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, FallbackItemToDomain)
{
  // Correctly initialised item
  FallbackItem item;

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), sequencergui::domainconstants::kFallbackInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  // it's not possible to add second item to ForceSuccess
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, ForceSuccessItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kForceSuccessInstructionType);
  input->AddAttribute(domainconstants::kNameAttribute, "abc");

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

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), sequencergui::domainconstants::kForceSuccessInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

TEST_F(StandardInstructionItemsTest, IncludeItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  input->AddAttribute(domainconstants::kFileAttribute, "abc");
  input->AddAttribute(domainconstants::kPathAttribute, "def");

  IncludeItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetFileName(), std::string("abc"));
  EXPECT_EQ(item.GetPath(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, IncludeItemToDomain)
{
  // we are testing only "local include" instruction
  IncludeItem item;
  item.SetPath("def");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kIncludeInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kPathAttribute), "def");
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kFileAttribute));

  // Setup of Input instruction requires existance of instruction to include
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->SetName("def");
  m_procedure.InsertInstruction(wait.release(), 0);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kInputInstructionType);
  input->AddAttribute(domainconstants::kDescriptionAttribute, "abc");
  input->AddAttribute(domainconstants::kInputTargetAttribute, "var");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kInputInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kDescriptionAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kInputTargetAttribute), "var");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

//! Validate SequenceItem convertion to the domain object.

TEST_F(StandardInstructionItemsTest, InverterItemToDomain)
{
  // Correctly initialised item
  sequencergui::InverterItem item;

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), sequencergui::domainconstants::kInverterInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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

  auto wait = item.InsertItem<WaitItem>({"", -1});
  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait}));
  // it's not possible to add second item to ForceSuccess
  EXPECT_THROW(item.InsertItem<WaitItem>({"", -1}), mvvm::InvalidOperationException);
}

TEST_F(StandardInstructionItemsTest, ListenItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kListenInstructionType);
  input->AddAttribute(domainconstants::kListenVarNamesAttribute, "abc");
  input->AddAttribute(domainconstants::kListenForceSuccessAttribute, "true");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kListenInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kListenVarNamesAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kListenForceSuccessAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  input->AddAttribute(domainconstants::kTextAttribute, "abc");
  input->AddAttribute(domainconstants::kIsRootAttribute, "true");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kMessageInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTextAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kIsRootAttribute), "true");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kOutputInstructionType);
  input->AddAttribute(domainconstants::kDescriptionAttribute, "abc");
  input->AddAttribute(domainconstants::kOutputSourceAttribute, "var");

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
  EXPECT_EQ(domain_item->GetType(), domainconstants::kOutputInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kDescriptionAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kOutputSourceAttribute), "var");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

// ----------------------------------------------------------------------------
// ParallelSequenceItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, ParallelSequenceItem)
{
  // Correctly initialised item
  ParallelSequenceItem item;
  EXPECT_EQ(item.GetSuccessThreshold(), -1);
  EXPECT_EQ(item.GetFailureThreshold(), 1);

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
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kSuccessThresholdAttribute), "42");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kFailureThresholdAttribute), "43");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

//! This checks that no kSuccessThresholdAttribute attribute created on domain side,
//! when GUI defines this threshold -1. This is our way to deal with obscured initialisation
//! of ParallelSequence::SetupImpl

TEST_F(StandardInstructionItemsTest, ParallelSequenceToDomainWhenNoSuccessThresholdDefined)
{
  ParallelSequenceItem item;
  item.SetSuccessThreshold(-1);
  item.SetFailureThreshold(43);

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kParallelInstructionType);
  EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kSuccessThresholdAttribute));
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kFailureThresholdAttribute), "43");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kMaxCountAttribute), "42");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  const SequenceItem item;

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kSequenceInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
  auto input = CreateDomainInstruction(domainconstants::kUserChoiceInstructionType);
  input->AddAttribute(domainconstants::kDescriptionAttribute, "abc");

  UserChoiceItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetDescription(), "abc");
}

TEST_F(StandardInstructionItemsTest, UserChoiceItemToDomain)
{
  UserChoiceItem item;
  item.SetDescription("abc");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kUserChoiceInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kDescriptionAttribute), "abc");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
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
    auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    wait->AddAttribute(domainconstants::kWaitTimeoutAttribute, "42");

    wait_item.InitFromDomain(wait.get());
    EXPECT_EQ(wait_item.GetTimeout(), 42.0);
  }

  // Wait instruction without timeout without attribute
  {
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
    EXPECT_EQ(wait_item.Property<std::string>("kName"), "First");
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
  EXPECT_TRUE(domain_item->HasAttribute(domainconstants::kWaitTimeoutAttribute));
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kWaitTimeoutAttribute), "0.0");
  EXPECT_EQ(domain_item->GetType(), domainconstants::kWaitInstructionType);

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}

//! UnknownInstructionItem tests.
//! Here we pretend that Condition insrtuction is uknown for a GUI, and check how
//! UnknownInstructionItem behaves on the way from/to domain.

TEST_F(StandardInstructionItemsTest, UnknownInstructionFromConditionItem)
{
  auto domain_item = CreateDomainInstruction(domainconstants::kConditionInstructionType);
  domain_item->AddAttribute(domainconstants::kConditionVarNameAttribute, "abc");

  // from domain
  UnknownInstructionItem item;
  item.InitFromDomain(domain_item.get());
  EXPECT_EQ(item.Property<std::string>(domainconstants::kConditionVarNameAttribute),
            std::string("abc"));

  // to domain
  auto new_domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(new_domain_item->GetType(), domainconstants::kConditionInstructionType);
  EXPECT_EQ(new_domain_item->GetAttribute(domainconstants::kConditionVarNameAttribute), "abc");
}

//! Testing if clone method is implemented

TEST_F(StandardInstructionItemsTest, Clone)
{
  using testutils::IsCloneImplemented;

  EXPECT_TRUE(IsCloneImplemented<ConditionItem>());
  EXPECT_TRUE(IsCloneImplemented<CopyItem>());
  EXPECT_TRUE(IsCloneImplemented<EqualsItem>());
  EXPECT_TRUE(IsCloneImplemented<FallbackItem>());
  EXPECT_TRUE(IsCloneImplemented<ForceSuccessItem>());
  EXPECT_TRUE(IsCloneImplemented<IncludeItem>());
  EXPECT_TRUE(IsCloneImplemented<InputItem>());
  EXPECT_TRUE(IsCloneImplemented<InverterItem>());
  EXPECT_TRUE(IsCloneImplemented<ListenItem>());
  EXPECT_TRUE(IsCloneImplemented<MessageItem>());
  EXPECT_TRUE(IsCloneImplemented<OutputItem>());
  EXPECT_TRUE(IsCloneImplemented<ParallelSequenceItem>());
  EXPECT_TRUE(IsCloneImplemented<RepeatItem>());
  EXPECT_TRUE(IsCloneImplemented<SequenceItem>());
  EXPECT_TRUE(IsCloneImplemented<UnknownInstructionItem>());
  EXPECT_TRUE(IsCloneImplemented<UserChoiceItem>());
  EXPECT_TRUE(IsCloneImplemented<WaitItem>());
}
