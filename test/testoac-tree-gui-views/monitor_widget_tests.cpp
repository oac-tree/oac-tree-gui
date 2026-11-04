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

#include "oac_tree_gui/views/pvmonitor/monitor_widget.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_settings_manager.h>
#include <testutils/test_utils.h>

#include <QSignalSpy>

namespace oac_tree_gui
{

//! Tests of MonitorWidget class.

class MonitorWidgetTest : public ::testing::Test
{
public:
  MonitorWidgetTest() { m_models.CreateEmpty(); }

  //! Returns the first procedure.
  ProcedureItem* GetProcedure() { return m_models.GetSequencerModel()->GetProcedures().at(0); }

  //! Inserts a local variable into the workspace of the first procedure.
  VariableItem* InsertLocalVariable(const std::string& name, const sup::dto::AnyValue& value)
  {
    auto result =
        test::InsertVariable(domainconstants::kLocalVariableType, GetProcedure()->GetWorkspace());
    result->SetName(name);
    SetAnyValue(value, *result);
    return result;
  }

  sup::gui::NullCommandService m_command_service;
  test::TestSettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
  ApplicationModels m_models;
};

TEST_F(MonitorWidgetTest, StartStopStart)
{
  (void)InsertLocalVariable("abc", sup::dto::AnyValue(sup::dto::SignedInteger32Type, 42));

  MonitorWidget widget(m_context);
  widget.SetModels(&m_models);

  EXPECT_NO_FATAL_FAILURE(widget.OnStartMonitoringRequest());
  EXPECT_NO_FATAL_FAILURE(widget.OnStopMonitoringRequest());
  EXPECT_NO_FATAL_FAILURE(widget.OnStartMonitoringRequest());
  EXPECT_EQ(m_models.GetJobModel()->GetJobItems().size(), 1);
}

//! The widget shows the workspace of the first procedure. If there is no procedure, it creates one.
TEST_F(MonitorWidgetTest, SetModels)
{
  auto procedure0 = GetProcedure();
  auto sequencer_model = m_models.GetSequencerModel();
  sequencer_model->CreateUntitledProcedure();

  MonitorWidget widget(m_context);
  widget.SetModels(&m_models);
  EXPECT_EQ(widget.GetWorkspaceItem(), procedure0->GetWorkspace());

  for (auto procedure : sequencer_model->GetProcedures())
  {
    sequencer_model->RemoveItem(procedure);
  }

  widget.SetModels(&m_models);
  ASSERT_EQ(sequencer_model->GetProcedures().size(), 1);
  EXPECT_EQ(widget.GetWorkspaceItem(), sequencer_model->GetProcedures().at(0)->GetWorkspace());
}

//! Start shows the workspace of the job's expanded procedure. Values arrive from the domain.
TEST_F(MonitorWidgetTest, StartShowsJobWorkspace)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  (void)InsertLocalVariable("abc", value);

  MonitorWidget widget(m_context);
  widget.SetModels(&m_models);
  QSignalSpy spy_running(&widget, &MonitorWidget::RunningStatusChanged);

  widget.OnStartMonitoringRequest();

  auto job_items = m_models.GetJobModel()->GetJobItems();
  ASSERT_EQ(job_items.size(), 1);
  auto expanded_workspace = job_items.at(0)->GetExpandedProcedure()->GetWorkspace();
  EXPECT_EQ(widget.GetWorkspaceItem(), expanded_workspace);
  EXPECT_NE(widget.GetWorkspaceItem(), GetProcedure()->GetWorkspace());

  ASSERT_EQ(spy_running.count(), 1);
  EXPECT_TRUE(spy_running.at(0).at(0).toBool());

  auto variable = expanded_workspace->GetVariables().at(0);
  EXPECT_TRUE(test::WaitQtFor(
      [variable, &value]() { return variable->IsAvailable() && test::IsEqual(*variable, value); }));
}

//! Stop shows the procedure workspace again and removes the job.
TEST_F(MonitorWidgetTest, StopShowsProcedureWorkspace)
{
  (void)InsertLocalVariable("abc", sup::dto::AnyValue(sup::dto::SignedInteger32Type, 42));

  MonitorWidget widget(m_context);
  widget.SetModels(&m_models);
  widget.OnStartMonitoringRequest();

  QSignalSpy spy_running(&widget, &MonitorWidget::RunningStatusChanged);
  widget.OnStopMonitoringRequest();

  EXPECT_EQ(widget.GetWorkspaceItem(), GetProcedure()->GetWorkspace());
  EXPECT_TRUE(m_models.GetJobModel()->GetJobItems().empty());
  ASSERT_EQ(spy_running.count(), 1);
  EXPECT_FALSE(spy_running.at(0).at(0).toBool());

  // second stop does nothing
  widget.OnStopMonitoringRequest();
  EXPECT_EQ(spy_running.count(), 1);
}

//! Jobs existing in the project are left untouched.
TEST_F(MonitorWidgetTest, ExistingJobs)
{
  (void)InsertLocalVariable("abc", sup::dto::AnyValue(sup::dto::SignedInteger32Type, 42));
  auto existing_job = m_models.GetJobModel()->InsertItem<LocalJobItem>();

  MonitorWidget widget(m_context);
  widget.SetModels(&m_models);

  widget.OnStartMonitoringRequest();
  EXPECT_EQ(m_models.GetJobModel()->GetJobItems().size(), 2);

  widget.OnStopMonitoringRequest();
  EXPECT_EQ(m_models.GetJobModel()->GetJobItems(), std::vector<JobItem*>({existing_job}));
}

TEST_F(MonitorWidgetTest, CollectSettings)
{
  const MonitorWidget widget(m_context);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  auto storage = m_settings_manager.GetStorage(sup::gui::SettingsScope::kApplication);
  EXPECT_TRUE(storage->Read("WorkspaceVariableTreeView/tree_header/state").has_value());
  EXPECT_TRUE(storage->Read("WorkspaceVariableTreeView/table_header/state").has_value());
}

}  // namespace oac_tree_gui
