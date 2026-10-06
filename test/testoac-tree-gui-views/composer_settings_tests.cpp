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

#include <oac_tree_gui/views/composer/composer_tools_panel.h>
#include <oac_tree_gui/views/composer/instruction_editor_widget.h>
#include <oac_tree_gui/views/composer/workspace_variable_tree_view.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for composer widgets storing settings in the settings manager.
 */
class ComposerSettingsTest : public ::testing::Test
{
public:
  //! Returns true if the application storage has the key.
  bool HasKey(const std::string& key)
  {
    return m_settings_manager.GetStorage(sup::gui::SettingsScope::kApplication)
        ->Read(key)
        .has_value();
  }

  sup::gui::NullCommandService m_command_service;
  TestSettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
};

TEST_F(ComposerSettingsTest, ComposerToolsPanel)
{
  const ComposerToolsPanel panel(m_context);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  EXPECT_TRUE(HasKey("ComposerToolsPanel/collapsible_list/state"));
}

TEST_F(ComposerSettingsTest, InstructionEditorWidget)
{
  const InstructionEditorWidget widget(m_context);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  EXPECT_TRUE(HasKey("InstructionEditorWidget/splitter/state"));
  EXPECT_TRUE(HasKey("InstructionEditorWidget/tree_header/state"));
  EXPECT_TRUE(HasKey("InstructionAttributeEditor/tree_header/state"));
}

TEST_F(ComposerSettingsTest, WorkspaceTreeAndTableHaveDifferentGroups)
{
  const WorkspaceVariableTreeView tree(WorkspacePresentationType::kWorkspaceTree,
                                       m_settings_manager);
  const WorkspaceVariableTreeView table(WorkspacePresentationType::kWorkspaceTable,
                                        m_settings_manager);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  EXPECT_TRUE(HasKey("WorkspaceVariableTreeView/tree_header/state"));
  EXPECT_TRUE(HasKey("WorkspaceVariableTreeView/table_header/state"));
}

}  // namespace oac_tree_gui::test
