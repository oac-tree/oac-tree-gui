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

using namespace sequi;

//! Tests for items from instructionitems.h

class SequencerItemsTest : public ::testing::Test
{
};

// ----------------------------------------------------------------------------
// ConditionItem tests
// ----------------------------------------------------------------------------

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
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kConditionInstructionType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kConditionVarNameAttribute), "abc");
}
