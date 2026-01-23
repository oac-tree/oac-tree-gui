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

#include "oac_tree_gui/model/component_types.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of methods in component_types.h
 */
class ComponentTypesTest : public ::testing::Test
{
};

TEST_F(ComponentTypesTest, ProcedureEditorInfoEqualityOperators)
{
  {  // default constructed
    const ProcedureEditorInfo info1{};
    const ProcedureEditorInfo info2{};

    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }

  {  // different id
    const ProcedureEditorInfo info1{"abc", ProcedureEditorType::kNodeEditor};
    const ProcedureEditorInfo info2{"def", ProcedureEditorType::kNodeEditor};
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {  // different tab
    const ProcedureEditorInfo info1{"abc", ProcedureEditorType::kNodeEditor};
    const ProcedureEditorInfo info2{"abc", ProcedureEditorType::kXmlPanel};
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {  // all the same
    const ProcedureEditorInfo info1{"abc", ProcedureEditorType::kNodeEditor};
    const ProcedureEditorInfo info2{"abc", ProcedureEditorType::kNodeEditor};
    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }
}

TEST_F(ComponentTypesTest, StringFromProcedureInfoList)
{
  {  // default
    const std::vector<ProcedureEditorInfo> info;
    const auto str = oac_tree_gui::GetStringFromProcedureInfoList(info);
    EXPECT_EQ(str, "identifiers[] tabs[]");

    // back
    const auto parsed_info = oac_tree_gui::GetProcedureInfoListFromString(str);
    EXPECT_TRUE(parsed_info.empty());
  }

  {  // non-empty
    const std::vector<ProcedureEditorInfo> info{{"proc1", ProcedureEditorType::kInstructionTree},
                                                {"proc2", ProcedureEditorType::kXmlPanel}};
    const auto str = oac_tree_gui::GetStringFromProcedureInfoList(info);
    EXPECT_EQ(str, "identifiers[proc1;proc2] tabs[0;3]");

    // back
    const auto parsed_info = oac_tree_gui::GetProcedureInfoListFromString(str);
    EXPECT_EQ(parsed_info, info);
  }
}

}  // namespace oac_tree_gui::test
