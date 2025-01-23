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

#include "oac-tree-gui/transform/variable_item_transform_helper.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/model/standard_variable_items.h>
#include <oac-tree-gui/model/variable_item.h>
#include <oac-tree-gui/model/workspace_item.h>
#include <oac-tree-gui/transform/anyvalue_item_transform_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace_info.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace sequencergui::test
{

/**
 * @brief Tests of helper methods from variable_item_transform_helper.h
 */
class VariableItemTransformHelperTest : public ::testing::Test
{
};

//! Testing CreateVariableItem helper method.
TEST_F(VariableItemTransformHelperTest, CreateVariableItem)
{
  const std::size_t variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});
  sup::oac_tree::VariableInfo info(domainconstants::kLocalVariableType, variable_id, attributes);

  auto variable_item = CreateVariableItem(info);

  EXPECT_EQ(variable_item->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(variable_item->GetDisplayName(), expected_name);
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*variable_item), expected_anyvalue);
}

TEST_F(VariableItemTransformHelperTest, PopulateWorkspaceItem)
{
  {  // attempt to populate non-empty workspace
    WorkspaceItem workspace_item;
    workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());

    sup::oac_tree::WorkspaceInfo info;
    EXPECT_THROW(PopulateWorkspaceItem(info, &workspace_item), RuntimeException);
  }

  const std::size_t variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});

  const sup::oac_tree::VariableInfo info0(
      domainconstants::kLocalVariableType, 0,
      {{domainconstants::kTypeAttribute, expected_type}, {domainconstants::kValueAttribute, "42"}});

  const sup::oac_tree::VariableInfo info1(
      domainconstants::kLocalVariableType, 1,
      {{domainconstants::kTypeAttribute, expected_type}, {domainconstants::kValueAttribute, "43"}});

  sup::oac_tree::WorkspaceInfo workspace_info;
  workspace_info.AddVariableInfo("var0", info0);
  workspace_info.AddVariableInfo("var1", info1);

  WorkspaceItem workspace_item;
  auto index_to_variable_item = PopulateWorkspaceItem(workspace_info, &workspace_item);

  auto variable_items = workspace_item.GetVariables();
  ASSERT_EQ(index_to_variable_item.size(), 2);
  ASSERT_EQ(variable_items.size(), 2);
  EXPECT_EQ(variable_items[0], index_to_variable_item[0]);
  EXPECT_EQ(variable_items[1], index_to_variable_item[1]);

  const sup::dto::AnyValue expected_anyvalue0(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*variable_items[0]), expected_anyvalue0);

  const sup::dto::AnyValue expected_anyvalue1(sup::dto::SignedInteger32Type, 43);
  EXPECT_EQ(GetAnyValue(*variable_items[1]), expected_anyvalue1);
}

TEST_F(VariableItemTransformHelperTest, PopulateWorkspaceItemFromDomain)
{
  const std::string procedure_xml = R"RAW(
<Procedure>
  <Wait timeout="42"/>
  <Workspace>
    <Local name="counter" type='{"type":"int32"}' value="0"/>
    <Local name="one" type='{"type":"int32"}' value="1"/>
  </Workspace>
</Procedure>
)RAW";
  auto procedure = sup::oac_tree::ParseProcedureString(procedure_xml);
  procedure->Setup();

  WorkspaceItem workspace_item;
  auto variables = PopulateWorkspaceItem(procedure->GetWorkspace(), nullptr, &workspace_item);

  ASSERT_EQ(variables.size(), 2);
  ASSERT_EQ(workspace_item.GetTotalItemCount(), 2);
  // validating indices
  EXPECT_EQ(variables[0], workspace_item.GetVariables().at(0));
  EXPECT_EQ(variables[1], workspace_item.GetVariables().at(1));

  EXPECT_TRUE(test::IsEqual(*variables[0], sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}));
  EXPECT_TRUE(test::IsEqual(*variables[1], sup::dto::AnyValue{sup::dto::SignedInteger32Type, 1}));
}

}  // namespace sequencergui::test
