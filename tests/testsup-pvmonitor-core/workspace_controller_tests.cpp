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

#include "suppvmonitor/workspace_controller.h"

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>

#include <QTest>

using namespace suppvmonitor;

//! Tests for WorkspaceController class.

class WorkspaceControllerTest : public ::testing::Test
{
public:
  //! Helper function to create LocalVariableItem with given name and initial AnyValue.
  static std::unique_ptr<sequencergui::LocalVariableItem> CreateLocalVariable(
      const std::string& name, const sup::dto::AnyValue& initial_value)
  {
    auto result = std::make_unique<sequencergui::LocalVariableItem>();
    SetupVariable(name, initial_value, *result.get());
    return result;
  }

  static void SetupVariable(const std::string& name, const sup::dto::AnyValue& initial_value,
                            sequencergui::LocalVariableItem& item)
  {
    item.SetName(name);
    item.SetJsonType(sup::gui::GetAnyTypeToJSONString(&initial_value));
    item.SetJsonValue(sup::gui::GetValuesToJSONString(&initial_value));
  }
};

TEST_F(WorkspaceControllerTest, InitialState)
{
  MonitorModel model;
  WorkspaceController controller(&model);
  EXPECT_EQ(controller.GetWorkspace(), nullptr);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Setting up the workspace and checking that proper domain variable has been created.

TEST_F(WorkspaceControllerTest, OnSetupWorkspaceRequest)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem(CreateLocalVariable("abc", value0), mvvm::TagIndex::Append());

  WorkspaceController controller(&model);
  controller.OnSetupWorkspaceRequest();

  ASSERT_TRUE(controller.GetWorkspace() != nullptr);
  EXPECT_EQ(controller.GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(controller.GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = controller.GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);
}

//! Setting up the workspace with single variable.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceControllerTest, OnVariableUpdated)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceController controller(&model);
  controller.OnSetupWorkspaceRequest();

  // FIXME current implementation doesn't update AnyValueItem on first connection
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // changing the value via domain workspace
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(controller.GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value1, stored_anyvalue);
}
