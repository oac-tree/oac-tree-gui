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

#include "oac_tree_gui/views/explorer/sequencer_explorer_view.h"

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for SequencerExplorerView class.
 */
class SequencerExplorerViewTest : public ::testing::Test
{
public:
  sup::gui::NullCommandService m_command_service;
  TestSettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
};

TEST_F(SequencerExplorerViewTest, CollectSettings)
{
  const SequencerExplorerView view(m_context);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  auto storage = m_settings_manager.GetStorage(sup::gui::SettingsScope::kApplication);
  EXPECT_TRUE(storage->Read("SequencerExplorerView/splitter/state").has_value());
  EXPECT_TRUE(storage->Read("ExplorerPanel/collapsible_list/state").has_value());
}

}  // namespace oac_tree_gui::test
