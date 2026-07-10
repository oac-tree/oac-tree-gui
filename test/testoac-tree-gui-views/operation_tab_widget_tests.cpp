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

#include "oac_tree_gui/views/operation/operation_tab_widget.h"

#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/views/composer/workspace_variable_tree_view.h>
#include <oac_tree_gui/views/operation/monitor_realtime_actions.h>
#include <oac_tree_gui/views/operation/realtime_instruction_tree_widget.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedure_items.h>

#include <QAbstractItemModel>
#include <QApplication>
#include <QTabWidget>
#include <QTreeView>
#include <chrono>
#include <vector>

Q_DECLARE_METATYPE(oac_tree_gui::JobItem*)

namespace oac_tree_gui::test
{

using msec = std::chrono::milliseconds;

/**
 * @brief Tests for OperationTabWidget class.
 */
class OperationTabWidgetTest : public ::testing::Test
{
public:
  OperationTabWidgetTest() { m_models.CreateEmpty(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  ApplicationModels m_models;
};

// ------------------------------------------------------------------------------------------------
// Tier 1: behavior that needs no expanded procedure.
// ------------------------------------------------------------------------------------------------

//! Freshly constructed widget has no job and exposes the expected set of tabs.
TEST_F(OperationTabWidgetTest, InitialState)
{
  const OperationTabWidget widget;

  EXPECT_EQ(widget.GetItem(), nullptr);

  auto tab_widget = widget.findChild<QTabWidget*>();
  ASSERT_NE(tab_widget, nullptr);
  ASSERT_EQ(tab_widget->count(), 5);
  EXPECT_EQ(tab_widget->tabText(0), QString("Instructions"));
  EXPECT_EQ(tab_widget->tabText(1), QString("Variable tree"));
  EXPECT_EQ(tab_widget->tabText(2), QString("Variable table"));
  EXPECT_EQ(tab_widget->tabText(3), QString("Node view"));
  EXPECT_EQ(tab_widget->tabText(4), QString("Task monitor"));
}

//! Setting a null job is safe and leaves the widget without a current job.
TEST_F(OperationTabWidgetTest, SetCurrentJobNullptrIsSafe)
{
  OperationTabWidget widget;

  EXPECT_NO_FATAL_FAILURE(widget.SetCurrentJob(nullptr));
  EXPECT_EQ(widget.GetItem(), nullptr);
}

//! SetItem delegates to SetCurrentJob; a non-JobItem resets the current job to nullptr.
TEST_F(OperationTabWidgetTest, SetItemDelegatesToSetCurrentJob)
{
  auto job = GetJobModel()->InsertItem<LocalJobItem>();

  OperationTabWidget widget;

  widget.SetItem(job);
  EXPECT_EQ(widget.GetItem(), job);

  // an item which is not a JobItem clears the current job
  auto procedure = GetSequencerModel()->InsertItem<ProcedureItem>();
  widget.SetItem(procedure);
  EXPECT_EQ(widget.GetItem(), nullptr);
}

//! Tick timeout can be set and read back without emitting a change request.
TEST_F(OperationTabWidgetTest, TickTimeoutRoundTrip)
{
  OperationTabWidget widget;

  widget.SetCurrentTickTimeout(42);
  EXPECT_EQ(widget.GetCurrentTickTimeout(), 42);
}

//! Toolbar actions are forwarded as widget signals, stamped with the current job.
TEST_F(OperationTabWidgetTest, SignalForwardingStampsCurrentJob)
{
  auto job = GetJobModel()->InsertItem<LocalJobItem>();

  OperationTabWidget widget;
  widget.SetCurrentJob(job);

  auto actions = widget.findChild<MonitorRealTimeActions*>();
  ASSERT_NE(actions, nullptr);

  using ActionKey = MonitorRealTimeActions::ActionKey;

  // record the job pointer forwarded by each signal
  std::vector<JobItem*> run;
  std::vector<JobItem*> pause;
  std::vector<JobItem*> step;
  std::vector<JobItem*> stop;
  std::vector<JobItem*> reset;

  QObject::connect(&widget, &OperationTabWidget::RunRequest,
                   [&run](JobItem* job) { run.push_back(job); });
  QObject::connect(&widget, &OperationTabWidget::PauseRequest,
                   [&pause](JobItem* job) { pause.push_back(job); });
  QObject::connect(&widget, &OperationTabWidget::StepRequest,
                   [&step](JobItem* job) { step.push_back(job); });
  QObject::connect(&widget, &OperationTabWidget::StopRequest,
                   [&stop](JobItem* job) { stop.push_back(job); });
  QObject::connect(&widget, &OperationTabWidget::ResetRequest,
                   [&reset](JobItem* job) { reset.push_back(job); });

  auto trigger = [actions](ActionKey key) { actions->GetActions({key}).at(0)->trigger(); };

  trigger(ActionKey::kRun);
  trigger(ActionKey::kPause);
  trigger(ActionKey::kStep);
  trigger(ActionKey::kStop);
  trigger(ActionKey::kReset);

  EXPECT_EQ(run, std::vector<JobItem*>{job});
  EXPECT_EQ(pause, std::vector<JobItem*>{job});
  EXPECT_EQ(step, std::vector<JobItem*>{job});
  EXPECT_EQ(stop, std::vector<JobItem*>{job});
  EXPECT_EQ(reset, std::vector<JobItem*>{job});
}

// ------------------------------------------------------------------------------------------------
// Tier 2: light integration - a real expanded procedure fans out into the child widgets.
// ------------------------------------------------------------------------------------------------

//! After submitting a job, SetCurrentJob populates the real time tree, the workspace views, and
//! picks up the job's tick timeout.
TEST_F(OperationTabWidgetTest, SetCurrentJobPopulatesChildWidgets)
{
  // procedure with a single Copy instruction and two workspace variables
  auto procedure = test::CreateCopyProcedureItem(GetSequencerModel());

  auto job = GetJobModel()->InsertItem<LocalJobItem>();
  job->SetProcedure(procedure);
  job->SetTickTimeout(msec(42));

  // building the expanded procedure (this is what the job handler does on submission)
  LocalJobHandler job_handler(job, UserContext{});
  ASSERT_NE(job->GetExpandedProcedure(), nullptr);

  OperationTabWidget widget;
  widget.SetCurrentJob(job);
  widget.show();  // workspace views only populate while their tab is visible (visibility agent)
  EXPECT_EQ(widget.GetItem(), job);

  // real time instruction tree (default tab) shows the single instruction
  auto realtime_tree_widget = widget.findChild<RealTimeInstructionTreeWidget*>();
  ASSERT_NE(realtime_tree_widget, nullptr);
  auto realtime_tree = realtime_tree_widget->findChild<QTreeView*>();
  ASSERT_NE(realtime_tree, nullptr);
  EXPECT_EQ(realtime_tree->model()->rowCount(), 1);
  EXPECT_EQ(realtime_tree->model()->columnCount(), 4);

  // activating a workspace tab makes its view populate with the two workspace variables
  auto tab_widget = widget.findChild<QTabWidget*>();
  ASSERT_NE(tab_widget, nullptr);
  for (int index : {1, 2})  // "Variable tree" and "Variable table"
  {
    tab_widget->setCurrentIndex(index);
    QApplication::processEvents();

    auto view = qobject_cast<WorkspaceVariableTreeView*>(tab_widget->widget(index));
    ASSERT_NE(view, nullptr);
    auto tree = view->findChild<QTreeView*>();
    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->model()->rowCount(), 2);
  }

  // tick timeout is taken from the job
  EXPECT_EQ(widget.GetCurrentTickTimeout(), 42);
}

// ------------------------------------------------------------------------------------------------
// Tier 3: expanded procedure life cycle while the same job stays selected. The JobItemController
// keeps the child views in sync when the expanded procedure is destroyed or (re-)created, as
// happens when a running job is re-run.
// ------------------------------------------------------------------------------------------------

//! When the expanded procedure is destroyed while its job stays selected, the child views are
//! cleared (no dangling pointers) without an explicit SetCurrentJob() call.
TEST_F(OperationTabWidgetTest, ExpandedProcedureDestroyedClearsViews)
{
  auto job = GetJobModel()->InsertItem<LocalJobItem>();
  (void)GetJobModel()->InsertItem(test::CreateCopyProcedureItem(), job, mvvm::TagIndex::Append());
  ASSERT_NE(job->GetExpandedProcedure(), nullptr);

  OperationTabWidget widget;
  widget.SetCurrentJob(job);
  widget.show();

  auto realtime_tree_widget = widget.findChild<RealTimeInstructionTreeWidget*>();
  ASSERT_NE(realtime_tree_widget, nullptr);
  auto realtime_tree = realtime_tree_widget->findChild<QTreeView*>();
  ASSERT_NE(realtime_tree, nullptr);
  EXPECT_EQ(realtime_tree->model()->rowCount(), 1);

  // destroying the expanded procedure clears the real time tree via the controller callback
  mvvm::utils::RemoveItem(*job->GetExpandedProcedure());
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
  EXPECT_EQ(realtime_tree->model()->rowCount(), 0);

  // the job is still the current one, only its expanded procedure is gone
  EXPECT_EQ(widget.GetItem(), job);
}

//! When a new expanded procedure appears while its job stays selected, the child views repopulate
//! automatically, without an explicit SetCurrentJob() call. This is the behavior the controller
//! adds on top of the previous destruction-only listener.
TEST_F(OperationTabWidgetTest, ExpandedProcedureReappearsRepopulatesViews)
{
  auto job = GetJobModel()->InsertItem<LocalJobItem>();

  OperationTabWidget widget;
  widget.SetCurrentJob(job);  // job without expanded procedure yet
  widget.show();

  auto realtime_tree_widget = widget.findChild<RealTimeInstructionTreeWidget*>();
  ASSERT_NE(realtime_tree_widget, nullptr);
  auto realtime_tree = realtime_tree_widget->findChild<QTreeView*>();
  ASSERT_NE(realtime_tree, nullptr);
  EXPECT_EQ(realtime_tree->model()->rowCount(), 0);

  // inserting an expanded procedure repopulates the real time tree via the controller callback
  (void)GetJobModel()->InsertItem(test::CreateCopyProcedureItem(), job, mvvm::TagIndex::Append());
  ASSERT_NE(job->GetExpandedProcedure(), nullptr);
  EXPECT_EQ(realtime_tree->model()->rowCount(), 1);
}

}  // namespace oac_tree_gui::test
