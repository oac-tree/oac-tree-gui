/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/composer/workspace_editor_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/monitor_model.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/exceptions.h>

#include <gtest/gtest.h>
#include <testutils/mock_workspace_editor_context.h>
#include <testutils/sequencer_test_utils.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Test WorkspaceEditorActionHandler for basic editing scenarios.
 */
class WorkspaceEditorActionHandlerTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerTest() { m_model.InsertItem<WorkspaceItem>(); }

  WorkspaceItem* GetWorkspaceItem() { return m_model.GetWorkspaceItem(); }

  /**
   * @brief Creates action handler.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(
      const std::vector<mvvm::SessionItem*>& selection)
  {
    return m_mock_context.CreateActionHandler(m_model.GetWorkspaceItem(), selection);
  }

  MonitorModel m_model;
  test::MockWorkspaceEditorContext m_mock_context;
};

//! Checking exceptions depending on how many callbacks left undefined.
TEST_F(WorkspaceEditorActionHandlerTest, AttemptToCreateWhenNoContextIsInitialised)
{
  EXPECT_TRUE(m_model.GetWorkspaceItem()->GetVariables().empty());

  {
    const WorkspaceEditorContext context{};
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    context.selected_items_callback = []() -> std::vector<mvvm::SessionItem*> { return {}; };
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    context.selected_items_callback = []() -> std::vector<mvvm::SessionItem*> { return {}; };
    context.notify_request = [](auto item) {};
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }
}

TEST_F(WorkspaceEditorActionHandlerTest, AttemptToAddVariableWhenWorkspaceIsAbsent)
{
  auto handler = m_mock_context.CreateActionHandler(nullptr, {});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // adding variable
  EXPECT_NO_THROW(handler->AddVariable(domainconstants::kLocalVariableType));
}

TEST_F(WorkspaceEditorActionHandlerTest, SelectedVariables)
{
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  auto var1 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  {
    auto handler = CreateActionHandler({var0, var1});
    EXPECT_EQ(handler->GetSelectedVariable(), var0);
    EXPECT_EQ(handler->GetSelectedVariables(), std::vector<VariableItem*>({var0, var1}));
  }

  {  // part of var0 is selected, and var1
    auto handler = CreateActionHandler({initial_anyvalue_item, var1});
    EXPECT_EQ(handler->GetSelectedVariable(), var1);
    EXPECT_EQ(handler->GetSelectedVariables(), std::vector<VariableItem*>({var1}));
  }
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestToEmptyModel)
{
  // pretending that nothing is selected
  auto handler = CreateActionHandler({});

  const mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // adding variable
  handler->AddVariable(domainconstants::kLocalVariableType);

  // validating default values of just inserted variable
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
  auto& inserted_variable0 = GetWorkspaceItem()->GetItem<VariableItem>(mvvm::TagIndex::Default(0));
  EXPECT_EQ(inserted_variable0.GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(inserted_variable0.GetName(), std::string("var0"));

  // just inserted variable was selected
  EXPECT_EQ(reported_item, &inserted_variable0);

  // it has scalar AnyValueItem on board by default
  auto anyvalue_item =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(inserted_variable0.GetAnyValueItem());
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);
  EXPECT_EQ(anyvalue_item->Data<int>(), 0);

  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_));

  // adding another variable
  handler->AddVariable(domainconstants::kLocalVariableType);

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 2);
  auto& inserted_variable1 = GetWorkspaceItem()->GetItem<VariableItem>(mvvm::TagIndex::Default(1));
  EXPECT_EQ(inserted_variable1.GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(inserted_variable1.GetName(), std::string("var1"));

  // attempty to add unknown variable type
  EXPECT_THROW(handler->AddVariable("non-existing-type"), sup::oac_tree::InvalidOperationException);
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableWhenNothingIsSelected)
{
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  auto var1 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());

  // pretending that var0 is selected
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_));

  EXPECT_FALSE(handler->CanRemoveVariable());

  // adding variable
  handler->AddVariable(domainconstants::kFileVariableType);

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 3);
  auto& inserted_variable0 = GetWorkspaceItem()->GetItem<VariableItem>(mvvm::TagIndex::Default(2));
  EXPECT_EQ(inserted_variable0.GetDomainType(), domainconstants::kFileVariableType);
  EXPECT_EQ(inserted_variable0.GetName(), std::string("var2"));
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestBetween)
{
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  auto var1 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());

  // pretending that var0 is selected
  auto handler = CreateActionHandler({var0});

  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_));

  // adding variable
  handler->AddVariable(domainconstants::kFileVariableType);

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 3);
  auto& inserted_variable0 = GetWorkspaceItem()->GetItem<VariableItem>(mvvm::TagIndex::Default(1));
  EXPECT_EQ(inserted_variable0.GetDomainType(), domainconstants::kFileVariableType);
  EXPECT_EQ(inserted_variable0.GetName(), std::string("var2"));
}

TEST_F(WorkspaceEditorActionHandlerTest, OnRemoveVariableRequest)
{
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // pretending that var0 is selected
  auto handler = CreateActionHandler({var0});

  // removing variable
  EXPECT_TRUE(handler->CanRemoveVariable());
  handler->RemoveVariable();

  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAttemptToRemoveVariable)
{
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // nothing is selected
  auto handler = CreateActionHandler({});

  // removing variable
  handler->RemoveVariable();

  // still same amount of variables
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
}

//! Attempt to edit AnyValueItem when nothing is selected.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenNothingIsSelected)
{
  // nothing is selected
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  handler->EditAnyValue();
}

//! Full scenario: editing AnyValueItem on board of a Local variable. Initially we have
//! VariableItem selected.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenVariableIsSelected)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler({var0});

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Variable initially doesn't have any value defined.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenVariableIsSelectedAndAnyValueIsAbsent)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  const sup::gui::AnyValueItem* initial_anyvalue_item{nullptr};

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler({var0});

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Full scenario: editing AnyValueItem on board of a Local variable.
//! The only difference with previous test is that we mimick selection of AnyValueItem instead
//! of VariableItem.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueIsSelected)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler({initial_anyvalue_item});

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Full scenario: editing AnyValueItem on board of a Local variable. Pretending that the user has
//! removed initial AnyValueItem and has pushed OK button. The variable gets an empty AnyValueItem,
//! i.e. not set.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueItemIsRemoved)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // Item mimicking editing result. Unique ptr is empty, that means that the user has deleted old
  // AnyValue
  std::unique_ptr<sup::gui::AnyValueItem> editing_result;

  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};
  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  auto handler = CreateActionHandler({initial_anyvalue_item});

  // expecting no warning callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that previous AnyValueItem has been replaced by an empty one
  auto anyvalue_item = var0->GetAnyValueItem();
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_NE(anyvalue_item, initial_anyvalue_item);
  EXPECT_EQ(anyvalue_item->GetType(), mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());
  EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
  EXPECT_TRUE(sup::dto::IsEmptyValue(GetAnyValue(*var0)));
}

//! Variable without AnyValueItem, the dialog returns no item: the variable gets an empty one.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueIsAbsentAndNotCreated)
{
  const bool dialog_was_acccepted = true;

  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  EXPECT_EQ(var0->GetAnyValueItem(), nullptr);

  std::unique_ptr<sup::gui::AnyValueItem> editing_result;
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};
  ON_CALL(m_mock_context, OnEditAnyvalue(nullptr))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  auto handler = CreateActionHandler({var0});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(nullptr)).Times(1);

  handler->EditAnyValue();

  auto anyvalue_item = var0->GetAnyValueItem();
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetType(), mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());
}

//! Full scenario: editing AnyValueItem on board of a Local variable. Pretending that the user has
//! modified initial AnyValueItem, but pushed CANCEL button. Old AnyValueItem should remain intact.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenDialogCanceled)
{
  const bool dialog_was_acccepted = false;  // cancel button was pushed

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};
  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler({initial_anyvalue_item});

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that previous item remained the same
  EXPECT_EQ(var0->GetAnyValueItem(), initial_anyvalue_item);
}

//! Full scenario: editing AnyValueItem on board of a Local variable that doesn't have any
//! AnyValueItem yet.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWheNoAnyValueItemIsStilExists)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = InsertVariable(domainconstants::kLocalVariableType, GetWorkspaceItem());
  EXPECT_EQ(var0->GetAnyValueItem(), nullptr);

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(nullptr))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler({var0});

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(::testing::_)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Adding SystemClock variable to the model. It has no dedicated GUI variable class. We check that
//! OnAdVariableRequest correctly creates such variables (real-life bug).
TEST_F(WorkspaceEditorActionHandlerTest, OnAddSystemClockVariable)
{
  if (!IsVariableTypeAvailable(domainconstants::kSystemClockVariableType))
  {
    GTEST_SKIP();
  }

  // pretending that nothing is selected
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_));

  // adding variable
  handler->AddVariable(domainconstants::kSystemClockVariableType);

  // validating default values of just inserted variable
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
  auto& inserted_variable0 = GetWorkspaceItem()->GetItem<VariableItem>(mvvm::TagIndex::Default(0));
  EXPECT_EQ(inserted_variable0.GetName(), std::string("var0"));
  EXPECT_EQ(inserted_variable0.GetDomainType(), domainconstants::kSystemClockVariableType);
}

}  // namespace oac_tree_gui::test
