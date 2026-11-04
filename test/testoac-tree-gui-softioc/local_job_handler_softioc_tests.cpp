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
#include <sup/epics-test/softioc_runner.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

#include <chrono>

namespace oac_tree_gui::test
{

namespace
{
const std::string kDbContent = R"RAW(
record (longout,"LOCAL-JOB-HANDLER-SOFTIOC-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"42")
}
)RAW";

}  // namespace

//! Tests for LocalJobHandler running a procedure with a ChannelAccess variable and without
//! instructions. The job is submitted through JobManager and never started.
class LocalJobHandlerSoftiocTest : public ::testing::Test
{
public:
  LocalJobHandlerSoftiocTest()
  {
    m_models.CreateEmpty();
    m_procedure = m_models.GetSequencerModel()->GetProcedures().at(0);
  }

  //! Disables all tests in the fixture if ChannelAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
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

  ApplicationModels m_models;
  ProcedureItem* m_procedure{nullptr};
  JobManager m_job_manager{[](JobItem& item)
                           { return std::make_unique<LocalJobHandler>(&item, UserContext{}); }};
};

//! Single ChannelAccess variable. The expanded variable follows softIoc connect and disconnect.
TEST_F(LocalJobHandlerSoftiocTest, ConnectAndDisconnect)
{
  const std::string channel("LOCAL-JOB-HANDLER-SOFTIOC-TESTS:INT");
  const sup::dto::AnyValue softioc_value({{"value", {sup::dto::UnsignedInteger32Type, 42}},
                                          {"connected", {sup::dto::BooleanType, true}}});

  sup::epics::test::SoftIocRunner softioc_service;
  softioc_service.Start(kDbContent);

  auto variable =
      InsertVariable(domainconstants::kChannelAccessVariableType, m_procedure->GetWorkspace());
  variable->SetAttribute(domainconstants::kChannelAttribute, channel);
  variable->SetName("var");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::UnsignedInteger32Type, 0}},
                                          {"connected", {sup::dto::BooleanType, false}}});
  SetAnyValue(initial_value, *variable);

  auto job = SubmitJob();
  auto expanded_variable = job->GetExpandedProcedure()->GetWorkspace()->GetVariables().at(0);

  EXPECT_TRUE(WaitQtFor(
      [expanded_variable, &softioc_value]()
      { return expanded_variable->IsAvailable() && IsEqual(*expanded_variable, softioc_value); }));

  softioc_service.Stop();

  EXPECT_TRUE(WaitQtFor([expanded_variable]() { return !expanded_variable->IsAvailable(); }));
  EXPECT_FALSE(expanded_variable->GetAnyValueItem()->GetChildren().at(1)->Data<bool>());
}

}  // namespace oac_tree_gui::test
