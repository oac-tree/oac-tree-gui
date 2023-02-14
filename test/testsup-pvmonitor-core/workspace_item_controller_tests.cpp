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
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_event.h>
#include <testutils/gui_domain_utils.h>
#include <testutils/mock_callback_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>

#include <stdexcept>

using namespace suppvmonitor;
using ::testing::_;

//! Tests for WorkspaceItemController class.

class WorkspaceItemControllerTests : public ::testing::Test
{
};

TEST_F(WorkspaceItemControllerTests, InitialState)
{
  MonitorModel model;
  WorkspaceItemController controller(&model);
  EXPECT_EQ(controller.GetWorkspaceItem(), nullptr);
  EXPECT_THROW(controller.GeVariableItemForName(""), std::logic_error);
}

TEST_F(WorkspaceItemControllerTests, GeVariableItemForName)
{
  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName("abc");

  WorkspaceItemController controller(&model);
  EXPECT_EQ(controller.GeVariableItemForName("abc"), variable_item);
}

//! Setting up the WorkspaceItem with single variable.
//! Triggering domain workspace event and validating AnyValueItem update.
//! Expecting no callbacks on item update.

TEST_F(WorkspaceItemControllerTests, ProcessEventFromDomain)
{
  testutils::MockCallbackListener<WorkspaceEvent> listener;

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());

  testutils::SetupVariable("abc", value, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceItemController controller(&model);
  controller.SetCallback(listener.CreateCallback());

  // initially VariableItem doesn't have AnyValueItem
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // expecting no callbacks on processing domain events
  EXPECT_CALL(listener, OnCallback(_)).Times(0);

  // triggering domain workspace event
  controller.ProcessEventFromDomain({"abc", value});

  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value, stored_anyvalue);
}

//! Setting up the WorkspaceItem with single variable.
//! Replacing AnyValueItem through the model and expecting callback toward the domain.

TEST_F(WorkspaceItemControllerTests, ModifyAnyValueFromModelViaInsert)
{
  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value, *variable_item0);
  sequencergui::UpdateAnyValue(value, *variable_item0);

  WorkspaceItemController controller(&model);
  controller.SetCallback(listener.CreateCallback());

  // preparing callback expectations
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  WorkspaceEvent expected_event{"abc", new_value};

  EXPECT_CALL(listener, OnCallback(expected_event)).Times(1);

  // modifying value from the model
  sequencergui::UpdateAnyValue(expected_event.m_value, *variable_item0);
}

//! Setting up the workspace with two variables. Replacing variables one after another and checking
//! the order of callbacks.

TEST_F(WorkspaceItemControllerTests, ModifyTwoVariablesViaInserts)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});

  testutils::MockCallbackListener<WorkspaceEvent> listener;

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("var0", value0, *variable_item0);
  sequencergui::UpdateAnyValue(value0, *variable_item0);

  auto variable_item1 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("var1", value1, *variable_item1);
  sequencergui::UpdateAnyValue(value1, *variable_item1);

  WorkspaceItemController controller(&model);
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
  sequencergui::UpdateAnyValue(expected_event0.m_value, *variable_item0);
  sequencergui::UpdateAnyValue(expected_event1.m_value, *variable_item1);
}
