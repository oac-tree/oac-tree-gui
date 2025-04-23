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

#include <oac_tree_gui/jobsystem/job_manager.h>
#include <oac_tree_gui/jobsystem/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/model/xml_utils.h>
#include <oac_tree_gui/operation/operation_action_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>
#include <QTest>

namespace oac_tree_gui::test
{

/**
 * @brief Complex scenario integration tests.
 */
class IntegrationScenarioTest : public test::FolderTest
{
public:
  IntegrationScenarioTest() : FolderTest("IntegrationScenarioTest")
  {
    m_models.CreateEmpty();
    m_models.GetSequencerModel()->GetProcedureContainer()->Clear();  // our untitled procedure
    m_job_item = m_models.GetJobModel()->InsertItem<LocalJobItem>();
  }

  /**
   * @brief Creates context necessary for JobManager to funciton.
   */
  JobManager::create_handler_func_t CreateJobManagerContext()
  {
    return GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  }

  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  ApplicationModels m_models;
  LocalJobItem* m_job_item{nullptr};

  test::MockRemoteConnectionService m_mock_connection_service;
  UserContext m_user_context;
};

//! Create procedure, save as a project file, load from disk, and execute procedure in a job
//! manager.
TEST_F(IntegrationScenarioTest, SaveToDiskLoadAndRun)
{
  auto procedure_item = test::CreateCopyProcedureItem(GetSequencerModel());

  const auto file_name = GetFilePath("XmlDocumentSaveLoad.xml");

  mvvm::XmlDocument document({GetSequencerModel()});

  // saving model in file
  document.Save(file_name);

  // loading from file
  document.Load(file_name);

  auto loaded_procedure = GetSequencerModel()->GetProcedures().at(0);
  EXPECT_NE(procedure_item, loaded_procedure);

  m_job_item->SetProcedure(loaded_procedure);

  JobManager manager(CreateJobManagerContext());

  EXPECT_NO_THROW(manager.SubmitJob(m_job_item));

  auto job_handler = manager.GetJobHandler(m_job_item);
  ASSERT_TRUE(job_handler != nullptr);

  EXPECT_FALSE(job_handler->IsRunning());

  auto procedure = job_handler->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariableCount(), 2);

  // starting procedure
  manager.Start(m_job_item);
  QTest::qWait(20);

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  auto var_inside0 = vars_inside.at(0);
  auto var_inside1 = vars_inside.at(1);

  const sup::dto::AnyValue anyvalue0{sup::dto::SignedInteger32Type, 42};
  const sup::dto::AnyValue anyvalue1{sup::dto::SignedInteger32Type, 43};
  auto predicate = [&manager, var_inside0, var_inside1, &anyvalue0, &anyvalue1]
  {
    const bool is_completed = !manager.HasRunningJobs();
    const bool var0_updated = test::IsEqual(*var_inside0, anyvalue0);
    const bool var1_updated = test::IsEqual(*var_inside1, anyvalue0);
    return is_completed && var0_updated && var1_updated;
  };

  EXPECT_TRUE(QTest::qWaitFor([&manager]() { return !manager.HasRunningJobs(); }, 100));

  EXPECT_TRUE(test::IsEqual(*var_inside0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var_inside1, anyvalue0));

  // variables at original model remained unchanged
  auto vars = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  auto var0 = vars.at(0);
  auto var1 = vars.at(1);

  EXPECT_TRUE(test::IsEqual(*var0, anyvalue0));
  EXPECT_TRUE(test::IsEqual(*var1, anyvalue1));
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
  mvvm::test::CreateTextFile(main_file_name, test::CreateProcedureString(main_procedure));

  const auto external_file_name = GetFilePath("external.xml");
  mvvm::test::CreateTextFile(external_file_name, test::CreateProcedureString(external_procedure));

  auto procedure_item = oac_tree_gui::ImportFromFile(main_file_name);
  auto procedure_item_ptr = procedure_item.get();

  auto model = GetSequencerModel();
  model->InsertItem(std::move(procedure_item), model->GetProcedureContainer(),
                    mvvm::TagIndex::Append());

  m_job_item->SetProcedure(procedure_item_ptr);

  JobManager manager(CreateJobManagerContext());

  EXPECT_NO_THROW(manager.SubmitJob(m_job_item));
}

//! Validating that external includes are correctly found, and that varying parameter is propagated.
TEST_F(IntegrationScenarioTest, ExternalIncludeWithVaryingParameter)
{
  const std::string main_procedure{R"(
    <Sequence isRoot="True">
        <Include name="External Wait" path="Just Wait" file="external2.xml" par1="42"/>
    </Sequence>
)"};

  const std::string external_procedure{R"(
  <Wait name = "Just Wait" timeout="$par1" />
)"};

  const auto main_file_name = GetFilePath("main2.xml");
  mvvm::test::CreateTextFile(main_file_name, test::CreateProcedureString(main_procedure));

  const auto external_file_name = GetFilePath("external2.xml");
  mvvm::test::CreateTextFile(external_file_name, test::CreateProcedureString(external_procedure));

  auto procedure_item = oac_tree_gui::ImportFromFile(main_file_name);
  auto procedure_item_ptr = procedure_item.get();

  auto model = GetSequencerModel();
  model->InsertItem(std::move(procedure_item), model->GetProcedureContainer(),
                    mvvm::TagIndex::Append());

  m_job_item->SetProcedure(procedure_item_ptr);

  JobManager manager(CreateJobManagerContext());

  EXPECT_NO_THROW(manager.SubmitJob(m_job_item));

  auto wait_items =
      FindInstructions(*m_models.GetJobModel(), domainconstants::kWaitInstructionType);
  ASSERT_EQ(wait_items.size(), 1);
}

}  // namespace oac_tree_gui::test
