/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_event.h>

#include <stdexcept>

using namespace suppvmonitor;

//! Tests for WorkspaceItemController class.

class WorkspaceItemControllerTests : public ::testing::Test
{
public:
  static void SetupVariable(const std::string& name, const sup::dto::AnyValue& initial_value,
                            sequencergui::LocalVariableItem& item)
  {
    item.SetName(name);
    item.SetJsonType(sup::gui::GetAnyTypeToJSONString(&initial_value));
    item.SetJsonValue(sup::gui::GetValuesToJSONString(&initial_value));
  }
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

//! Setting up the workspace with single variable.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceItemControllerTests, OnVariableUpdated)
{
  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());

  SetupVariable("abc", value, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceItemController controller(&model);

  // initially VariableItem doesn't have AnyValueItem
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // changing the value via domain workspace
  controller.ProcessDomainEvent({"abc", value});

  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value, stored_anyvalue);
}
