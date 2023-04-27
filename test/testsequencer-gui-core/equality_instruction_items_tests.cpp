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

#include "sequencergui/model/equality_instruction_items.h"

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
// LessThanItem tests
// ----------------------------------------------------------------------------

TEST_F(StandardInstructionItemsTest, LessThanItem)
{
  LessThanItem item;
  EXPECT_EQ(item.GetLeftHandSide(), std::string());
  item.SetLeftHandSide("abc");
  EXPECT_EQ(item.GetLeftHandSide(), std::string("abc"));

  EXPECT_EQ(item.GetRightHandSide(), std::string());
  item.SetRightHandSide("def");
  EXPECT_EQ(item.GetRightHandSide(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, LessThanItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kLessThanInstructionType);
  input->AddAttribute(domainconstants::kLeftHandAttribute, "abc");
  input->AddAttribute(domainconstants::kRightHandAttribute, "def");

  LessThanItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetLeftHandSide(), std::string("abc"));
  EXPECT_EQ(item.GetRightHandSide(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, LessThanItemToDomain)
{
  LessThanItem item;
  item.SetLeftHandSide("abc");
  item.SetRightHandSide("def");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kLessThanInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kLeftHandAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kRightHandAttribute), "def");

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
  item.SetRightHandSide("def");
  EXPECT_EQ(item.GetRightHandSide(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, EqualsItemFromDomain)
{
  auto input = CreateDomainInstruction(domainconstants::kEqualsInstructionType);
  input->AddAttribute(domainconstants::kLeftHandAttribute, "abc");
  input->AddAttribute(domainconstants::kRightHandAttribute, "def");

  EqualsItem item;
  item.InitFromDomain(input.get());

  EXPECT_EQ(item.GetLeftHandSide(), std::string("abc"));
  EXPECT_EQ(item.GetRightHandSide(), std::string("def"));
}

TEST_F(StandardInstructionItemsTest, EqualsItemToDomain)
{
  EqualsItem item;
  item.SetLeftHandSide("abc");
  item.SetRightHandSide("def");

  auto domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kEqualsInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kLeftHandAttribute), "abc");
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kRightHandAttribute), "def");

  EXPECT_NO_THROW(domain_item->Setup(m_procedure));
}
