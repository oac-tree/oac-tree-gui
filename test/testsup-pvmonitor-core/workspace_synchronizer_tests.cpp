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
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/workspace_monitor_helper.h>
#include <testutils/gui_domain_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>

#include <QTest>

using namespace suppvmonitor;

//! Tests for WorkspaceSyncronizer class.

class WorkspaceSynchronizerTests : public ::testing::Test
{
public:
  WorkspaceSynchronizerTests() { m_model.InsertItem<sequencergui::WorkspaceItem>(); }

  //! Helper function to create LocalVariableItem with given name and initial AnyValue.
  static std::unique_ptr<sequencergui::LocalVariableItem> CreateLocalVariable(
      const std::string& name, const sup::dto::AnyValue& initial_value)
  {
    auto result = std::make_unique<sequencergui::LocalVariableItem>();
    testutils::SetupVariable(name, initial_value, *result.get());
    return result;
  }

  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    m_workspace.Setup(); // current convention: has to be setup before starting listening
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

TEST_F(WorkspaceSynchronizerTests, InitialState)
{
  auto synchronizer = CreateSynchronizer();
  EXPECT_EQ(synchronizer->GetWorkspace(), &m_workspace);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Setting up the workspace and checking that proper domain variable has been created.

TEST_F(WorkspaceSynchronizerTests, Start)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 = m_model.GetWorkspaceItem()->InsertItem(CreateLocalVariable("abc", value0),
                                                               mvvm::TagIndex::Append());

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  ASSERT_TRUE(synchronizer->GetWorkspace() != nullptr);
  EXPECT_EQ(synchronizer->GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(synchronizer->GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");

  // In current implementation of SetupRequest we do nothing with initial values of AnyValue.
  // Let's test this by checking an emptiness of AnyValue, we will return to it later.
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);
}

//! Setting up the workspace with single variable.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceSynchronizerTests, OnDomainVariableUpdated)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 = m_model.GetWorkspaceItem()->InsertItem<sequencergui::LocalVariableItem>(
      mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // FIXME current implementation doesn't update AnyValueItem on first connection
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  // changing the value via domain workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  ASSERT_NE(variable_item0->GetAnyValueItem(), nullptr);
  auto stored_anyvalue = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value1, stored_anyvalue);
}

TEST_F(WorkspaceSynchronizerTests, OnModelVariableUpdate)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 = m_model.GetWorkspaceItem()->InsertItem<sequencergui::LocalVariableItem>(
      mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);
  sequencergui::SetAnyValue(value0, *variable_item0);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // changing the value via the model
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  sequencergui::SetAnyValue(value1, *variable_item0);

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value1);
}
