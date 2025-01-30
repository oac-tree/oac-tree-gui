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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac-tree-gui/model/job_model.h>
#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/sequencer_model.h>

#include <mvvm/model/item_pool.h>
#include <mvvm/test/mock_project_context.h>
#include <mvvm/utils/file_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

#include "oac-tree-gui/model/application_models.h"

namespace oac_tree_gui::test
{

/**
 * @brief Tests of ApplicationModels class.
 */
class ApplicationModelsTest : public test::FolderTest
{
public:
  ApplicationModelsTest() : FolderTest("ApplicationModelsTest") {}

  std::unique_ptr<ApplicationModels> CreateProject()
  {
    return std::make_unique<ApplicationModels>(
        m_mock_project_context.CreateContext(ApplicationModels::kApplicationType));
  }

  mvvm::test::MockProjectContext m_mock_project_context;
};

TEST_F(ApplicationModelsTest, InitialState)
{
  ApplicationModels models;

  EXPECT_EQ(models.GetSequencerModel(), nullptr);
  EXPECT_EQ(models.GetJobModel(), nullptr);
  EXPECT_TRUE(models.GetPath().empty());
  EXPECT_EQ(models.GetProjectType(), mvvm::ProjectType::kFileBased);
  EXPECT_EQ(models.GetApplicationType(), std::string());

  models.CreateEmpty();

  EXPECT_NE(models.GetSequencerModel(), nullptr);
  EXPECT_NE(models.GetJobModel(), nullptr);

  const std::vector<mvvm::ISessionModel*> expected(
      {models.GetSequencerModel(), models.GetJobModel()});
  EXPECT_EQ(models.GetModels(), expected);
}

TEST_F(ApplicationModelsTest, FindItems)
{
  ApplicationModels models;
  models.CreateEmpty();

  // default catalogue is capable of creating sequencer items
  auto procedure = models.GetSequencerModel()->InsertItem<ProcedureItem>();
  EXPECT_EQ(procedure->GetType(), ProcedureItem::GetStaticType());

  // second model can find alien item
  EXPECT_EQ(models.GetJobModel()->FindItem(procedure->GetIdentifier()), procedure);
}

//! Test focuses on item pool recreation after creation of the new project.
TEST_F(ApplicationModelsTest, RecreateModels)
{
  ApplicationModels models;
  models.CreateEmpty();

  // by default we have already untitled procedure created
  ASSERT_FALSE(models.GetSequencerModel()->GetProcedures().empty());
  EXPECT_FALSE(models.IsModified());

  // second model can find item belonging to another model
  auto procedure = models.GetSequencerModel()->GetProcedures().at(0);
  EXPECT_EQ(models.GetJobModel()->FindItem(procedure->GetIdentifier()), procedure);

  // on project close models should be removed, item pool cleared
  models.Close();
  EXPECT_EQ(models.GetItemPool()->GetSize(), 0);
  EXPECT_EQ(models.GetSequencerModel(), nullptr);
  EXPECT_EQ(models.GetJobModel(), nullptr);

  // on new project creation item pool should receive new items
  models.CreateEmpty();
  // two root items from two models, and one procedure container, and one untitled procedure with
  // all its properties
  EXPECT_TRUE(models.GetItemPool()->GetSize() > 3);
}

TEST_F(ApplicationModelsTest, CreateNewProjectThenModifyThenSaveThenClose)
{
  const auto expected_path = GetFilePath("untitled.xml");

  auto project = CreateProject();

  // setting up expectations before project creation
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  EXPECT_TRUE(project->CreateEmpty());

  EXPECT_FALSE(project->HasPath());
  EXPECT_NE(project->GetSequencerModel(), nullptr);
  EXPECT_NE(project->GetJobModel(), nullptr);
  EXPECT_FALSE(project->IsModified());

  // setting up expectation before project modification
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);

  // modifying a project
  project->GetSequencerModel()->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);
  EXPECT_TRUE(project->Save(expected_path));

  EXPECT_EQ(project->GetPath(), expected_path);
  EXPECT_FALSE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->Close());
  EXPECT_EQ(project->GetSequencerModel(), nullptr);
  EXPECT_EQ(project->GetJobModel(), nullptr);
  EXPECT_FALSE(project->IsModified());

  EXPECT_TRUE(mvvm::utils::IsExists(expected_path));
}

}  // namespace oac_tree_gui::test
