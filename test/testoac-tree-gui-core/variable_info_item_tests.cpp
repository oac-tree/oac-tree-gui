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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/variable_info_item.h"

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class VariableInfoItemTest : public ::testing::Test
{
};

TEST_F(VariableInfoItemTest, InitialState)
{
  const VariableInfoItem item;

  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
  EXPECT_THROW(item.CreateDomainVariable(), sup::oac_tree::InvalidOperationException);
}

}  // namespace oac_tree_gui::test
