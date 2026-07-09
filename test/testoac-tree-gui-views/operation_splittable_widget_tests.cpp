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

#include "oac_tree_gui/views/operation/operation_splittable_widget.h"

#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/views/operation/instruction_task_monitor.h>
#include <oac_tree_gui/views/operation/instruction_task_widget_builder.h>
#include <oac_tree_gui/views/operation/operation_tab_widget.h>

#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/model/item_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/standard_procedure_items.h>

#include <QSplitter>
#include <QTabWidget>
#include <vector>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for OperationSplittableWidget class.
 */
class OperationSplittableWidgetTest : public ::testing::Test
{
public:
  OperationSplittableWidgetTest() { m_models.CreateEmpty(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  JobItem* InsertJob() { return GetJobModel()->InsertItem<LocalJobItem>(); }

  ApplicationModels m_models;
};

//! Freshly constructed widget has a controller and a single panel.
TEST_F(OperationSplittableWidgetTest, InitialState)
{
  const OperationSplittableWidget widget;

  ASSERT_NE(widget.GetController(), nullptr);
  EXPECT_EQ(widget.GetController()->GetPanels().size(), 1);

  auto splitter = widget.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  EXPECT_TRUE(widget.GetActiveJobs().empty());
}

//! Creating a panel adds an OperationTabWidget-based panel to the splitter.
TEST_F(OperationSplittableWidgetTest, CreatePanel)
{
  OperationSplittableWidget widget;

  EXPECT_NO_THROW(widget.CreatePanel());

  auto panels = widget.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_NE(panels.at(0)->GetMainEditor<OperationTabWidget>(), nullptr);
  EXPECT_NE(panels.at(1)->GetMainEditor<OperationTabWidget>(), nullptr);
}

//! SetModel propagates the model to all panels, and the combo lists the model's jobs.
TEST_F(OperationSplittableWidgetTest, SetModelPropagatesToPanels)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());
  widget.CreatePanel();

  InsertJob();
  InsertJob();

  auto panels = widget.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetModel(), GetJobModel());
  EXPECT_EQ(panels.at(1)->GetModel(), GetJobModel());
  EXPECT_EQ(panels.at(0)->GetAvailableItems().size(), 2);
}

//! SetCurrentJob shows the job in the focused panel and it appears in the active-jobs set.
TEST_F(OperationSplittableWidgetTest, SetCurrentJobShowsJobInFocusPanel)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());

  auto job = InsertJob();
  widget.SetCurrentJob(job);

  auto focus_widget = widget.GetController()->GetFocusWidget();
  ASSERT_NE(focus_widget, nullptr);
  EXPECT_EQ(focus_widget->GetCurrentItem(), job);
  EXPECT_THAT(widget.GetActiveJobs(), ::testing::ElementsAre(job));
}

//! Adding a panel emits activeJobsChanged with the current union.
TEST_F(OperationSplittableWidgetTest, ActiveJobsChangedEmittedOnPanelAdded)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());

  auto job = InsertJob();
  widget.SetCurrentJob(job);  // initial panel shows the job (emitted before we start recording)

  std::vector<std::vector<JobItem*>> recorded;
  QObject::connect(&widget, &OperationSplittableWidget::activeJobsChanged,
                   [&recorded](const std::vector<JobItem*>& jobs) { recorded.push_back(jobs); });

  widget.CreatePanel();  // new (empty) panel; union is unchanged

  ASSERT_EQ(recorded.size(), 1);
  EXPECT_THAT(recorded.back(), ::testing::ElementsAre(job));
}

//! Changing the job shown in a panel emits activeJobsChanged; reselecting the same job does not.
TEST_F(OperationSplittableWidgetTest, ActiveJobsChangedEmittedWhenPanelJobChanges)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());

  auto job = InsertJob();

  std::vector<std::vector<JobItem*>> recorded;
  QObject::connect(&widget, &OperationSplittableWidget::activeJobsChanged,
                   [&recorded](const std::vector<JobItem*>& jobs) { recorded.push_back(jobs); });

  widget.SetCurrentJob(job);
  ASSERT_EQ(recorded.size(), 1);
  EXPECT_THAT(recorded.back(), ::testing::ElementsAre(job));

  // reselecting the same job produces no new emission
  widget.SetCurrentJob(job);
  EXPECT_EQ(recorded.size(), 1);
}

//! GetActiveJobs returns the union of jobs shown across all panels.
TEST_F(OperationSplittableWidgetTest, GetActiveJobsUnionAcrossPanels)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());
  widget.CreatePanel();

  auto job0 = InsertJob();
  auto job1 = InsertJob();

  auto panels = widget.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  panels.at(0)->SetItem(job0);
  panels.at(1)->SetItem(job1);

  EXPECT_THAT(widget.GetActiveJobs(), ::testing::ElementsAre(job0, job1));

  // the same job shown in two panels is reported twice (deduplication is JobManager's concern)
  panels.at(1)->SetItem(job0);
  EXPECT_THAT(widget.GetActiveJobs(), ::testing::ElementsAre(job0, job0));
}

//! Closing a panel emits activeJobsChanged with the closing panel's job excluded.
TEST_F(OperationSplittableWidgetTest, ActiveJobsChangedOnCloseExcludesClosingPanel)
{
  OperationSplittableWidget widget;
  widget.SetModel(GetJobModel());
  widget.CreatePanel();

  auto job0 = InsertJob();
  auto job1 = InsertJob();

  auto panels = widget.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  panels.at(0)->SetItem(job0);
  panels.at(1)->SetItem(job1);

  std::vector<std::vector<JobItem*>> recorded;
  QObject::connect(&widget, &OperationSplittableWidget::activeJobsChanged,
                   [&recorded](const std::vector<JobItem*>& jobs) { recorded.push_back(jobs); });

  widget.GetController()->ClosePanel(panels.at(1));

  ASSERT_FALSE(recorded.empty());
  EXPECT_THAT(recorded.back(), ::testing::ElementsAre(job0));
}

//! We have single panel (with InstructionTaskMonitor selected) looking on a job with expanded
//! procedure. Removal of the expanded job should clean-up the panel.
TEST_F(OperationSplittableWidgetTest, DISABLED_RemoveExpandedProcedure)
{
  OperationSplittableWidget operation_splittable_widget;
  operation_splittable_widget.show();
  operation_splittable_widget.SetModel(GetJobModel());

  auto job0 = InsertJob();

  // creating expanded procedure
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "abc");
  job0->SetProcedure(procedure);
  LocalJobHandler job_handler(job0, UserContext{});
  ASSERT_NE(job0->GetExpandedProcedure(), nullptr);

  // accessing our only panel and underlying tab operation_splittable_widget
  auto panels = operation_splittable_widget.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 1);
  auto splittable_combo_panel = panels.at(0);
  auto operation_tab_widget = splittable_combo_panel->GetMainEditor<OperationTabWidget>();
  ASSERT_NE(operation_tab_widget, nullptr);
  auto tab_widget = operation_tab_widget->findChild<QTabWidget*>();
  ASSERT_NE(tab_widget, nullptr);
  auto task_monitor = operation_tab_widget->findChild<InstructionTaskMonitor*>();
  ASSERT_NE(task_monitor, nullptr);

  // it doesn't have model and item
  EXPECT_EQ(splittable_combo_panel->GetModel(), GetJobModel());
  EXPECT_EQ(splittable_combo_panel->GetCurrentItem(), nullptr);
  EXPECT_EQ(operation_tab_widget->GetItem(), nullptr);

  // setting a last tab (TaskMonitor)
  tab_widget->setCurrentIndex(4);

  // setting item
  operation_splittable_widget.SetCurrentJob(job0);

  EXPECT_EQ(splittable_combo_panel->GetCurrentItem(), job0);
  EXPECT_EQ(operation_tab_widget->GetItem(), job0);

  // validating what is shown inside task monitor
  ASSERT_NE(task_monitor->GetTaskWidgetBuilder(), nullptr);
  EXPECT_EQ(task_monitor->GetTaskWidgetBuilder()->GetInstructionCount(), 1U);

  mvvm::utils::RemoveItem(*job0->GetExpandedProcedure());
  EXPECT_EQ(task_monitor->GetTaskWidgetBuilder()->GetInstructionCount(), 0U);
}

}  // namespace oac_tree_gui::test
