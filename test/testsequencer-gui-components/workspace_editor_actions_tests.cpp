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

#include "sequencergui/pvmonitor/workspace_editor_actions.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>

using namespace sequencergui;
using ::testing::_;

class WorkspaceEditorActionsTest : public ::testing::Test
{
public:
  WorkspaceEditorActionsTest() { m_model.InsertItem<WorkspaceItem>(); }

  //! Mock dialog that pretend to take an item for editing and return the result to the user.
  class MockDialog
  {
  public:
    void SetItemToReturn(std::unique_ptr<sup::gui::AnyValueItem> item_to_return)
    {
      m_item_to_return = std::move(item_to_return);
    }

    MOCK_METHOD(void, OnEditingRequest, (const sup::gui::AnyValueItem& item));

    //! Creates a callback that mimicks editing request and returning the result to the user
    std::function<std::unique_ptr<sup::gui::AnyValueItem>(const sup::gui::AnyValueItem&)>
    CreateCallback()
    {
      return [this](const sup::gui::AnyValueItem& item) -> std::unique_ptr<sup::gui::AnyValueItem>
      {
        OnEditingRequest(item);
        return std::move(m_item_to_return);
      };
    }

    std::unique_ptr<sup::gui::AnyValueItem> m_item_to_return;
  };

  //! Creates context necessary for AnyValueEditActions to function.
  WorkspaceEditorContext CreateContext(mvvm::SessionItem* selected_item,
                                       std::unique_ptr<sup::gui::AnyValueItem> item_to_return = {})
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [selected_item]() { return selected_item; };

    m_mock_dialog.SetItemToReturn(std::move(item_to_return));

    auto get_workspace_callback = [this]() { return m_model.GetWorkspaceItem(); };

    return {get_selected_callback, m_warning_listener.CreateCallback(),
            m_mock_dialog.CreateCallback(), get_workspace_callback};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<WorkspaceEditorActions> CreateActions(
      mvvm::SessionItem* selection, std::unique_ptr<sup::gui::AnyValueItem> item_to_return = {})
  {
    return std::make_unique<WorkspaceEditorActions>(
        CreateContext(selection, std::move(item_to_return)), nullptr);
  }

  WorkspaceItem* GetWorkspaceItem() { return m_model.GetWorkspaceItem(); }

  MonitorModel m_model;
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  MockDialog m_mock_dialog;
};

TEST_F(WorkspaceEditorActionsTest, InitialState)
{
  EXPECT_TRUE(m_model.GetWorkspaceItem()->GetVariables().empty());
  EXPECT_THROW(WorkspaceEditorActions({}, nullptr), RuntimeException);
}

//! Adding variables to an empty model.

TEST_F(WorkspaceEditorActionsTest, OnAddVariableRequestToEmptyModel)
{
  // pretending that nothing is selected
  auto actions = CreateActions(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding variable
  actions->OnAddVariableRequest(QString::fromStdString(LocalVariableItem::Type));

  // validating default values of just inserted variable
  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);
  auto inserted_variable0 =
      dynamic_cast<LocalVariableItem*>(m_model.GetWorkspaceItem()->GetVariables().at(0));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var0"));

  // it has scalar AnyValueItem on board by default
  auto anyvalue_item =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(inserted_variable0->GetAnyValueItem());
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), std::string("value"));
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);
  EXPECT_EQ(anyvalue_item->Data<int>(), 0);

  // adding another variable
  actions->OnAddVariableRequest(QString::fromStdString(LocalVariableItem::Type));

  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 2);
  auto inserted_variable1 =
      dynamic_cast<LocalVariableItem*>(m_model.GetWorkspaceItem()->GetVariables().at(1));
  ASSERT_NE(inserted_variable1, nullptr);
  EXPECT_EQ(inserted_variable1->GetName(), std::string("var1"));

  // expecting warning below
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempty to add unknown variable type
  actions->OnAddVariableRequest("non-existing-type");
}

//! Inserting variable between two existing variables.

TEST_F(WorkspaceEditorActionsTest, OnAddVariableWHenNothingIsSelected)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());

  // pretending that var0 is selected
  auto actions = CreateActions(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding variable
  actions->OnAddVariableRequest(QString::fromStdString(FileVariableItem::Type));

  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 3);
  auto inserted_variable0 =
      dynamic_cast<FileVariableItem*>(m_model.GetWorkspaceItem()->GetVariables().at(2));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var2"));
}

//! Inserting variable between two existing variables.

TEST_F(WorkspaceEditorActionsTest, OnAddVariableRequestBetween)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());

  // pretending that var0 is selected
  auto actions = CreateActions(var0);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding variable
  actions->OnAddVariableRequest(QString::fromStdString(FileVariableItem::Type));

  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 3);
  auto inserted_variable0 =
      dynamic_cast<FileVariableItem*>(m_model.GetWorkspaceItem()->GetVariables().at(1));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var2"));
}

//! Removing variable.

TEST_F(WorkspaceEditorActionsTest, OnRemoveVariableRequest)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);

  // pretending that var0 is selected
  auto actions = CreateActions(var0);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // removing variable
  actions->OnRemoveVariableRequest();

  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 0);
}

//! Attempt to remove variable when nothing is selected.

TEST_F(WorkspaceEditorActionsTest, OnAttemptToRemoveVariable)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);

  // nothing is selected
  auto actions = CreateActions(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // removing variable
  actions->OnRemoveVariableRequest();

  // still same amount of variables
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);
}

//! Attempt to edit variable when nothing is selected.

TEST_F(WorkspaceEditorActionsTest, OnEditRequestWhenNothingIsSelected)
{
  // pretending that var0 is selected
  auto actions = CreateActions(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // removing variabl
  actions->OnEditAnyvalueRequest();
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem.
//! Initially we have VariableItem selected.

TEST_F(WorkspaceEditorActionsTest, OnEditRequestWhenVariableIsSelected)
{
  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions = CreateActions(var0, std::move(editing_result));

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem.
//! The only difference with previous test is that we mimick selection of AnyValueItem instead
//! of VariableItem.

TEST_F(WorkspaceEditorActionsTest, OnEditRequestWhenAnyValueIsSelected)
{
  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions = CreateActions(initial_anyvalue_item, std::move(editing_result));

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}
