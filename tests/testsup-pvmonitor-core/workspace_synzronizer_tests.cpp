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

#include "suppvmonitor/workspace_syncronizer.h"

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <testutils/gui_domain_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>

#include <QTest>

using namespace suppvmonitor;

//! Tests for WorkspaceSyncronizer class.

class WorkspaceSyncronizerTests : public ::testing::Test
{
public:
  //! Helper function to create LocalVariableItem with given name and initial AnyValue.
  static std::unique_ptr<sequencergui::LocalVariableItem> CreateLocalVariable(
      const std::string& name, const sup::dto::AnyValue& initial_value)
  {
    auto result = std::make_unique<sequencergui::LocalVariableItem>();
    testutils::SetupVariable(name, initial_value, *result.get());
    return result;
  }
};

TEST_F(WorkspaceSyncronizerTests, InitialState)
{
  MonitorModel model;
  WorkspaceSyncronizer syncronizer(&model);
  EXPECT_EQ(syncronizer.GetWorkspace(), nullptr);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Setting up the workspace and checking that proper domain variable has been created.

TEST_F(WorkspaceSyncronizerTests, OnSetupWorkspaceRequest)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem(CreateLocalVariable("abc", value0), mvvm::TagIndex::Append());

  WorkspaceSyncronizer syncronizer(&model);
  syncronizer.OnSetupWorkspaceRequest();

  ASSERT_TRUE(syncronizer.GetWorkspace() != nullptr);
  EXPECT_EQ(syncronizer.GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(syncronizer.GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = syncronizer.GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");

  // In current implementation of SetupRequest we do nothing with initial values of AnyValue.
  // Let's test this by checking an emptiness of AnyValue, we will return to it later.
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);
}

//! Setting up the workspace with single variable.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceSyncronizerTests, OnDomainVariableUpdated)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  WorkspaceSyncronizer syncronizer(&model);
  syncronizer.OnSetupWorkspaceRequest();

  // FIXME current implementation doesn't update AnyValueItem on first connection
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // changing the value via domain workspace
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(syncronizer.GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value1, stored_anyvalue);
}

TEST_F(WorkspaceSyncronizerTests, OnModelVariableUpdate)
{
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  MonitorModel model;
  auto workspace_item = model.InsertItem<sequencergui::WorkspaceItem>();
  auto variable_item0 =
      workspace_item->InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);
  sequencergui::UpdateAnyValue(value0, *variable_item0);

  WorkspaceSyncronizer syncronizer(&model);
  syncronizer.OnSetupWorkspaceRequest();

  // changing the value via the model
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  sequencergui::UpdateAnyValue(value1, *variable_item0);

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  auto domain_variable0 = syncronizer.GetWorkspace()->GetVariable("abc");
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value1);
}
