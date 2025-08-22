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

#include "oac_tree_gui/model/sequencer_settings_helper.h"

#include <sup/gui/core/environment.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper functions in sequencere_settings_helper.h
 */
class SequencerSettingsHelperTest : public ::testing::Test
{
};

TEST_F(SequencerSettingsHelperTest, GetDefaultPluginDirList)
{
  if (sup::gui::IsOnCodac())
  {
    EXPECT_EQ(GetDefaultPluginDirList(),
              std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"}));
  }
  else
  {
    EXPECT_TRUE(GetDefaultPluginDirList().empty());
  }
}

}  // namespace oac_tree_gui::test
