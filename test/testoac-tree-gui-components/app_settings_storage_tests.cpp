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

#include "oac_tree_gui/settings/app_settings_storage.h"

#include <oac_tree_gui/settings/settings_archive.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QMap>

namespace oac_tree_gui::test
{

namespace
{

struct PlainInfo
{
  bool flag{false};
  mvvm::int32 count{0};
  double value{0.0};
  std::string name;
  QByteArray data;
  std::vector<std::string> names;
};

template <typename Archive>
void Visit(Archive& archive, PlainInfo& info)
{
  archive("flag", info.flag);
  archive("count", info.count);
  archive("value", info.value);
  archive("name", info.name);
  archive("data", info.data);
  archive("names", info.names);
}

/**
 * @brief The IniLikeFile class emulates QSettings reading from an INI file after restart.
 *
 * Values are stored as strings, so their type is lost.
 */
class IniLikeFile
{
public:
  QVariant Read(const QString& key) const { return m_values.value(key); }

  void Write(const QString& key, const QVariant& value) { m_values[key] = value.toString(); }

  QMap<QString, QVariant> m_values;
};

}  // namespace

/**
 * @brief Tests for AppSettingsStorage class.
 */
class AppSettingsStorageTest : public ::testing::Test
{
public:
  //! Creates storage working with mock functions.
  std::unique_ptr<AppSettingsStorage> CreateMockedStorage()
  {
    return std::make_unique<AppSettingsStorage>(m_mock_read.AsStdFunction(),
                                                m_mock_write.AsStdFunction());
  }

  //! Creates storage working with INI-like file emulation.
  std::unique_ptr<AppSettingsStorage> CreateIniLikeStorage()
  {
    return std::make_unique<AppSettingsStorage>(
        [this](const QString& key) { return m_file.Read(key); },
        [this](const QString& key, const QVariant& value) { m_file.Write(key, value); });
  }

  ::testing::MockFunction<QVariant(const QString&)> m_mock_read;
  ::testing::MockFunction<void(const QString&, const QVariant&)> m_mock_write;
  IniLikeFile m_file;
};

TEST_F(AppSettingsStorageTest, Write)
{
  auto storage = CreateMockedStorage();

  EXPECT_CALL(m_mock_write, Call(QString("Group/flag"), QVariant(true))).Times(1);
  EXPECT_CALL(m_mock_write, Call(QString("Group/count"), QVariant(42))).Times(1);
  EXPECT_CALL(m_mock_write, Call(QString("Group/value"), QVariant(1.5))).Times(1);
  EXPECT_CALL(m_mock_write, Call(QString("Group/name"), QVariant(QString("abc")))).Times(1);

  storage->Write("Group/flag", mvvm::variant_t{true});
  storage->Write("Group/count", mvvm::variant_t{mvvm::int32{42}});
  storage->Write("Group/value", mvvm::variant_t{1.5});
  storage->Write("Group/name", mvvm::variant_t{std::string("abc")});
}

TEST_F(AppSettingsStorageTest, Read)
{
  auto storage = CreateMockedStorage();

  EXPECT_CALL(m_mock_read, Call(QString("Group/count"))).WillOnce(::testing::Return(QVariant(42)));
  EXPECT_CALL(m_mock_read, Call(QString("Group/name")))
      .WillOnce(::testing::Return(QVariant(QString("abc"))));

  EXPECT_EQ(storage->Read("Group/count"), mvvm::variant_t{mvvm::int32{42}});
  EXPECT_EQ(storage->Read("Group/name"), mvvm::variant_t{std::string("abc")});
}

TEST_F(AppSettingsStorageTest, ReadAbsentKey)
{
  auto storage = CreateMockedStorage();

  EXPECT_CALL(m_mock_read, Call(QString("Group/count"))).WillOnce(::testing::Return(QVariant()));

  EXPECT_FALSE(storage->Read("Group/count").has_value());
}

TEST_F(AppSettingsStorageTest, IniLikeFileLosesType)
{
  auto storage = CreateIniLikeStorage();

  storage->Write("Group/count", mvvm::variant_t{mvvm::int32{42}});

  EXPECT_EQ(storage->Read("Group/count"), mvvm::variant_t{std::string("42")});
}

TEST_F(AppSettingsStorageTest, StructRoundTripThroughIniLikeFile)
{
  auto storage = CreateIniLikeStorage();
  const PlainInfo info{true, 42, 1.5, "abc", QByteArray("\x00\x01\xff", 3), {"a", "b"}};

  WriteToStorage(*storage, "Group", info);
  const auto result = ReadFromStorage<PlainInfo>(*storage, "Group");

  EXPECT_EQ(result.flag, info.flag);
  EXPECT_EQ(result.count, info.count);
  EXPECT_EQ(result.value, info.value);
  EXPECT_EQ(result.name, info.name);
  EXPECT_EQ(result.data, info.data);
  EXPECT_EQ(result.names, info.names);
}

TEST_F(AppSettingsStorageTest, ShrinkingVectorThroughIniLikeFile)
{
  auto storage = CreateIniLikeStorage();
  PlainInfo info;
  info.names = {"a", "b", "c"};
  WriteToStorage(*storage, "Group", info);

  info.names = {"d"};
  WriteToStorage(*storage, "Group", info);

  // stale elements stay in the file, but are not read
  EXPECT_TRUE(m_file.m_values.contains("Group/names/1"));
  EXPECT_EQ(ReadFromStorage<PlainInfo>(*storage, "Group").names, info.names);
}

}  // namespace oac_tree_gui::test
