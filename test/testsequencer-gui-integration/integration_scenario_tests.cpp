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

#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/model/xml_utils.h>
#include <sequencergui/monitor/message_panel.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_based_test.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/test_utils.h>

#include <QTest>

using namespace sequencergui;

#include <QSignalSpy>

//! Tests for complex integration scenario.

class IntegrationScenarioTest : public testutils::FolderBasedTest
{
public:
  IntegrationScenarioTest() : FolderBasedTest("test_IntegrationScenario")
  {
    m_job_item = m_models.GetJobModel()->InsertItem<JobItem>();
  }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  ApplicationModels m_models;
  JobItem* m_job_item{nullptr};
};

//! Create procedure, save as a project file, load from disk, and execute procedure in a job
//! manager.

TEST_F(IntegrationScenarioTest, SaveToDiskLoadAndRun)
{
  ApplicationModels m_models;

  auto procedure_item = testutils::CreateCopyProcedureItem(GetSequencerModel());

  const auto file_name = GetFilePath("XmlDocumentSaveLoad.xml");

  mvvm::XmlDocument document({GetSequencerModel()});

  // saving model in file
  document.Save(file_name);

  // loading from file
  document.Load(file_name);

  auto loaded_procedure = GetSequencerModel()->GetProcedures().at(0);
  EXPECT_NE(procedure_item, loaded_procedure);

  MessagePanel panel;
  m_job_item->SetProcedure(loaded_procedure);

  JobManager manager;
  manager.SetMessagePanel(&panel);

  EXPECT_NO_THROW(manager.SubmitJob(m_job_item));

  manager.SetCurrentJob(m_job_item);
  EXPECT_EQ(manager.GetCurrentJob(), m_job_item);
  manager.OnChangeDelayRequest(10);

  QSignalSpy spy_instruction_status(&manager, &JobManager::InstructionStatusChanged);

  auto job_handler = manager.GetCurrentJobHandler();
  ASSERT_TRUE(job_handler != nullptr);

  EXPECT_FALSE(job_handler->IsRunning());

  auto procedure = job_handler->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariableCount(), 2);

  // starting procedure
  manager.OnStartJobRequest();

  // We are testing here queued signals, need special waiting to let procedure complete
  EXPECT_TRUE(QTest::qWaitFor(
      [&spy_instruction_status]() { return spy_instruction_status.count() == 2; }, 100));

  EXPECT_FALSE(job_handler->IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  auto new_anyvalue_item0 = vars_inside.at(0)->GetAnyValueItem();
  auto new_anyvalue_item1 = vars_inside.at(1)->GetAnyValueItem();

  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};

  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*new_anyvalue_item1), anyvalue0);

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  auto initial_anyvalue_item0 = inside.at(0)->GetAnyValueItem();
  auto initial_anyvalue_item1 = inside.at(1)->GetAnyValueItem();
  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item0), anyvalue0);
  EXPECT_EQ(sup::gui::CreateAnyValue(*initial_anyvalue_item1), anyvalue1);
}

//! Validating that external includes are correctly found.

TEST_F(IntegrationScenarioTest, ExternalInclude)
{
  const std::string main_procedure{R"(
    <Sequence isRoot="True">
        <Include name="External Wait" path="Just Wait" file="external.xml" />
    </Sequence>
)"};

  const std::string external_procedure{R"(
  <Wait name = "Just Wait" timeout="0" />
)"};

  const auto main_file_name = GetFilePath("main.xml");
  testutils::CreateTextFile(main_file_name, testutils::CreateProcedureString(main_procedure));

  const auto external_file_name = GetFilePath("external.xml");
  testutils::CreateTextFile(external_file_name, testutils::CreateProcedureString(external_procedure));

  auto procedure_item = sequencergui::ImportFromFile(main_file_name);
  auto procedure_item_ptr = procedure_item.get();

  auto model = GetSequencerModel();
  model->InsertItem(std::move(procedure_item), model->GetProcedureContainer(),
                    mvvm::TagIndex::Append());

  MessagePanel panel;
  m_job_item->SetProcedure(procedure_item_ptr);

  JobManager manager;
  manager.SetMessagePanel(&panel);

  EXPECT_NO_THROW(manager.SubmitJob(m_job_item));
}
