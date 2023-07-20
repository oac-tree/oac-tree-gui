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

#include "sequencergui/pvmonitor/workspace_editor_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/transform/transform_helpers.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>

using namespace sequencergui;
using ::testing::_;

class WorkspaceEditorActionHandlerTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerTest() { m_model.InsertItem<WorkspaceItem>(); }

  //! Mock dialog that pretend to take an item for editing and return the result to the user.
  class MockDialog
  {
  public:
    void SetItemToReturn(AnyValueDialogResult dialog_result)
    {
      m_dialog_result = std::move(dialog_result);
    }

    MOCK_METHOD(void, OnEditingRequest, (const sup::gui::AnyValueItem* item));

    //! Creates a callback that mimicks editing request and returning the result to the user
    std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> CreateCallback()
    {
      return [this](const sup::gui::AnyValueItem* item) -> AnyValueDialogResult
      {
        OnEditingRequest(item);
        return std::move(m_dialog_result);
      };
    }

    AnyValueDialogResult m_dialog_result;
  };

  //! Creates context necessary for AnyValueEditActions to function.
  WorkspaceEditorContext CreateContext(mvvm::SessionItem* selected_item,
                                       AnyValueDialogResult dialog_result = {})
  {
    // callback returns given item, pretending it is user's selection
    auto selected_item_callback = [selected_item]() { return selected_item; };

    m_mock_dialog.SetItemToReturn(std::move(dialog_result));

    auto selected_workspace_callback = [this]() { return m_model.GetWorkspaceItem(); };

    auto send_message_callback = m_warning_listener.CreateCallback();
    auto edit_anyvalue_callback = m_mock_dialog.CreateCallback();

    return {selected_workspace_callback, selected_item_callback, send_message_callback,
            edit_anyvalue_callback};
  }

  //! Creates action handler for testing.
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(
      mvvm::SessionItem* selection, AnyValueDialogResult dialog_result = {})
  {
    return std::make_unique<WorkspaceEditorActionHandler>(
        CreateContext(selection, std::move(dialog_result)), nullptr);
  }

  WorkspaceItem* GetWorkspaceItem() { return m_model.GetWorkspaceItem(); }

  MonitorModel m_model;
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  MockDialog m_mock_dialog;
};

TEST_F(WorkspaceEditorActionHandlerTest, InitialState)
{
  EXPECT_TRUE(m_model.GetWorkspaceItem()->GetVariables().empty());
  EXPECT_THROW(WorkspaceEditorActionHandler({}, nullptr), RuntimeException);
}

//! Attempt to add variable into non-existing workspace.

TEST_F(WorkspaceEditorActionHandlerTest, AttemptToAddVariableWhenWorkspaceIsAbsent)
{
  auto selected_workspace_callback = []() { return nullptr; };
  auto selected_item_callback = []() { return nullptr; };
  auto send_message_callback = m_warning_listener.CreateCallback();
  auto edit_anyvalue_callback = m_mock_dialog.CreateCallback();

  WorkspaceEditorContext context{selected_workspace_callback, selected_item_callback,
                                 send_message_callback, edit_anyvalue_callback};
  WorkspaceEditorActionHandler actions(context);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding variable
  EXPECT_NO_THROW(actions.OnAddVariableRequest(QString::fromStdString(LocalVariableItem::Type)));
}

//! Adding variables to an empty model.

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestToEmptyModel)
{
  // pretending that nothing is selected
  auto actions = CreateActionHandler(nullptr);

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

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableWHenNothingIsSelected)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());

  // pretending that var0 is selected
  auto actions = CreateActionHandler(nullptr);

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

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestBetween)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());

  // pretending that var0 is selected
  auto actions = CreateActionHandler(var0);

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

TEST_F(WorkspaceEditorActionHandlerTest, OnRemoveVariableRequest)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);

  // pretending that var0 is selected
  auto actions = CreateActionHandler(var0);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // removing variable
  actions->OnRemoveVariableRequest();

  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 0);
}

//! Attempt to remove variable when nothing is selected.

TEST_F(WorkspaceEditorActionHandlerTest, OnAttemptToRemoveVariable)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);

  // nothing is selected
  auto actions = CreateActionHandler(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // removing variable
  actions->OnRemoveVariableRequest();

  // still same amount of variables
  EXPECT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);
}

//! Attempt to edit variable when nothing is selected.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenNothingIsSelected)
{
  // pretending that var0 is selected
  auto actions = CreateActionHandler(nullptr);

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // removing variabl
  actions->OnEditAnyvalueRequest();
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem.
//! Initially we have VariableItem selected.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenVariableIsSelected)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions = CreateActionHandler(var0, {dialog_was_acccepted, std::move(editing_result)});

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

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueIsSelected)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions =
      CreateActionHandler(initial_anyvalue_item, {dialog_was_acccepted, std::move(editing_result)});

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem. Pretending that the user has
//! removed initial AnyValueItem and has pushed OK button. That should lead to the dissapearance of
//! AnyValueItem child on board of LocalVariableItem.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueItemIsRemoved)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // Item mimicking editing result. Unique ptr is empty, that means that the user has deleted old
  // AnyValue
  std::unique_ptr<sup::gui::AnyValueItem> editing_result;

  // preparing actions
  auto actions =
      CreateActionHandler(initial_anyvalue_item, {dialog_was_acccepted, std::move(editing_result)});

  // expecting no warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that previous AnyValueItem has been removed
  EXPECT_EQ(var0->GetAnyValueItem(), nullptr);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem. Pretending that the user has
//! modified initial AnyValueItem, but pushed CANCEL button. Old AnyValueItem should remain intact.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenDialogCanceled)
{
  const bool dialog_was_acccepted = false;  // cancel button was pushed

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions =
      CreateActionHandler(initial_anyvalue_item, {dialog_was_acccepted, std::move(editing_result)});

  // expecting no waning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that previous item remained the same
  EXPECT_EQ(var0->GetAnyValueItem(), initial_anyvalue_item);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem that doesn't have any
//! AnyValueItem yet.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWheNoAnyValueItemIsStilExists)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  EXPECT_EQ(var0->GetAnyValueItem(), nullptr);

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();

  // preparing actions
  auto actions = CreateActionHandler(var0, {dialog_was_acccepted, std::move(editing_result)});

  // expecting no warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_dialog, OnEditingRequest(_)).Times(1);

  // editing request
  actions->OnEditAnyvalueRequest();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}
