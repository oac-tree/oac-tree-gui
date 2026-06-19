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

#include "oac_tree_gui/views/operation/operation_monitor_view.h"

#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/operation/objects/operation_action_handler.h>
#include <oac_tree_gui/operation/operation_action_helper.h>
#include <oac_tree_gui/views/operation/operation_job_panel.h>
#include <oac_tree_gui/views/operation/operation_realtime_panel.h>
#include <oac_tree_gui/views/operation/realtime_instruction_tree_widget.h>

#include <sup/gui/app/null_command_service.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedure_items.h>

#include <QTreeView>

namespace oac_tree_gui
{

/**
 * @brief Tests for OperationMonitorView.
 */
class OperationMonitorViewTest : public ::testing::Test
{
public:
  OperationMonitorViewTest() { m_models.CreateEmpty(); }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }
  std::vector<JobItem*> GetJobItems() { return GetJobModel()->GetJobItems(); }

  ApplicationModels m_models;
  sup::gui::NullCommandService m_command_service;
};

//! We submit a job, then resubmit and check that real time tree shows non-empty instruction tree.
//! (real-life bug)

TEST_F(OperationMonitorViewTest, ReloadJob)
{
  OperationMonitorView view(m_command_service, OperationPresentationMode::kOperationMode);
  view.SetModels(&m_models);
  view.show();

  // accessing internals
  auto action_handler = view.GetOperationActionHandler();
  ASSERT_NE(action_handler, nullptr);
  auto operation_realtime_panel = view.findChild<OperationRealTimePanel*>();
  ASSERT_NE(operation_realtime_panel, nullptr);
  auto operation_job_panel = view.findChild<OperationJobPanel*>();
  ASSERT_NE(operation_job_panel, nullptr);
  auto realtime_instruction_tree_widget =
      operation_realtime_panel->findChild<RealTimeInstructionTreeWidget*>();
  ASSERT_NE(realtime_instruction_tree_widget, nullptr);
  auto realtime_instruction_tree = realtime_instruction_tree_widget->findChild<QTreeView*>();
  ASSERT_NE(realtime_instruction_tree, nullptr);

  // create procedure
  auto procedure = test::CreateMessageProcedureItem(GetSequencerModel(), "text");
  procedure->SetDisplayName("procedure_display_name");

  // submit procedure to create job item
  EXPECT_TRUE(GetJobItems().empty());
  EXPECT_TRUE(action_handler->SubmitLocalJob(procedure));
  ASSERT_EQ(GetJobItems().size(), 1);
  auto job_item = GetJobItems().at(0);

  // tree sees single message instruction
  EXPECT_EQ(realtime_instruction_tree->model()->rowCount(), 1);
  EXPECT_EQ(realtime_instruction_tree->model()->columnCount(), 4);

  // resubmit same procedure again
  EXPECT_TRUE(action_handler->OnRegenerateJobRequest());

  // tree still sees single message instruction
  EXPECT_EQ(realtime_instruction_tree->model()->rowCount(), 1);
  EXPECT_EQ(realtime_instruction_tree->model()->columnCount(), 4);
}

}  // namespace oac_tree_gui
