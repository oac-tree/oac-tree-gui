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

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/jobsystem/job_utils.h>
#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/xml_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>
#include <QTest>

namespace oac_tree_gui
{

//! Tests for all XML procedures found in resource/functional folder.

class ResourceFolderTest : public ::testing::TestWithParam<std::string>
{
public:
  ResourceFolderTest()
  {
    m_models.CreateEmpty();
    m_job_item = m_models.GetJobModel()->InsertItem<LocalJobItem>();
  }

  /**
   * @brief Prevent any test from here running if not all plugins are loaded.
   */
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable() || !IsSequencerControlPluginAvailable())
    {
      GTEST_SKIP();
    }
  }

  ApplicationModels m_models;
  LocalJobItem* m_job_item{nullptr};
};

TEST_P(ResourceFolderTest, RunProcedure)
{
  // load procedure from XML
  auto filename = test::ProjectResourceDir() + "/" + GetParam();
  EXPECT_TRUE(mvvm::utils::IsExists(filename));
  auto procedure_item = oac_tree_gui::ImportFromFile(filename);
  EXPECT_TRUE(procedure_item);

  // insert procedure in the model
  auto procedure_ptr = procedure_item.get();
  auto model = m_models.GetSequencerModel();
  model->InsertItem(std::move(procedure_item), model->GetRootItem(), mvvm::TagIndex::Append());

  // setup JobHandler to run procedure with the help of JobItem
  m_job_item->SetProcedure(procedure_ptr);
  LocalJobHandler job_handler(m_job_item, UserContext{});
  QSignalSpy spy_instruction_status(&job_handler, &LocalJobHandler::InstructionStatusChanged);

  // starting procedure and waiting for completion
  job_handler.Start();

  auto predicate = [this]() { return m_job_item->GetStatus() == RunnerStatus::kSucceeded; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 1000));

  // validating some of parameters after job is complet
  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kSucceeded);
  EXPECT_EQ(job_handler.GetRunnerStatus(), RunnerStatus::kSucceeded);
  auto instructions = mvvm::utils::FindItems<InstructionItem>(*m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

INSTANTIATE_TEST_SUITE_P(FunctionalTests, ResourceFolderTest,
                         ::testing::Values("wait_for_condition.xml", "fallback.xml",
                                           "local_message.xml"));

}  // namespace oac_tree_gui
