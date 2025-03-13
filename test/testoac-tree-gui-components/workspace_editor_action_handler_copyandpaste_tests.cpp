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
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_workspace_editor_context.h>
#include <testutils/test_utils.h>

#include <QMimeData>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorActionHandler for copy-and-paste scenarios.
 */
class WorkspaceEditorActionHandlerCopyAndPasteTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerCopyAndPasteTest() { m_model.InsertItem<WorkspaceItem>(); }

  WorkspaceItem* GetWorkspaceItem() const { return m_model.GetWorkspaceItem(); }

  /**
   * @brief Creates action handler.
   *
   * @param selection Currently selected variable or one of its leafs.
   * @param current_mime The content of the clipboard.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(
      const std::vector<mvvm::SessionItem*>& selection, const QMimeData* current_mime)
  {
    return m_mock_context.CreateActionHandler(GetWorkspaceItem(), selection, current_mime);
  }

  MonitorModel m_model;
  test::MockWorkspaceEditorContext m_mock_context;
};

TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, InitialState)
{
  auto handler = CreateActionHandler(/*selected variable*/ {}, /*mime*/ nullptr);

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPaste());
  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, CopyOperation)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  var0->SetName("abc");

  EXPECT_EQ(m_mock_context.GetCopyResult(), nullptr);

  // instruction is selected, no mime
  auto handler = CreateActionHandler({var0}, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_mock_context.GetCopyResult(), nullptr);
  EXPECT_TRUE(m_mock_context.GetCopyResult()->hasFormat(kCopyVariableMimeType));
}

//! Testing CanPaste.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler({}, nullptr);
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
    EXPECT_FALSE(handler->CanPaste());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateActionHandler({}, &mime_data);
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
    EXPECT_FALSE(handler->CanPaste());
  }

  {  // nothing is selected, correct mime data
    const LocalVariableItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);
    auto handler = CreateActionHandler({}, mime_data.get());
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
    EXPECT_TRUE(handler->CanPaste());
  }

  {  // selected item in the container, correct mime data
    const LocalVariableItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);
    auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
    auto handler = CreateActionHandler({var0}, mime_data.get());
    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);
    EXPECT_TRUE(handler->CanPaste());
  }
}

//! Testing Paste operation for the following scenario: empty model, nothing is selected, pasted
//! item should just appear as a first item.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, PasteAfterIntoEmptyContainer)
{
  // creating mime data representing clipboard content
  LocalVariableItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler({}, mime_data.get());

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);
  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));

  EXPECT_TRUE(handler->CanPaste());
  handler->Paste();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  auto inserted_variable0 = GetWorkspaceItem()->GetVariables().at(0);
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(reported_item, inserted_variable0);
}

//! Testing Paste for the following scenario: two variables in a model, the first one is selected,
//! pasting new variable right after it.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, PasteAfterSelectedItem)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());

  LocalVariableItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);

  // creating action handler mimicking `var0` instruction selected, and mime data in a buffer
  auto handler = CreateActionHandler({var0}, mime_data.get());

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);

  EXPECT_TRUE(handler->CanPaste());
  handler->Paste();

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 3);

  auto inserted_variable0 = GetWorkspaceItem()->GetVariables().at(1);
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(reported_item, inserted_variable0);
}

//! Cut selected variable.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, CutOperation)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());
  auto var1 = m_model.InsertItem<LocalVariableItem>(GetWorkspaceItem());

  // creating action handler mimicking `var0` instruction selected
  auto handler = CreateActionHandler({var0}, nullptr);

  mvvm::SessionItem* reported_item{nullptr};
  EXPECT_CALL(m_mock_context, SelectRequest(testing::_))
      .WillOnce(::testing::SaveArg<0>(&reported_item));
  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(1);

  EXPECT_TRUE(handler->CanCut());
  EXPECT_FALSE(handler->CanPaste());
  handler->Cut();

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // checking the request to select remaining item
  EXPECT_EQ(reported_item, var1);
}

}  // namespace oac_tree_gui::test
