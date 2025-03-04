/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/instruction_info_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class InstructionInfoItemTest : public ::testing::Test
{
};

TEST_F(InstructionInfoItemTest, InitialState)
{
  const InstructionInfoItem item;

  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

  EXPECT_THROW(item.CreateDomainInstruction(), sup::oac_tree::InvalidOperationException);
}

TEST_F(InstructionInfoItemTest, InitFromInstructionInfo)
{
  const std::string expected_name("MySpecialWait");
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"},
                                             {domainconstants::kNameAttribute, expected_name}});

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);
  EXPECT_EQ(item.GetDomainType(), oac_tree_gui::domainconstants::kWaitInstructionType);
  EXPECT_EQ(item.GetName(), expected_name);
}

}  // namespace oac_tree_gui::test
