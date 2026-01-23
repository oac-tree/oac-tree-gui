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

#include "oac_tree_gui/model/project_model.h"

#include <oac_tree_gui/model/composer_settings_item.h>

#include <mvvm/model/model_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class ProjectModelTest : public ::testing::Test
{
};

TEST_F(ProjectModelTest, InitialState)
{
  ProjectModel model;
  EXPECT_EQ(model.GetType(), "ProjectModel");
  EXPECT_NE(mvvm::utils::GetTopItem<ComposerSettingsItem>(model), nullptr);

  // item recreated after cleanup
  model.Clear();
  EXPECT_NE(mvvm::utils::GetTopItem<ComposerSettingsItem>(model), nullptr);
}

}  // namespace oac_tree_gui::test
