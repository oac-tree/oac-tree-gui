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

#include "sequencergui/model/domainutils.h"

#include "Instruction.h"

#include <gtest/gtest.h>

#include <chrono>
#include <future>

using namespace sequencergui;

class DomainUtilsTest : public ::testing::Test
{
};

TEST_F(DomainUtilsTest, IsRootInstruction)
{
  auto item = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
  EXPECT_FALSE(DomainUtils::IsRootInstruction(item.get()));

  item->AddAttribute(DomainConstants::kIsRootAttribute, "true");
  EXPECT_TRUE(DomainUtils::IsRootInstruction(item.get()));

  item->SetAttribute(DomainConstants::kIsRootAttribute, "yEs");
  EXPECT_TRUE(DomainUtils::IsRootInstruction(item.get()));

  item->SetAttribute(DomainConstants::kIsRootAttribute, "false");
  EXPECT_FALSE(DomainUtils::IsRootInstruction(item.get()));
}
