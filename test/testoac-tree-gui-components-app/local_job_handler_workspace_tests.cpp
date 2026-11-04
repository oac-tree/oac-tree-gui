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

#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

#include <QCoreApplication>
#include <chrono>

namespace oac_tree_gui::test
{

//! Tests for LocalJobHandler running a procedure with a workspace and without instructions.
//! Jobs are submitted through JobManager and never started.
class LocalJobHandlerWorkspaceTest : public ::testing::Test
{
public:
  LocalJobHandlerWorkspaceTest()
  {
    m_models.CreateEmpty();
    m_procedure = m_models.GetSequencerModel()->GetProcedures().at(0);
  }

  //! Inserts a local variable into the procedure workspace.
  VariableItem* InsertLocalVariable(const std::string& name, const sup::dto::AnyValue& value)
  {
    auto result = InsertVariable(domainconstants::kLocalVariableType, m_procedure->GetWorkspace());
    result->SetName(name);
    SetAnyValue(value, *result);
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

  //! Returns the first variable of the expanded procedure.
  static VariableItem* GetExpandedVariable(JobItem& job)
  {
    return job.GetExpandedProcedure()->GetWorkspace()->GetVariables().at(0);
  }

  //! Processes Qt events until the variable is available and has the given value.
  static bool WaitForValue(const VariableItem& variable, const sup::dto::AnyValue& value)
  {
    return WaitQtFor([&variable, &value]()
                     { return variable.IsAvailable() && IsEqual(variable, value); });
  }

  ApplicationModels m_models;
  ProcedureItem* m_procedure{nullptr};
  JobManager m_job_manager{[](JobItem& item)
                           { return std::make_unique<LocalJobHandler>(&item, UserContext{}); }};
};

//! Submission alone sets up the domain workspace. The expanded variable gets its value without
//! Start.
TEST_F(LocalJobHandlerWorkspaceTest, InitialValue)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto variable = InsertLocalVariable("var", value);

  auto job = SubmitJob();
  auto job_handler = m_job_manager.GetJobHandler(job);
  ASSERT_NE(job_handler, nullptr);

  auto expanded_variable = GetExpandedVariable(*job);
  EXPECT_NE(expanded_variable, variable);
  EXPECT_EQ(expanded_variable->GetName(), "var");

  EXPECT_TRUE(WaitForValue(*expanded_variable, value));
  EXPECT_EQ(job_handler->GetRunnerStatus(), RunnerStatus::kInitial);
}

//! A value set in the expanded workspace doesn't change the procedure workspace.
TEST_F(LocalJobHandlerWorkspaceTest, SetValueInExpandedWorkspace)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto variable = InsertLocalVariable("var", value);

  auto job = SubmitJob();
  auto expanded_variable = GetExpandedVariable(*job);
  EXPECT_TRUE(WaitForValue(*expanded_variable, value));

  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  SetAnyValue(new_value, *expanded_variable);

  EXPECT_TRUE(IsEqual(*expanded_variable, new_value));
  EXPECT_TRUE(IsEqual(*variable, value));
}

//! Removal of the handler leaves the expanded workspace with its last values. No events arrive
//! after removal.
TEST_F(LocalJobHandlerWorkspaceTest, RemoveJobHandler)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  (void)InsertLocalVariable("var", value);

  auto job = SubmitJob();
  auto expanded_variable = GetExpandedVariable(*job);
  EXPECT_TRUE(WaitForValue(*expanded_variable, value));

  m_job_manager.RemoveJobHandler(job, RemovalPolicy::kForce);
  EXPECT_EQ(m_job_manager.GetJobHandler(job), nullptr);
  EXPECT_EQ(m_job_manager.GetJobCount(), 0);

  const ::testing::StrictMock<mvvm::test::MockModelListener> model_listener(m_models.GetJobModel());
  QCoreApplication::processEvents();

  EXPECT_TRUE(expanded_variable->IsAvailable());
  EXPECT_TRUE(IsEqual(*expanded_variable, value));
}

//! Submit, remove the job, submit again. The new expanded workspace gets values from the domain.
TEST_F(LocalJobHandlerWorkspaceTest, SubmitRemoveSubmit)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  (void)InsertLocalVariable("var", value);

  auto job0 = SubmitJob();
  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job0), value));

  m_job_manager.RemoveJobHandler(job0, RemovalPolicy::kForce);
  m_models.GetJobModel()->RemoveItem(job0);

  auto job1 = SubmitJob();
  EXPECT_TRUE(WaitForValue(*GetExpandedVariable(*job1), value));
  EXPECT_EQ(m_job_manager.GetJobCount(), 1);
}

}  // namespace oac_tree_gui::test
