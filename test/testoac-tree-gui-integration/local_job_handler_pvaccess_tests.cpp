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

#include "oac_tree_gui/jobsystem/objects/local_job_handler.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

#include <chrono>

namespace oac_tree_gui::test
{

namespace
{
const std::string kTestPrefix("LocalJobHandlerPVAccessTests:");
}  // namespace

//! Tests for LocalJobHandler running a procedure with PVAccess variables and without instructions.
//! Jobs are submitted through JobManager and never started. A separate domain workspace plays the
//! other end of the channel.
class LocalJobHandlerPVAccessTest : public ::testing::Test
{
public:
  LocalJobHandlerPVAccessTest()
  {
    m_models.CreateEmpty();
    m_procedure = m_models.GetSequencerModel()->GetProcedures().at(0);
  }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  //! Inserts a PVAccess variable into the procedure workspace.
  VariableItem* InsertPVAccessVariable(const std::string& domain_type, const std::string& name,
                                       const std::string& channel)
  {
    auto result = InsertVariable(domain_type, m_procedure->GetWorkspace());
    result->SetAttribute(domainconstants::kChannelAttribute, channel);
    result->SetName(name);
    return result;
  }

  //! Creates a job for the procedure and submits it.
  JobItem* SubmitJob()
  {
    auto result = m_models.GetJobModel()->InsertItem<LocalJobItem>();
    result->SetProcedure(m_procedure);
    result->SetTickTimeout(std::chrono::milliseconds(0));
    m_job_manager.SubmitJob(result);
    return result;
  }

  //! Returns the variable of the expanded procedure with the given index.
  static VariableItem* GetExpandedVariable(JobItem& job, std::size_t index)
  {
    return job.GetExpandedProcedure()->GetWorkspace()->GetVariables().at(index);
  }

  //! Processes Qt events until the variable is available and has the given value.
  static bool WaitForValue(const VariableItem& variable, const sup::dto::AnyValue& value)
  {
    return WaitQtFor([&variable, &value]()
                     { return variable.IsAvailable() && IsEqual(variable, value); });
  }

  sup::oac_tree::Workspace m_external_workspace;
  ApplicationModels m_models;
  ProcedureItem* m_procedure{nullptr};
  JobManager m_job_manager{[](JobItem& item)
                           { return std::make_unique<LocalJobHandler>(&item, UserContext{}); }};
};

//! Single server variable. The expanded variable gets the initial value without Start.
TEST_F(LocalJobHandlerPVAccessTest, ServerVariable)
{
  const std::string channel(kTestPrefix + "STRUCT1");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  auto variable =
      InsertPVAccessVariable(domainconstants::kPvAccessServerVariableType, "var", channel);
  SetAnyValue(initial_value, *variable);

  auto job = SubmitJob();
  auto expanded_variable = GetExpandedVariable(*job, 0);
  EXPECT_FALSE(expanded_variable->IsAvailable());

  EXPECT_TRUE(WaitForValue(*expanded_variable, initial_value));
  EXPECT_EQ(m_job_manager.GetJobHandler(job)->GetRunnerStatus(), RunnerStatus::kInitial);
}

//! Single server variable in the job. The data is set in the expanded workspace, an external
//! client gets the update.
TEST_F(LocalJobHandlerPVAccessTest, SetDataFromGUI)
{
  const std::string channel(kTestPrefix + "STRUCT2");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  auto variable =
      InsertPVAccessVariable(domainconstants::kPvAccessServerVariableType, "var", channel);
  SetAnyValue(initial_value, *variable);

  auto job = SubmitJob();
  auto expanded_variable = GetExpandedVariable(*job, 0);
  EXPECT_TRUE(WaitForValue(*expanded_variable, initial_value));

  m_external_workspace.AddVariable("client", CreatePVAccessClientVariable("client", {}, channel));
  m_external_workspace.Setup();
  EXPECT_TRUE(m_external_workspace.WaitForVariable("client", 20.0));

  // setting the data from GUI
  expanded_variable->GetAnyValueItem()->GetChildren().at(0)->SetData(42);

  const sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_TRUE(WaitQtFor([this, &expected_value]()
                        { return GetAnyValue("client", m_external_workspace) == expected_value; }));
  EXPECT_TRUE(IsEqual(*expanded_variable, expected_value));
}

//! Single client variable in the job. The data is set on an external server, the expanded
//! variable gets the update.
TEST_F(LocalJobHandlerPVAccessTest, SetDataFromDomain)
{
  const std::string channel(kTestPrefix + "STRUCT3");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  m_external_workspace.AddVariable("server",
                                   CreatePVAccessServerVariable("server", initial_value, channel));
  m_external_workspace.Setup();

  (void)InsertPVAccessVariable(domainconstants::kPvAccessClientVariableType, "var", channel);

  auto job = SubmitJob();
  auto expanded_variable = GetExpandedVariable(*job, 0);
  EXPECT_TRUE(WaitForValue(*expanded_variable, initial_value));

  // setting the data from the domain
  const sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_TRUE(m_external_workspace.SetValue("server", expected_value));

  EXPECT_TRUE(WaitForValue(*expanded_variable, expected_value));
}

//! One server and one client variable with initial values in the job.
TEST_F(LocalJobHandlerPVAccessTest, ClientAndServerVariableConnection)
{
  const std::string channel(kTestPrefix + "STRUCT4");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  auto server =
      InsertPVAccessVariable(domainconstants::kPvAccessServerVariableType, "server", channel);
  SetAnyValue(initial_value, *server);
  auto client =
      InsertPVAccessVariable(domainconstants::kPvAccessClientVariableType, "client", channel);
  SetAnyValue(initial_value, *client);

  auto job = SubmitJob();

  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job, 0), initial_value));
  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job, 1), initial_value));
}

//! One server and one client variable in the job. The client has no value, it gets the value from
//! the server on the first connection.
TEST_F(LocalJobHandlerPVAccessTest, ClientWithoutAnyValueAndServerVariableConnection)
{
  const std::string channel(kTestPrefix + "STRUCT5");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  auto server =
      InsertPVAccessVariable(domainconstants::kPvAccessServerVariableType, "server", channel);
  SetAnyValue(initial_value, *server);
  (void)InsertPVAccessVariable(domainconstants::kPvAccessClientVariableType, "client", channel);

  auto job = SubmitJob();

  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job, 0), initial_value));
  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job, 1), initial_value));
}

}  // namespace oac_tree_gui::test
