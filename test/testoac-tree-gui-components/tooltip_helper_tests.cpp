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

#include "oac_tree_gui/operation/tooltip_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class ToolTipHelperTest : public ::testing::Test
{
};

TEST_F(ToolTipHelperTest, CollectToolTipAttributes)
{
  const std::string expected_name("MySpecialWait");
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"},
                                             {domainconstants::kNameAttribute, expected_name}});

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);

  auto tooltip_attributes = CollectToolTipAttributes(&item);

  ASSERT_EQ(tooltip_attributes.size(), 1);
  EXPECT_EQ(tooltip_attributes.at(0).first, domainconstants::kTimeoutAttribute);
  EXPECT_EQ(tooltip_attributes.at(0).second, "42");
}

}  // namespace oac_tree_gui::test
