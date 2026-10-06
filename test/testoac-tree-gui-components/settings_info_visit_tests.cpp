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

#include <oac_tree_gui/model/component_types.h>
#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/undo_settings_item.h>
#include <oac_tree_gui/settings/in_memory_settings_storage.h>
#include <oac_tree_gui/settings/settings_archive.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{

constexpr auto kGroup = "Group";

}  // namespace

/**
 * @brief Tests for Visit functions of settings structs.
 */
class SettingsInfoVisitTest : public ::testing::Test
{
public:
  InMemorySettingsStorage m_storage;
};

TEST_F(SettingsInfoVisitTest, ComposerViewInfoReadFromEmptyStorage)
{
  const auto info = ReadFromStorage<ComposerViewInfo>(m_storage, kGroup);

  EXPECT_EQ(info, ComposerViewInfo{});
}

TEST_F(SettingsInfoVisitTest, ComposerViewInfoWriteKeys)
{
  const ComposerViewInfo info{"state", {{"proc0", ProcedureEditorType::kWorkspace}}};

  WriteToStorage(m_storage, kGroup, info);

  const std::vector<std::string> expected_keys(
      {"Group/editor_info_list/0/editor_type", "Group/editor_info_list/0/procedure_id",
       "Group/editor_info_list/size", "Group/splitter_state"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/editor_info_list/0/editor_type"),
            mvvm::variant_t{mvvm::int32{1}});
}

TEST_F(SettingsInfoVisitTest, ComposerViewInfoRoundTrip)
{
  const ComposerViewInfo info{"state",
                              {{"proc0", ProcedureEditorType::kInstructionTree},
                               {"proc1", ProcedureEditorType::kWorkspace},
                               {"", ProcedureEditorType::kXmlPanel}}};

  WriteToStorage(m_storage, kGroup, info);

  EXPECT_EQ(ReadFromStorage<ComposerViewInfo>(m_storage, kGroup), info);
}

TEST_F(SettingsInfoVisitTest, ComposerViewInfoMissingEditorType)
{
  m_storage.Write("Group/editor_info_list/size", mvvm::int32{1});
  m_storage.Write("Group/editor_info_list/0/procedure_id", std::string("proc0"));

  const auto info = ReadFromStorage<ComposerViewInfo>(m_storage, kGroup);

  ASSERT_EQ(info.editor_info_list.size(), 1);
  EXPECT_EQ(info.editor_info_list.at(0).procedure_id, "proc0");
  EXPECT_EQ(info.editor_info_list.at(0).editor_type, ProcedureEditorType::kInstructionTree);
}

TEST_F(SettingsInfoVisitTest, UndoInfoReadFromEmptyStorage)
{
  const auto info = ReadFromStorage<UndoInfo>(m_storage, kGroup);

  EXPECT_EQ(info.is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(info.undo_limit, constants::kUndoLimitDefault);
}

TEST_F(SettingsInfoVisitTest, UndoInfoWriteKeys)
{
  WriteToStorage(m_storage, kGroup, UndoInfo{false, 42});

  const std::vector<std::string> expected_keys({"Group/is_enabled", "Group/undo_limit"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/is_enabled"), mvvm::variant_t{false});
  EXPECT_EQ(m_storage.Read("Group/undo_limit"), mvvm::variant_t{mvvm::int32{42}});
}

TEST_F(SettingsInfoVisitTest, UndoInfoKeysInSettingsGroup)
{
  WriteToStorage(m_storage, kUndoSettingsGroup, UndoInfo{});

  const std::vector<std::string> expected_keys(
      {"Settings/Undo/is_enabled", "Settings/Undo/undo_limit"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
}

TEST_F(SettingsInfoVisitTest, UndoInfoRoundTrip)
{
  WriteToStorage(m_storage, kGroup, UndoInfo{false, 42});

  const auto info = ReadFromStorage<UndoInfo>(m_storage, kGroup);

  EXPECT_FALSE(info.is_enabled);
  EXPECT_EQ(info.undo_limit, 42);
}

TEST_F(SettingsInfoVisitTest, PluginSettingsInfoReadFromEmptyStorage)
{
  const auto initial = CreateDefaultPluginSettingsInfo();

  EXPECT_EQ(ReadFromStorage<PluginSettingsInfo>(m_storage, kGroup), PluginSettingsInfo{});
  EXPECT_EQ(ReadFromStorage<PluginSettingsInfo>(m_storage, kGroup, initial), initial);
}

TEST_F(SettingsInfoVisitTest, PluginSettingsInfoWriteKeys)
{
  WriteToStorage(m_storage, kGroup, PluginSettingsInfo{true, {"dir"}, false, {"abc", "def"}});

  const std::vector<std::string> expected_keys(
      {"Group/dir_list/0", "Group/dir_list/size", "Group/plugin_list/0", "Group/plugin_list/1",
       "Group/plugin_list/size", "Group/use_dir_list", "Group/use_plugin_list"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/plugin_list/1"), mvvm::variant_t{std::string("def")});
}

TEST_F(SettingsInfoVisitTest, PluginSettingsInfoKeysInSettingsGroup)
{
  WriteToStorage(m_storage, kPluginSettingsGroup, PluginSettingsInfo{});

  const std::vector<std::string> expected_keys(
      {"Settings/Plugins/dir_list/size", "Settings/Plugins/plugin_list/size",
       "Settings/Plugins/use_dir_list", "Settings/Plugins/use_plugin_list"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
}

TEST_F(SettingsInfoVisitTest, PluginSettingsInfoRoundTrip)
{
  const PluginSettingsInfo info{true, {"dir1", "dir2"}, false, {"abc"}};

  WriteToStorage(m_storage, kGroup, info);

  EXPECT_EQ(ReadFromStorage<PluginSettingsInfo>(m_storage, kGroup), info);
}

TEST_F(SettingsInfoVisitTest, PluginSettingsInfoEmptyListOverridesInitial)
{
  WriteToStorage(m_storage, kGroup, PluginSettingsInfo{});

  const auto info =
      ReadFromStorage<PluginSettingsInfo>(m_storage, kGroup, CreateDefaultPluginSettingsInfo());

  EXPECT_EQ(info, PluginSettingsInfo{});
}

}  // namespace oac_tree_gui::test
