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

#include "sequencergui/model/sequenceritems.h"

#include "Instruction.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for items from instructionitems.h

class SequencerItemsTest : public ::testing::Test
{
};

//! Validate Workspace

TEST_F(SequencerItemsTest, WorkspaceItem)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariables().empty());

  auto var0 = item.InsertItem<LocalVariableItem>(ModelView::TagIndex::Append());
  auto var1 = item.InsertItem<LocalVariableItem>(ModelView::TagIndex::Append());
  EXPECT_EQ(item.GetVariables(), std::vector<VariableItem*>({var0, var1}));
}

//! UnknownInstructionItem tests.
//! Here we pretend that Condition insrtuction is uknown for a GUI, and check how
//! UnknownInstructionItem behaves on the way from/to domain.

TEST_F(SequencerItemsTest, UnknownInstructionFromConditionItem)
{
  auto domain_item =
      DomainUtils::CreateDomainInstruction(DomainConstants::kConditionInstructionType);
  domain_item->AddAttribute(DomainConstants::kConditionVarNameAttribute, "abc");

  // from domain
  UnknownInstructionItem item;
  item.InitFromDomain(domain_item.get());
  EXPECT_EQ(item.Property<std::string>(DomainConstants::kConditionVarNameAttribute),
            std::string("abc"));

  // to domain
  auto new_domain_item = item.CreateDomainInstruction();
  EXPECT_EQ(new_domain_item->GetType(), DomainConstants::kConditionInstructionType);
  EXPECT_EQ(new_domain_item->GetAttribute(DomainConstants::kConditionVarNameAttribute), "abc");
}
