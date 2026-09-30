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

#include "oac_tree_gui/settings/project_settings_storage.h"

#include <oac_tree_gui/model/component_types.h>
#include <oac_tree_gui/model/project_model.h>
#include <oac_tree_gui/model/settings_container_item.h>
#include <oac_tree_gui/settings/settings_archive.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace oac_tree_gui::test
{

namespace
{

constexpr auto kValueKey = "Group/name";

}  // namespace

/**
 * @brief Tests for ProjectSettingsStorage class.
 */
class ProjectSettingsStorageTest : public test::FolderTest
{
public:
  ProjectSettingsStorageTest() : FolderTest("ProjectSettingsStorageTest") {}

  //! Returns storage working with the model of the fixture.
  ProjectSettingsStorage CreateStorage()
  {
    return ProjectSettingsStorage([this]() { return m_model; });
  }

  //! Returns the number of settings containers in the model.
  static std::size_t GetContainerCount(ProjectModel& model)
  {
    return mvvm::utils::GetTopItems<SettingsContainerItem>(model).size();
  }

  ProjectModel* m_model{nullptr};
};

TEST_F(ProjectSettingsStorageTest, NoModel)
{
  auto storage = CreateStorage();

  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  EXPECT_NO_THROW(storage.Write(kValueKey, std::string("abc")));
}

TEST_F(ProjectSettingsStorageTest, ReadDoesNotCreateContainer)
{
  ProjectModel model;
  m_model = &model;
  auto storage = CreateStorage();

  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  EXPECT_EQ(GetContainerCount(model), 0);
}

TEST_F(ProjectSettingsStorageTest, WriteAndRead)
{
  ProjectModel model;
  m_model = &model;
  auto storage = CreateStorage();

  storage.Write(kValueKey, std::string("abc"));
  storage.Write("Group/size", mvvm::int32{2});

  EXPECT_EQ(GetContainerCount(model), 1);
  EXPECT_EQ(storage.Read(kValueKey), mvvm::variant_t{std::string("abc")});
  EXPECT_EQ(storage.Read("Group/size"), mvvm::variant_t{mvvm::int32{2}});
  EXPECT_FALSE(storage.Read("Group/other").has_value());
}

TEST_F(ProjectSettingsStorageTest, FollowsCurrentModel)
{
  ProjectModel model0;
  ProjectModel model1;
  auto storage = CreateStorage();

  m_model = &model0;
  storage.Write(kValueKey, std::string("abc"));

  m_model = &model1;
  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  storage.Write(kValueKey, std::string("def"));

  m_model = &model0;
  EXPECT_EQ(storage.Read(kValueKey), mvvm::variant_t{std::string("abc")});

  m_model = nullptr;
  EXPECT_FALSE(storage.Read(kValueKey).has_value());
}

TEST_F(ProjectSettingsStorageTest, XmlDocumentSaveLoad)
{
  const auto file_path = GetFilePath("XmlDocumentSaveLoad.xml");

  ProjectModel model;
  m_model = &model;
  auto storage = CreateStorage();

  const ComposerViewInfo info{
      "state",
      {{"proc0", ProcedureEditorType::kWorkspace}, {"proc1", ProcedureEditorType::kXmlPanel}}};
  WriteToStorage(storage, kValueKey, info);

  mvvm::XmlDocument document({&model});
  document.Save(file_path);
  model.Clear();
  ASSERT_EQ(GetContainerCount(model), 0);

  document.Load(file_path);

  EXPECT_EQ(ReadFromStorage<ComposerViewInfo>(storage, kValueKey), info);
}

}  // namespace oac_tree_gui::test
