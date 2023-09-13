/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/xml_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>
#include <QTest>
#include <iostream>

namespace
{

/**
 * @brief Returns full path to resource folder.
 */
std::string GetResourceFolder()
{
  return testutils::GetProjectSourceDir() + "/test/resources/functional";
}

}  // namespace

using namespace sequencergui;

//! Tests for all XML procedures found in resource/functional folder.

class ResourceFolderTests : public ::testing::TestWithParam<std::string>
{
public:
  ResourceFolderTests() { m_job_item = m_models.GetJobModel()->InsertItem<JobItem>(); }

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
  JobItem* m_job_item{nullptr};
};

TEST_P(ResourceFolderTests, RunProcedure)
{
  // load procedure from XML
  auto filename = GetResourceFolder() + "/" + GetParam();
  EXPECT_TRUE(mvvm::utils::IsExists(filename));
  auto procedure_item = sequencergui::ImportFromFile(filename);
  EXPECT_TRUE(procedure_item);

  // insert procedure in the model
  auto procedure_ptr = procedure_item.get();
  auto model = m_models.GetSequencerModel();
  model->InsertItem(std::move(procedure_item), model->GetRootItem(), mvvm::TagIndex::Append());

  // setup JobHandler to run procedure with the help of JobItem
  m_job_item->SetProcedure(procedure_ptr);
  JobHandler job_handler(m_job_item);
  job_handler.onPrepareJobRequest();
  QSignalSpy spy_instruction_status(&job_handler, &JobHandler::InstructionStatusChanged);

  // starting procedure and waiting for completion
  job_handler.onStartRequest();
  auto predicate = [this]() { return m_job_item->GetStatus() == "Completed"; };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 300));

  // validating some of parameters after job is complet
  EXPECT_EQ(GetRunnerStatus(m_job_item->GetStatus()), RunnerStatus::kCompleted);
  EXPECT_EQ(job_handler.GetRunnerStatus(), RunnerStatus::kCompleted);
  auto instructions = mvvm::utils::FindItems<InstructionItem>(m_models.GetJobModel());
  EXPECT_EQ(instructions.at(0)->GetStatus(), "Success");
}

INSTANTIATE_TEST_SUITE_P(FunctionalTests, ResourceFolderTests,
                         ::testing::Values("wait_for_condition.xml", "fallback.xml"));
