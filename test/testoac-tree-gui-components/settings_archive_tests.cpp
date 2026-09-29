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

#include "oac_tree_gui/settings/settings_archive.h"

#include <oac_tree_gui/settings/in_memory_settings_storage.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{

enum class Color : std::int8_t
{
  kRed = 0,
  kGreen,
  kBlue
};

struct PlainInfo
{
  bool flag{false};
  mvvm::int32 count{0};
  double value{0.0};
  std::string name;
  QByteArray data;
  Color color{Color::kRed};
};

template <typename Archive>
void Visit(Archive& archive, PlainInfo& info)
{
  archive("flag", info.flag);
  archive("count", info.count);
  archive("value", info.value);
  archive("name", info.name);
  archive("data", info.data);
  archive("color", info.color);
}

struct PanelInfo
{
  std::string procedure_id;
  mvvm::int32 editor_type{0};
};

template <typename Archive>
void Visit(Archive& archive, PanelInfo& info)
{
  archive("procedure_id", info.procedure_id);
  archive("editor_type", info.editor_type);
}

struct IntVectorInfo
{
  std::vector<std::int32_t> values;
};

template <typename Archive>
void Visit(Archive& archive, IntVectorInfo& info)
{
  archive("values", info.values);
}

struct ViewInfo
{
  std::string splitter_state;
  PanelInfo main_panel;
  std::vector<PanelInfo> panels;
  std::vector<std::string> names;
};

template <typename Archive>
void Visit(Archive& archive, ViewInfo& info)
{
  archive("splitter_state", info.splitter_state);
  archive("main_panel", info.main_panel);
  archive("panels", info.panels);
  archive("names", info.names);
}

}  // namespace

/**
 * @brief Tests for WriteArchive, ReadArchive and helper functions.
 */
class SettingsArchiveTest : public ::testing::Test
{
public:
  InMemorySettingsStorage m_storage;
};

TEST_F(SettingsArchiveTest, JoinSettingsKey)
{
  EXPECT_EQ(JoinSettingsKey("", "name"), std::string("name"));
  EXPECT_EQ(JoinSettingsKey("Group", "name"), std::string("Group/name"));
  EXPECT_EQ(JoinSettingsKey("Group/Nested", "name"), std::string("Group/Nested/name"));
}

TEST_F(SettingsArchiveTest, ByteArrayConversion)
{
  const QByteArray data("\x00\x01\xff", 3);
  const auto settings_value = ToSettingsValue(data);
  EXPECT_EQ(settings_value, mvvm::variant_t{data.toBase64().toStdString()});

  QByteArray result;
  EXPECT_TRUE(FromSettingsValue(settings_value, result));
  EXPECT_EQ(result, data);
}

TEST_F(SettingsArchiveTest, WritePlainFields)
{
  PlainInfo info{true, 42, 1.5, "abc", QByteArray("xyz"), Color::kBlue};

  WriteToStorage(m_storage, "Group", info);

  const std::vector<std::string> expected_keys(
      {"Group/color", "Group/count", "Group/data", "Group/flag", "Group/name", "Group/value"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/flag"), mvvm::variant_t{true});
  EXPECT_EQ(m_storage.Read("Group/count"), mvvm::variant_t{mvvm::int32{42}});
  EXPECT_EQ(m_storage.Read("Group/value"), mvvm::variant_t{1.5});
  EXPECT_EQ(m_storage.Read("Group/name"), mvvm::variant_t{std::string("abc")});
  EXPECT_EQ(m_storage.Read("Group/data"), mvvm::variant_t{std::string("eHl6")});
  EXPECT_EQ(m_storage.Read("Group/color"), mvvm::variant_t{mvvm::int32{2}});
}

TEST_F(SettingsArchiveTest, PlainFieldsRoundTrip)
{
  const PlainInfo info{true, 42, 1.5, "abc", QByteArray("xyz"), Color::kGreen};

  WriteToStorage(m_storage, "Group", info);
  const auto result = ReadFromStorage<PlainInfo>(m_storage, "Group");

  EXPECT_EQ(result.flag, info.flag);
  EXPECT_EQ(result.count, info.count);
  EXPECT_EQ(result.value, info.value);
  EXPECT_EQ(result.name, info.name);
  EXPECT_EQ(result.data, info.data);
  EXPECT_EQ(result.color, info.color);
}

TEST_F(SettingsArchiveTest, MissingKeysKeepInitialValue)
{
  m_storage.Write("Group/count", mvvm::variant_t{mvvm::int32{42}});

  const PlainInfo initial{true, 1, 2.0, "abc", QByteArray("xyz"), Color::kBlue};
  const auto result = ReadFromStorage(m_storage, "Group", initial);

  EXPECT_EQ(result.flag, initial.flag);
  EXPECT_EQ(result.count, 42);
  EXPECT_EQ(result.value, initial.value);
  EXPECT_EQ(result.name, initial.name);
  EXPECT_EQ(result.data, initial.data);
  EXPECT_EQ(result.color, initial.color);
}

TEST_F(SettingsArchiveTest, WrongTypeKeepsInitialValue)
{
  m_storage.Write("Group/flag", mvvm::variant_t{mvvm::int32{0}});
  m_storage.Write("Group/count", mvvm::variant_t{1.5});
  m_storage.Write("Group/value", mvvm::variant_t{false});

  const PlainInfo initial{true, 1, 2.0};
  const auto result = ReadFromStorage(m_storage, "Group", initial);

  EXPECT_TRUE(result.flag);
  EXPECT_EQ(result.count, 1);
  EXPECT_EQ(result.value, 2.0);
}

TEST_F(SettingsArchiveTest, StringValuesAreParsed)
{
  m_storage.Write("Group/flag", mvvm::variant_t{std::string("true")});
  m_storage.Write("Group/count", mvvm::variant_t{std::string("42")});
  m_storage.Write("Group/value", mvvm::variant_t{std::string("1.5")});
  m_storage.Write("Group/color", mvvm::variant_t{std::string("2")});

  const auto result = ReadFromStorage<PlainInfo>(m_storage, "Group");

  EXPECT_TRUE(result.flag);
  EXPECT_EQ(result.count, 42);
  EXPECT_EQ(result.value, 1.5);
  EXPECT_EQ(result.color, Color::kBlue);
}

TEST_F(SettingsArchiveTest, InvalidStringValuesKeepInitialValue)
{
  m_storage.Write("Group/flag", mvvm::variant_t{std::string("maybe")});
  m_storage.Write("Group/count", mvvm::variant_t{std::string("abc")});
  m_storage.Write("Group/value", mvvm::variant_t{std::string("")});

  const PlainInfo initial{false, 1, 2.0};
  const auto result = ReadFromStorage(m_storage, "Group", initial);

  EXPECT_FALSE(result.flag);
  EXPECT_EQ(result.count, 1);
  EXPECT_EQ(result.value, 2.0);
}

TEST_F(SettingsArchiveTest, IntVectorRoundTrip)
{
  const IntVectorInfo info{{1, 2, 3}};

  WriteToStorage(m_storage, "Group", info);

  const std::vector<std::string> expected_keys(
      {"Group/values/0", "Group/values/1", "Group/values/2", "Group/values/size"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/values/size"), mvvm::variant_t{mvvm::int32{3}});
  EXPECT_EQ(m_storage.Read("Group/values/0"), mvvm::variant_t{mvvm::int32{1}});
  EXPECT_EQ(m_storage.Read("Group/values/1"), mvvm::variant_t{mvvm::int32{2}});
  EXPECT_EQ(m_storage.Read("Group/values/2"), mvvm::variant_t{mvvm::int32{3}});

  const auto result = ReadFromStorage<IntVectorInfo>(m_storage, "Group");
  EXPECT_EQ(result.values, info.values);
}

TEST_F(SettingsArchiveTest, WriteNestedStructAndVectors)
{
  ViewInfo info;
  info.splitter_state = "state";
  info.main_panel = {"proc0", 1};
  info.panels = {{"proc1", 2}, {"proc2", 3}};
  info.names = {"a", "b"};

  WriteToStorage(m_storage, "View", info);

  const std::vector<std::string> expected_keys(
      {"View/main_panel/editor_type", "View/main_panel/procedure_id", "View/names/0",
       "View/names/1", "View/names/size", "View/panels/0/editor_type", "View/panels/0/procedure_id",
       "View/panels/1/editor_type", "View/panels/1/procedure_id", "View/panels/size",
       "View/splitter_state"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("View/panels/size"), mvvm::variant_t{mvvm::int32{2}});
  EXPECT_EQ(m_storage.Read("View/panels/1/procedure_id"), mvvm::variant_t{std::string("proc2")});
}

TEST_F(SettingsArchiveTest, NestedStructAndVectorsRoundTrip)
{
  ViewInfo info;
  info.splitter_state = "state";
  info.main_panel = {"proc0", 1};
  info.panels = {{"proc1", 2}, {"proc2", 3}};
  info.names = {"a", "b"};

  WriteToStorage(m_storage, "View", info);
  const auto result = ReadFromStorage<ViewInfo>(m_storage, "View");

  EXPECT_EQ(result.splitter_state, info.splitter_state);
  EXPECT_EQ(result.main_panel.procedure_id, info.main_panel.procedure_id);
  EXPECT_EQ(result.main_panel.editor_type, info.main_panel.editor_type);
  ASSERT_EQ(result.panels.size(), 2);
  EXPECT_EQ(result.panels.at(0).procedure_id, std::string("proc1"));
  EXPECT_EQ(result.panels.at(0).editor_type, 2);
  EXPECT_EQ(result.panels.at(1).procedure_id, std::string("proc2"));
  EXPECT_EQ(result.panels.at(1).editor_type, 3);
  EXPECT_EQ(result.names, info.names);
}

TEST_F(SettingsArchiveTest, EmptyVectorRoundTrip)
{
  ViewInfo initial;
  initial.names = {"a"};

  WriteToStorage(m_storage, "View", ViewInfo{});
  const auto result = ReadFromStorage(m_storage, "View", initial);

  EXPECT_EQ(m_storage.Read("View/names/size"), mvvm::variant_t{mvvm::int32{0}});
  EXPECT_TRUE(result.names.empty());
}

TEST_F(SettingsArchiveTest, ShrinkingVectorIgnoresStaleKeys)
{
  ViewInfo info;
  info.names = {"a", "b", "c"};
  WriteToStorage(m_storage, "View", info);

  info.names = {"d"};
  WriteToStorage(m_storage, "View", info);

  // stale elements stay in the storage, but are not read
  EXPECT_TRUE(m_storage.Read("View/names/1").has_value());
  EXPECT_TRUE(m_storage.Read("View/names/2").has_value());
  EXPECT_EQ(ReadFromStorage<ViewInfo>(m_storage, "View").names, info.names);
}

TEST_F(SettingsArchiveTest, MissingVectorSizeKeepsInitialValue)
{
  m_storage.Write("View/names/0", mvvm::variant_t{std::string("x")});

  ViewInfo initial;
  initial.names = {"a", "b"};
  const auto result = ReadFromStorage(m_storage, "View", initial);

  EXPECT_EQ(result.names, initial.names);
}

TEST_F(SettingsArchiveTest, InvalidVectorSizeKeepsInitialValue)
{
  m_storage.Write("View/names/size", mvvm::variant_t{mvvm::int32{-1}});

  ViewInfo initial;
  initial.names = {"a", "b"};
  const auto result = ReadFromStorage(m_storage, "View", initial);

  EXPECT_EQ(result.names, initial.names);
}

TEST_F(SettingsArchiveTest, MissingVectorElementIsDefault)
{
  m_storage.Write("View/panels/size", mvvm::variant_t{mvvm::int32{2}});
  m_storage.Write("View/panels/1/procedure_id", mvvm::variant_t{std::string("proc1")});

  const auto result = ReadFromStorage<ViewInfo>(m_storage, "View");

  ASSERT_EQ(result.panels.size(), 2);
  EXPECT_TRUE(result.panels.at(0).procedure_id.empty());
  EXPECT_EQ(result.panels.at(1).procedure_id, std::string("proc1"));
}

TEST_F(SettingsArchiveTest, WriteKeepsOtherKeysOfGroup)
{
  m_storage.Write("Group/other", mvvm::variant_t{mvvm::int32{1}});

  WriteToStorage(m_storage, "Group", PlainInfo{});

  EXPECT_EQ(m_storage.Read("Group/other"), mvvm::variant_t{mvvm::int32{1}});
}

}  // namespace oac_tree_gui::test
