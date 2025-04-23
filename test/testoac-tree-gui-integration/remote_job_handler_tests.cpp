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

#include "test_automation_server.h"

#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/oac-tree-server/epics_config_utils.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>
#include <thread>

#include "oac_tree_gui/jobsystem/remote_job_handler.h"

namespace oac_tree_gui
{

class RemoteJobHandlerTest : public ::testing::Test
{
public:
  RemoteJobHandlerTest() { m_job_item = m_model.InsertItem<RemoteJobItem>(); }

  JobModel m_model;
  RemoteJobItem* m_job_item{nullptr};
};

TEST_F(RemoteJobHandlerTest, SimpleProcedure)
{
  const std::string kProcedureBodyText{
      R"RAW(
  <Repeat maxCount="3">
    <Sequence>
       <Increment varName="var0"/>
    </Sequence>
  </Repeat>
  <Workspace>
    <Local name="var0" type='{"type":"uint32"}' value='0'/>
  </Workspace>
)RAW"};

  const std::string server_name("RemoteJobHandlerTestServer");
  test::TestAutomationServer server;

  server.Start(server_name, kProcedureBodyText);

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  auto manager = sup::oac_tree_server::utils::CreateEPICSJobManager(server_name);
  EXPECT_EQ(manager->GetNumberOfJobs(), 1);

  RemoteJobHandler job_handler(m_job_item, *manager, 0, {});

  ASSERT_NE(m_job_item->GetExpandedProcedure(), nullptr);
  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(job_handler.GetRunnerStatus(), RunnerStatus::kInitial);

  job_handler.Start();
  QTest::qWait(50);
  EXPECT_FALSE(job_handler.IsRunning());
  EXPECT_EQ(m_job_item->GetStatus(), RunnerStatus::kSucceeded);

  auto variables = m_job_item->GetExpandedProcedure()->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 1);

  const sup::dto::AnyValue expected_value{sup::dto::UnsignedInteger32Type, 3};
  EXPECT_TRUE(test::IsEqual(*variables.at(0), expected_value));

  server.Stop();
}

}  // namespace oac_tree_gui
