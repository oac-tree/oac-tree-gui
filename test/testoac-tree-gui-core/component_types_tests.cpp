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

TEST_F(ComponentTypesTest, ProcedureEditorInfoStringConversion)
{
  {  // default constructed
    const ProcedureEditorInfo input_info{};
    const auto str = GetStringFromProcedureInfo(input_info);
    EXPECT_EQ(str, "\"\" 0");
    const auto output_info = GetProcedureInfoFromString(str);
    EXPECT_TRUE(input_info == output_info);
  }

  {  // non-default
    const ProcedureEditorInfo input_info{"my_procedure", ProcedureEditorType::kWorkspace};
    const auto str = GetStringFromProcedureInfo(input_info);
    EXPECT_EQ(str, "\"my_procedure\" 1");
    const auto output_info = GetProcedureInfoFromString(str);
    EXPECT_TRUE(input_info == output_info);
  }

  {  // from empty string
    const auto output_info = GetProcedureInfoFromString("");
    const ProcedureEditorInfo expected_info{};
    EXPECT_TRUE(expected_info == output_info);
  }
}

TEST_F(ComponentTypesTest, ComposerViewInfoOperators)
{
  {  // default constructed
    const ComposerViewInfo info1{};
    const ComposerViewInfo info2{};

    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }

  {  // different splitter state
    const ComposerViewInfo info1{"state1", {}};
    const ComposerViewInfo info2{"state2", {}};

    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {  // different editor info list
    const ComposerViewInfo info1{"state", {{"proc1", ProcedureEditorType::kXmlPanel}}};
    const ComposerViewInfo info2{"state", {{"proc2", ProcedureEditorType::kXmlPanel}}};

    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {  // all the same
    const ComposerViewInfo info1{"state", {{"proc1", ProcedureEditorType::kXmlPanel}}};
    const ComposerViewInfo info2{"state", {{"proc1", ProcedureEditorType::kXmlPanel}}};

    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }
}

}  // namespace oac_tree_gui::test
