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

#include <oac_tree_gui/domain/domain_automation_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

class VariableInfoItemTest : public ::testing::Test
{
};

TEST_F(VariableInfoItemTest, InitialState)
{
  const VariableInfoItem item;

  EXPECT_TRUE(item.GetDomainType().empty());
  const std::vector<std::string> expected_tags({itemconstants::kIsAvailable});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
  EXPECT_THROW(item.CreateDomainVariable(), sup::oac_tree::InvalidOperationException);
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
}

TEST_F(VariableInfoItemTest, InitFromDomainInfoBeforeSetup)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");
  const sup::dto::uint32 expected_index = 42U;
  const sup::dto::AnyValue expected_anyvalue(sup::dto::UnsignedInteger32Type, 42U);

  // constructing LocalVariable
  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  // constructing VariableInfo
  auto var_info = CreateVariableInfo(*local_variable, expected_index);
  EXPECT_EQ(var_info.GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(var_info.GetIndex(), expected_index);

  const std::vector<std::pair<std::string, std::string>> expected_attributes = {
      {domainconstants::kNameAttribute, expected_name},
      {domainconstants::kTypeAttribute, expected_type},
      {domainconstants::kValueAttribute, expected_value},
  };

  auto attributes = var_info.GetAttributes();
  EXPECT_EQ(var_info.GetAttributes(), expected_attributes);

  // populating VariableInfoItem
  VariableInfoItem item;
  item.InitFromDomainInfo(var_info);

  EXPECT_EQ(mvvm::utils::RegisteredTags(item),
            std::vector<std::string>({itemconstants::kIsAvailable, itemconstants::kAnyValueTag}));

  EXPECT_EQ(item.GetDisplayName(), expected_name);
  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(item.GetType(), VariableInfoItem::GetStaticType());
  if (itemconstants::kProvideVariableInfoInitialValue)
  {
    EXPECT_NE(item.GetAnyValueItem(), nullptr);
    EXPECT_EQ(GetAnyValue(item), expected_anyvalue);
  }
  else
  {
    EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  }
}

TEST_F(VariableInfoItemTest, InitialConnectableItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // building domain variable
  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessServerVariableItemFromDomain");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  const attribute_list_t attributes = {{domainconstants::kNameAttribute, expected_name},
                                       {domainconstants::kChannelAttribute, expected_channel},
                                       {domainconstants::kTypeAttribute, expected_datatype},
                                       {domainconstants::kValueAttribute, expected_value}};

  auto item = CreateVariableInfoItem(domainconstants::kPvAccessServerVariableType, 0, attributes);

  EXPECT_EQ(item->GetName(), expected_name);
  ASSERT_NE(GetChannelItem(*item), nullptr);
  EXPECT_EQ(GetChannelItem(*item)->Data<std::string>(), expected_channel);
  ASSERT_NE(GetIsAvailableItem(*item), nullptr);
  EXPECT_FALSE(GetIsAvailableItem(*item)->Data<bool>());
}

}  // namespace oac_tree_gui::test
