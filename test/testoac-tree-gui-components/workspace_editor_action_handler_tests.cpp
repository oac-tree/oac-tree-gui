/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/composer/workspace_editor_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/monitor_model.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/exceptions.h>

#include <gtest/gtest.h>
#include <testutils/mock_workspace_editor_context.h>

#include <QMimeData>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Test WorkspaceEditorActionHandler for basic editing scenarios.
 */
class WorkspaceEditorActionHandlerTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerTest() { m_model.InsertItem<WorkspaceItem>(); }

  WorkspaceItem* GetWorkspaceItem() const { return m_model.GetWorkspaceItem(); }

  /**
   * @brief Creates action handler.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(mvvm::SessionItem* selection)
  {
    return m_mock_context.CreateActionHandler(m_model.GetWorkspaceItem(), selection);
  }

  MonitorModel m_model;
  test::MockWorkspaceEditorContext m_mock_context;
};

TEST_F(WorkspaceEditorActionHandlerTest, InitialState)
{
  EXPECT_TRUE(m_model.GetWorkspaceItem()->GetVariables().empty());

  {  // no callbacks defined
    const WorkspaceEditorContext context{};
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {  // item callback undefined
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {  // select_notify callback is undefined
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    context.selected_item_callback = []() -> mvvm::SessionItem* { return nullptr; };
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }

  {  // send_message callback is undefined
    WorkspaceEditorContext context;
    context.selected_workspace = []() -> WorkspaceItem* { return nullptr; };
    context.selected_item_callback = []() -> mvvm::SessionItem* { return nullptr; };
    context.select_notify = [](auto item) {};
    EXPECT_THROW(WorkspaceEditorActionHandler{context}, RuntimeException);
  }
}

TEST_F(WorkspaceEditorActionHandlerTest, AttemptToAddVariableWhenWorkspaceIsAbsent)
{
  auto handler = m_mock_context.CreateActionHandler(nullptr, nullptr);

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // adding variable
  EXPECT_NO_THROW(handler->AddVariable(LocalVariableItem::GetStaticType()));
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestToEmptyModel)
{
  // pretending that nothing is selected
  auto handler = CreateActionHandler(nullptr);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  // adding variable
  handler->AddVariable(LocalVariableItem::GetStaticType());

  // validating default values of just inserted variable
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
  auto inserted_variable0 =
      dynamic_cast<LocalVariableItem*>(GetWorkspaceItem()->GetVariables().at(0));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var0"));

  // just inserted variable was selected
  EXPECT_EQ(reported_item, inserted_variable0);

  // it has scalar AnyValueItem on board by default
  auto anyvalue_item =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(inserted_variable0->GetAnyValueItem());
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);
  EXPECT_EQ(anyvalue_item->Data<int>(), 0);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // adding another variable
  handler->AddVariable(LocalVariableItem::GetStaticType());

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 2);
  auto inserted_variable1 =
      dynamic_cast<LocalVariableItem*>(GetWorkspaceItem()->GetVariables().at(1));
  ASSERT_NE(inserted_variable1, nullptr);
  EXPECT_EQ(inserted_variable1->GetName(), std::string("var1"));

  // attempty to add unknown variable type
  EXPECT_THROW(handler->AddVariable("non-existing-type"), sup::oac_tree::InvalidOperationException);
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableWhenNothingIsSelected)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());

  // pretending that var0 is selected
  auto handler = CreateActionHandler(nullptr);

  EXPECT_FALSE(handler->CanRemoveVariable());

  // adding variable
  handler->AddVariable(FileVariableItem::GetStaticType());

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 3);
  auto inserted_variable0 =
      dynamic_cast<FileVariableItem*>(GetWorkspaceItem()->GetVariables().at(2));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var2"));
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAddVariableRequestBetween)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());

  // pretending that var0 is selected
  auto handler = CreateActionHandler(var0);

  // adding variable
  handler->AddVariable(FileVariableItem::GetStaticType());

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 3);
  auto inserted_variable0 =
      dynamic_cast<FileVariableItem*>(GetWorkspaceItem()->GetVariables().at(1));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var2"));
}

TEST_F(WorkspaceEditorActionHandlerTest, OnRemoveVariableRequest)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // pretending that var0 is selected
  auto handler = CreateActionHandler(var0);

  // removing variable
  EXPECT_TRUE(handler->CanRemoveVariable());
  handler->RemoveVariable();

  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);
}

TEST_F(WorkspaceEditorActionHandlerTest, OnAttemptToRemoveVariable)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // nothing is selected
  auto handler = CreateActionHandler(nullptr);

  // removing variable
  handler->RemoveVariable();

  // still same amount of variables
  EXPECT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
}

//! Attempt to edit AnyValueItem when nothing is selected.

TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenNothingIsSelected)
{
  // pretending that var0 is selected
  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // removing variable
  handler->EditAnyValue();
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem. Initially we have
//! VariableItem selected.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenVariableIsSelected)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler(var0);

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

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
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler(initial_anyvalue_item);

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem. Pretending that the user has
//! removed initial AnyValueItem and has pushed OK button. That should trigger the warning.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenAnyValueItemIsRemoved)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // Item mimicking editing result. Unique ptr is empty, that means that the user has deleted old
  // AnyValue
  std::unique_ptr<sup::gui::AnyValueItem> editing_result;

  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};
  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  auto handler = CreateActionHandler(initial_anyvalue_item);

  // expecting no warning callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);
  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that previous AnyValueItem has been removed
  EXPECT_EQ(var0->GetAnyValueItem(), initial_anyvalue_item);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem. Pretending that the user has
//! modified initial AnyValueItem, but pushed CANCEL button. Old AnyValueItem should remain intact.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWhenDialogCanceled)
{
  const bool dialog_was_acccepted = false;  // cancel button was pushed

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);
  auto initial_anyvalue_item = var0->GetAnyValueItem();

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};
  ON_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler(initial_anyvalue_item);

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(initial_anyvalue_item)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that previous item remained the same
  EXPECT_EQ(var0->GetAnyValueItem(), initial_anyvalue_item);
}

//! Full scenario: editing AnyValueItem on board of LocalVariableItem that doesn't have any
//! AnyValueItem yet.
TEST_F(WorkspaceEditorActionHandlerTest, OnEditRequestWheNoAnyValueItemIsStilExists)
{
  const bool dialog_was_acccepted = true;

  // creating variable with AnyValue on board
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  EXPECT_EQ(var0->GetAnyValueItem(), nullptr);

  // item mimicking editing result
  auto editing_result = std::make_unique<sup::gui::AnyValueStructItem>();
  auto editing_result_ptr = editing_result.get();
  AnyValueDialogResult dialog_result{dialog_was_acccepted, std::move(editing_result)};

  ON_CALL(m_mock_context, OnEditAnyvalue(nullptr))
      .WillByDefault(::testing::Return(::testing::ByMove(std::move(dialog_result))));

  // preparing handler
  auto handler = CreateActionHandler(var0);

  // expecting call to editing widget
  EXPECT_CALL(m_mock_context, OnEditAnyvalue(::testing::_)).Times(1);

  // editing request
  handler->EditAnyValue();

  // checking that variable got new AnyValueItem
  EXPECT_EQ(var0->GetAnyValueItem(), editing_result_ptr);
}

//! Adding SystemClock variable to the model. It is UniversalVariableItem (no GUI variable
//! counterpart exists, like FileVariable). We check that OnAdVariableRequest correctly create such
//! variables (real-life bug).
TEST_F(WorkspaceEditorActionHandlerTest, OnAddSystemClockVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // pretending that nothing is selected
  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  // adding variable
  handler->AddVariable(domainconstants::kSystemClockVariableType);

  // validating default values of just inserted variable
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
  auto inserted_variable0 =
      dynamic_cast<UniversalVariableItem*>(GetWorkspaceItem()->GetVariables().at(0));
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("var0"));
  EXPECT_EQ(inserted_variable0->GetDomainType(), domainconstants::kSystemClockVariableType);
  EXPECT_EQ(inserted_variable0->GetType(), UniversalVariableItem::GetStaticType());
}

}  // namespace oac_tree_gui::test
