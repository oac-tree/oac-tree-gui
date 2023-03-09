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

#include "suppvmonitor/monitor_widget_actions.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <suppvmonitor/monitor_model.h>

#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>

using namespace suppvmonitor;
using ::testing::_;

class MonitorWidgetActionsTest : public ::testing::Test
{
public:
  MonitorWidgetActionsTest() { m_model.InsertItem<sequencergui::WorkspaceItem>(); }

  //! Creates context necessary for AnyValueEditActions to function.
  MonitorWidgetContext CreateContext(sequencergui::VariableItem* item)
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [item]() { return item; };
    return {get_selected_callback, m_warning_listener.CreateCallback()};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<MonitorWidgetActions> CreateActions(sequencergui::VariableItem* selection)
  {
    return std::make_unique<MonitorWidgetActions>(CreateContext(selection), &m_model, nullptr);
  }

  MonitorModel m_model;
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
};

TEST_F(MonitorWidgetActionsTest, InitialState)
{
  EXPECT_TRUE(m_model.GetWorkspaceItem()->GetVariables().empty());
}

//! Adding variables to an empty model.

TEST_F(MonitorWidgetActionsTest, OnAddVariableRequestToEmptyModel)
{
  // pretending that nothing is selected
  auto actions = CreateActions(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  actions->OnAddVariableRequest(QString::fromStdString(sequencergui::LocalVariableItem::Type));

  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariables().size(), 1);
  auto inserted_variable = dynamic_cast<sequencergui::LocalVariableItem*>(
      m_model.GetWorkspaceItem()->GetVariables().at(0));
  ASSERT_NE(inserted_variable, nullptr);
}
