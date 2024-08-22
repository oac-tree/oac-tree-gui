/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/pvmonitor/workspace_synchronizer.h"

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/test/mock_model_listener.h>

#include <sup/epics-test/softioc_runner.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/test_utils.h>

#include <QTest>

using namespace sequencergui;

namespace
{
const std::string kDbContent = R"RAW(
record (longout,"WORKSPACE-SYNCHRONIZER-SOFTIOC-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"42")
}
)RAW";

}

namespace sequencergui
{

//! Testing WorkspaceSynchronizer for a single SoftIoc variable in a full cicrle: connect, change
//! value, disconnect.
class WorkspaceSynchronizerSoftiocTest : public ::testing::Test
{
public:
  WorkspaceSynchronizerSoftiocTest() { m_model.InsertItem<WorkspaceItem>(); }

  //! Creates synchronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  //! Disables all tests in the fixture if ChannelAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

// sup::epics::test::SoftIocRunner WorkspaceSynchronizerSoftiocTest::m_softioc_service{};

TEST_F(WorkspaceSynchronizerSoftiocTest, ConnectAndDisconnect)
{
  // initial values
  const std::string kChannelName("WORKSPACE-SYNCHRONIZER-SOFTIOC-TESTS:INT");
  const std::string var_name("var");
  const sup::dto::AnyValue softioc_initial_value(
      {{"value", {sup::dto::UnsignedInteger32Type, 42}}});

  // start SoftIoc
  sup::epics::test::SoftIocRunner m_softioc_service;
  m_softioc_service.Start(kDbContent);

  // creating ChannelAccessVariableItem
  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<ChannelAccessVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::UnsignedInteger32Type, 0}}});
  SetAnyValue(initial_value, *variable_item);
  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();

  // Creating listeners and setting callback expectations.
  mvvm::test::MockModelListenerV2 model_listener(&m_model);

  // WorkspaceSynchronizer::Start method below will cause DataChangedEvents

  // expecting an event about IsAvailable status change
  auto is_available_property = GetIsAvailableItem(*variable_item);
  auto expected_event1 = mvvm::DataChangedEvent{is_available_property, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);

  // expecting event with appearance change (gray rectangle is getting green)
  auto channel_property = variable_item->GetItem(domainconstants::kChannelAttribute);
  auto expected_event2 = mvvm::DataChangedEvent{channel_property, mvvm::DataRole::kAppearance};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(1);

  // expecting event with value of scalar change
  auto value_item = variable_item->GetAnyValueItem()->GetChildren().at(0);
  auto expected_event3 = mvvm::DataChangedEvent{value_item, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event3)).Times(1);

  synchronizer->Start();

  // waiting for domain workspace noticing SoftIoc variable
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 10.0));

  // checking the value on domain side
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, softioc_initial_value);

  // queued signals need special waiting to fly to the GUI side
  QTest::qWait(100);

  // checking the value on GUI side
  EXPECT_TRUE(variable_item->IsAvailable());
  auto gui_value = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
  EXPECT_EQ(gui_value, softioc_initial_value);

  // expecting an event about IsAvailable status change back to false
  EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);

  // disconnecting SoftIoc
  m_softioc_service.Stop();

  auto predicate = [variable_item]()
  { return !variable_item->IsAvailable();  };
  EXPECT_TRUE(QTest::qWaitFor(predicate, 1000));  // letting event loop to work

}

}  // namespace sequencergui
