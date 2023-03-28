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

#include "suppvmonitor/workspace_synchronizer.h"

#include <gtest/gtest.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_monitor_helper.h>
#include <testutils/gui_domain_utils.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

#include <QTest>

using namespace suppvmonitor;
using ::testing::_;

namespace
{
const std::string kTestPrefix("WorkspaceSynchronizerPVAccessTests:");
const std::string kStructChannelName(kTestPrefix + "STRUCT");
}  // namespace

//! Tests for WorkspaceSyncronizer class.

class WorkspaceSynchronizerPVAccessTests : public ::testing::Test
{
public:
  WorkspaceSynchronizerPVAccessTests() { m_model.InsertItem<sequencergui::WorkspaceItem>(); }

  //! Creates syncronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    m_workspace.Setup();  // current convention: has to be setup before starting listening
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  static bool IsPVAccessAvailable()
  {
    return sequencergui::IsPVAccessServerAvailable() && sequencergui::IsPVAccessClientAvailable();
  }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsPVAccessAvailable())
    {
      GTEST_SKIP();
    }
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

//! Single server variable (struct) in a workspace.
//! Creating variable on GUI side, duplicating in the domain workspace. Starting listening,
//! making sure that no signals are flying and initial values coincide.

TEST_F(WorkspaceSynchronizerPVAccessTests, ServerVariableSimpleStart)
{
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item = m_model.GetWorkspaceItem()->InsertItem<sequencergui::PVServerVariableItem>(
      mvvm::TagIndex::Append());
  variable_item->SetChannel(kStructChannelName);
  variable_item->SetName(var_name);
  sequencergui::SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating syncronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();

  // Creating listeners and setting callback expectations.
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  testutils::MockModelListener model_listener(&m_model);

  // After domain workspace was set-up, the only callback will be DataChangedEvent for IsAvailable
  // status, caused by Start method.

  auto is_available_property = variable_item->GetItem("kIsAvailable");
  auto expected_event =
      mvvm::event_variant_t(mvvm::DataChangedEvent{is_available_property, mvvm::DataRole::kData});

  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);
  EXPECT_CALL(domain_listener, OnEvent(_, _, _)).Times(0);

  synchronizer->Start();

  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 1.0));

  // checking the value on domain side
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, initial_value);

  //  QTest::qWait(50);  // queued signals need special waiting

  // checking the value on GUI side
  EXPECT_TRUE(variable_item->IsAvailable());
  auto gui_value = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
  EXPECT_EQ(gui_value, initial_value);
}

//! Single server variable (struct) in a workspace.
//! The data is set on the GUI side, validating updates on the domain side.

TEST_F(WorkspaceSynchronizerPVAccessTests, SetDataFromGUI)
{
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item = m_model.GetWorkspaceItem()->InsertItem<sequencergui::PVServerVariableItem>(
      mvvm::TagIndex::Append());
  variable_item->SetChannel(kStructChannelName);
  variable_item->SetName(var_name);
  sequencergui::SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating syncronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // Creating domain and setting callback expectations.
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  auto anyvalue_item = variable_item->GetAnyValueItem();
  sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_CALL(domain_listener, OnEvent(var_name, expected_value, true)).Times(1);

  // creating model listener and setting expectations
  testutils::MockModelListener model_listener(&m_model);
  auto scalar_field = anyvalue_item->GetChildren().at(0);
  auto expected_event =
      mvvm::event_variant_t(mvvm::DataChangedEvent{scalar_field, mvvm::DataRole::kData});
  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);

  // setting the data from GUI (will trigger expectation)
  scalar_field->SetData(42);

  // validating the data in the domain
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, expected_value);
}
