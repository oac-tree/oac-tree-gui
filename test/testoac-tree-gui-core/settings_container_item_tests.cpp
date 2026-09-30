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

#include "oac_tree_gui/model/settings_container_item.h"

#include <oac_tree_gui/model/project_model.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/test/mock_model_listener.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace oac_tree_gui::test
{

namespace
{

constexpr auto kKey = "Group/editor_info_list/0/procedure_id";

}  // namespace

/**
 * @brief Tests for SettingsContainerItem class.
 */
class SettingsContainerItemTest : public test::FolderTest
{
public:
  SettingsContainerItemTest() : FolderTest("SettingsContainerItemTest") {}
};

TEST_F(SettingsContainerItemTest, InitialState)
{
  const SettingsContainerItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<SettingsContainerItem>());
  EXPECT_FALSE(item.GetValue(kKey).has_value());
}

TEST_F(SettingsContainerItemTest, SetAndGetValue)
{
  SettingsContainerItem item;

  item.SetValue(kKey, std::string("proc0"));
  item.SetValue("Group/size", mvvm::int32{2});
  item.SetValue("Group/flag", true);

  EXPECT_EQ(item.GetValue(kKey), mvvm::variant_t{std::string("proc0")});
  EXPECT_EQ(item.GetValue("Group/size"), mvvm::variant_t{mvvm::int32{2}});
  EXPECT_EQ(item.GetValue("Group/flag"), mvvm::variant_t{true});
  EXPECT_FALSE(item.GetValue("Group").has_value());
}

TEST_F(SettingsContainerItemTest, ChangeValue)
{
  SettingsContainerItem item;
  item.SetValue(kKey, std::string("proc0"));
  auto property = &item.GetItem(kKey);

  item.SetValue(kKey, std::string("proc1"));

  EXPECT_EQ(item.GetValue(kKey), mvvm::variant_t{std::string("proc1")});
  EXPECT_EQ(&item.GetItem(kKey), property);
}

TEST_F(SettingsContainerItemTest, ChangeValueType)
{
  SettingsContainerItem item;
  item.SetValue(kKey, mvvm::int32{1});
  auto property = &item.GetItem(kKey);

  item.SetValue(kKey, std::string("proc1"));

  EXPECT_EQ(item.GetValue(kKey), mvvm::variant_t{std::string("proc1")});
  EXPECT_EQ(&item.GetItem(kKey), property);
}

TEST_F(SettingsContainerItemTest, SetUnchangedValueInModel)
{
  ProjectModel model;
  auto item = model.InsertItem<SettingsContainerItem>();
  item->SetValue(kKey, std::string("proc0"));

  mvvm::test::MockModelListener listener(&model);
  EXPECT_CALL(listener, OnItemInserted(::testing::_)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(::testing::_)).Times(0);

  item->SetValue(kKey, std::string("proc0"));
}

TEST_F(SettingsContainerItemTest, SetValueInModel)
{
  ProjectModel model;
  auto item = model.InsertItem<SettingsContainerItem>();
  item->SetValue(kKey, std::string("proc0"));

  mvvm::test::MockModelListener listener(&model);
  EXPECT_CALL(listener, OnDataChanged(::testing::_)).Times(1);

  item->SetValue(kKey, std::string("proc1"));
}

TEST_F(SettingsContainerItemTest, ChangeValueTypeInModel)
{
  ProjectModel model;
  auto item = model.InsertItem<SettingsContainerItem>();
  item->SetValue(kKey, mvvm::int32{1});

  item->SetValue(kKey, std::string("proc1"));

  EXPECT_EQ(item->GetValue(kKey), mvvm::variant_t{std::string("proc1")});
}

TEST_F(SettingsContainerItemTest, XmlDocumentSaveLoad)
{
  const auto file_path = GetFilePath("XmlDocumentSaveLoad.xml");

  ProjectModel model;
  auto item = model.InsertItem<SettingsContainerItem>();
  item->SetValue(kKey, std::string("proc0"));
  item->SetValue("Group/size", mvvm::int32{1});

  mvvm::XmlDocument document({&model});
  document.Save(file_path);
  model.Clear();
  ASSERT_EQ(mvvm::utils::GetTopItem<SettingsContainerItem>(model), nullptr);

  document.Load(file_path);

  auto restored_item = mvvm::utils::GetTopItem<SettingsContainerItem>(model);
  ASSERT_NE(restored_item, nullptr);
  EXPECT_EQ(restored_item->GetValue(kKey), mvvm::variant_t{std::string("proc0")});
  EXPECT_EQ(restored_item->GetValue("Group/size"), mvvm::variant_t{mvvm::int32{1}});
}

TEST_F(SettingsContainerItemTest, Clone)
{
  SettingsContainerItem item;
  item.SetValue(kKey, std::string("proc0"));

  auto clone = item.Clone();

  auto clone_item = dynamic_cast<SettingsContainerItem*>(clone.get());
  ASSERT_NE(clone_item, nullptr);
  EXPECT_EQ(clone_item->GetValue(kKey), mvvm::variant_t{std::string("proc0")});
}

}  // namespace oac_tree_gui::test
