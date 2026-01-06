/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/model/variable_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/item_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for VariableItem base class.
 */
class VariableItemTest : public ::testing::Test
{
public:
  class TestItem : public VariableItem
  {
  public:
    TestItem() : VariableItem("test") {}

    void ResiterOtherTags() { RegisterAnyValueItemTag(); }

  private:
    void InitFromDomainImpl(const variable_t* variable, const anytype_registry_t* registry) override
    {
      (void)variable;
      (void)registry;
    }

    void SetupDomainImpl(variable_t* variable) const override { (void)variable; }
  };
};

TEST_F(VariableItemTest, SetDomainType)
{
  TestItem item;

  EXPECT_EQ(item.GetType(), "test");

  EXPECT_TRUE(item.GetDomainType().empty());
  const std::string expected_domain_type = "SomeDomainVariable";
  item.SetDomainType(expected_domain_type);
  EXPECT_EQ(item.GetDomainType(), expected_domain_type);
}

TEST_F(VariableItemTest, RegisterAnyValueTags)
{
  TestItem item;

  EXPECT_EQ(mvvm::utils::RegisteredTags(item),
            std::vector<std::string>({itemconstants::kIsAvailable}));

  item.ResiterOtherTags();
  EXPECT_EQ(mvvm::utils::RegisteredTags(item),
            std::vector<std::string>({itemconstants::kIsAvailable, itemconstants::kAnyValueTag}));
}

}  // namespace oac_tree_gui::test
