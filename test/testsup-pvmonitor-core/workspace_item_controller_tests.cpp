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

#include "suppvmonitor/workspace_item_controller.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_event.h>
#include <testutils/gui_domain_utils.h>
#include <testutils/mock_callback_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/sequencer/workspace.h>

#include <stdexcept>

using namespace suppvmonitor;
using ::testing::_;

//! Tests for WorkspaceItemController class.

class WorkspaceItemControllerTests : public ::testing::Test
{
public:
  WorkspaceItemControllerTests()
  {
    m_workspace_item = m_model.InsertItem<sequencergui::WorkspaceItem>();
  }

  MonitorModel m_model;
  sequencergui::WorkspaceItem* m_workspace_item{nullptr};
};

TEST_F(WorkspaceItemControllerTests, InitialState)
{
  WorkspaceItemController controller(m_workspace_item);
  EXPECT_EQ(controller.GetWorkspaceItem(), m_workspace_item);
}

TEST_F(WorkspaceItemControllerTests, GeVariableItemForName)
{
  auto variable_item =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName("abc");

  WorkspaceItemController controller(m_workspace_item);
  EXPECT_EQ(controller.GeVariableItemForName("abc"), variable_item);
}

//! Setting up the WorkspaceItem with single variable.
//! Triggering domain workspace event and validating AnyValueItem update.

TEST_F(WorkspaceItemControllerTests, ProcessEventFromDomainWhenConnected)
{
  testutils::MockCallbackListener<WorkspaceEvent> listener;

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());

  testutils::SetupVariable("abc", value, *variable_item0);
  // initially VariableItem doesn't have AnyValueItem
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // expecting no callbacks on processing domain events
  EXPECT_CALL(listener, OnCallback(_)).Times(0);

  // triggering domain workspace event (pretending it is disconnected)
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  controller.ProcessEventFromDomain({"abc", new_value, false});

  EXPECT_TRUE(variable_item0->IsAvailable());
  // current implementation: for disconnected status do not update the value
  ASSERT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // triggering domain workspace event (pretending it is connected)
  controller.ProcessEventFromDomain({"abc", new_value, true});

  EXPECT_TRUE(variable_item0->IsAvailable());
  // checking updated value
  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(new_value, stored_anyvalue);
}

//! Setting up the WorkspaceItem with single variable.
//! Triggering domain workspace event twice and validating AnyValueItem update.
//! The difference with the previous test: on first update AnyValueItem should be created,
//! on second update it should be updated (not re-created).

TEST_F(WorkspaceItemControllerTests, ProcessEventFromDomainTwice)
{
  testutils::MockCallbackListener<WorkspaceEvent> listener;

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());

  testutils::SetupVariable("abc", value, *variable_item0);
  // initially VariableItem doesn't have AnyValueItem
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // expecting no callbacks on processing domain events
  EXPECT_CALL(listener, OnCallback(_)).Times(0);

  // triggering domain workspace event
  controller.ProcessEventFromDomain({"abc", value, true});

  auto anyvalue_item = variable_item0->GetAnyValueItem();
  ASSERT_NE(anyvalue_item, nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value, stored_anyvalue);

  // triggering the second domain workspace event
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  controller.ProcessEventFromDomain({"abc", new_value, true});

  auto new_anyvalue_item = variable_item0->GetAnyValueItem();
  ASSERT_EQ(anyvalue_item, new_anyvalue_item);
  auto new_stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(new_value, new_stored_anyvalue);
}

//! Setting up the WorkspaceItem with single variable.
//! Replacing AnyValueItem through the model and expecting callback toward the domain.

TEST_F(WorkspaceItemControllerTests, ModifyAnyValueFromModelViaInsert)
{
  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  auto variable_item0 =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value, *variable_item0);
  sequencergui::SetAnyValue(value, *variable_item0);

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // preparing callback expectations
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  WorkspaceEvent expected_event{"abc", new_value};

  EXPECT_CALL(listener, OnCallback(expected_event)).Times(1);

  // modifying value from the model
  sequencergui::SetAnyValue(expected_event.value, *variable_item0);
}

//! Setting up the workspace with two variables. Replacing variables one after another and checking
//! the order of callbacks.

TEST_F(WorkspaceItemControllerTests, ModifyTwoVariablesViaInserts)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  auto variable_item0 =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("var0", value0, *variable_item0);
  sequencergui::SetAnyValue(value0, *variable_item0);

  auto variable_item1 =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("var1", value1, *variable_item1);
  sequencergui::SetAnyValue(value1, *variable_item1);

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // preparing callback expectations
  WorkspaceEvent expected_event0{"var0", new_value};
  WorkspaceEvent expected_event1{"var1", new_value};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnCallback(expected_event0)).Times(1);
    EXPECT_CALL(listener, OnCallback(expected_event1)).Times(1);
  }

  // modifying value from the model
  sequencergui::SetAnyValue(expected_event0.value, *variable_item0);
  sequencergui::SetAnyValue(expected_event1.value, *variable_item1);
}

//! Setting up the WorkspaceItem with single ChannelAccessVariableItem.
//! Triggering domain workspace event and validating AnyValueItem update.
//! There is no any real SoftIoc behind. The main purpose of the test is to validate
//! that "connected" status is propagated from WorkspaceEvent to the item.

TEST_F(WorkspaceItemControllerTests, ChannelAccessVariableInTheWorkspace)
{
  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item = m_workspace_item->InsertItem<sequencergui::ChannelAccessVariableItem>(
      mvvm::TagIndex::Append());
  variable_item->SetName("abc");

  // initially it doesn't have a value on board and it is disconnected
  EXPECT_EQ(variable_item->GetAnyValueItem(), nullptr);
  EXPECT_FALSE(variable_item->IsAvailable());

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // expecting no callbacks on processing domain events
  EXPECT_CALL(listener, OnCallback(_)).Times(0);

  // triggering domain workspace event, pretending connected status
  controller.ProcessEventFromDomain({"abc", value, true});

  // status become connected, value updated
  EXPECT_TRUE(variable_item->IsAvailable());
  ASSERT_NE(variable_item->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
  EXPECT_EQ(value, stored_anyvalue);
}

//! Setting up the WorkspaceItem with single scalar variable. Setting the value through the model
//! and expecting notification to the domain. The purpose of the test is to validate propagation
//! of DataChangedEvent from the model to the domain.

TEST_F(WorkspaceItemControllerTests, SetScalarData)
{
  const std::string var_name("abc");

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName(var_name);
  sequencergui::SetAnyValue(value, *variable_item);

  auto scalar_anyvalue_item =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(variable_item->GetAnyValueItem());
  ASSERT_NE(scalar_anyvalue_item, nullptr);
  EXPECT_EQ(scalar_anyvalue_item->Data<int>(), 42);

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // preparing callback expectations
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  WorkspaceEvent expected_event{var_name, new_value};

  EXPECT_CALL(listener, OnCallback(expected_event)).Times(1);

  // modifying value from the model
  scalar_anyvalue_item->SetData(43);
}

//! Setting up the WorkspaceItem with struct variable with a scalar on board. Setting scalar value
//! through the model and expecting notification to the domain.

TEST_F(WorkspaceItemControllerTests, SetScalarDataInStruct)
{
  const std::string var_name("abc");

  auto variable_item =
      m_workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName(var_name);

  auto struct_item = variable_item->InsertItem<sup::gui::AnyValueStructItem>({});
  auto scalar_item = struct_item->AddScalarField("value", sup::dto::kInt32TypeName, 42);
  EXPECT_EQ(scalar_item->Data<int>(), 42);

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  WorkspaceItemController controller(m_workspace_item);
  controller.SetCallback(listener.CreateCallback());

  // preparing callback expectations
  sup::dto::AnyValue new_value({{"value", {sup::dto::SignedInteger32Type, 43}}});
  WorkspaceEvent expected_event{var_name, new_value};
  EXPECT_CALL(listener, OnCallback(expected_event)).Times(1);

  // modifying value from the model
  scalar_item->SetData(43);
}
