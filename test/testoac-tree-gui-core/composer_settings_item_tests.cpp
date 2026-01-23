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

#include "oac_tree_gui/model/composer_settings_item.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for ComposerSettingsItem class.
 */
class ComposerSettingsItemTest : public ::testing::Test
{
};

TEST_F(ComposerSettingsItemTest, ProcedureEditorInfoListWhenEmpty)
{
  ComposerSettingsItem item;

  EXPECT_TRUE(item.GetProcedureEditorInfoList().empty());

  item.SetProcedureEditorInfoList({});
  EXPECT_TRUE(item.GetProcedureEditorInfoList().empty());
}

TEST_F(ComposerSettingsItemTest, ProcedureEditorInfoListNonEmpty)
{
  ComposerSettingsItem item;

  const std::vector<ProcedureEditorInfo> input_info{
      {"proc1", ProcedureEditorType::kInstructionTree}, {"proc2", ProcedureEditorType::kXmlPanel}};
  item.SetProcedureEditorInfoList(input_info);
  const auto output_info = item.GetProcedureEditorInfoList();

  EXPECT_TRUE(input_info == output_info);
}

}  // namespace oac_tree_gui::test
