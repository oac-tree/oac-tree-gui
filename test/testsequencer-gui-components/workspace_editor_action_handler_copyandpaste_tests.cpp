/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standard_instruction_items.h"
#include "sequencergui/pvmonitor/workspace_editor_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/test/mock_callback_listener.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/mock_dialog.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

using namespace sequencergui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Testing WorkspaceEditorActionHandler class (copy-and-paste scenarios).

class WorkspaceEditorActionHandlerCopyAndPasteTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerCopyAndPasteTest() { m_model.InsertItem<WorkspaceItem>(); }

  /**
   * @brief Test helper to create context mimicking the current state of WorkspaceEditor widget.
   *
   * @param selected_item Currently selected item (variable, or one of it's attributes).
   *
   * @return Context object.
   */
  WorkspaceEditorContext CreateContext(mvvm::SessionItem* selected_item,
                                       const QMimeData* current_mime)
  {
    WorkspaceEditorContext result;

    // callback returns given item, pretending it is user's selection
    result.selected_item_callback = [selected_item]() { return selected_item; };
    result.selected_workspace_callback = [this]() { return m_model.GetWorkspaceItem(); };
    result.send_message_callback = m_warning_listener.CreateCallback();
    result.edit_anyvalue_callback = {};  // editing is not checked in this test
    result.get_mime_data = [current_mime]() { return current_mime; };
    result.set_mime_data = [this](std::unique_ptr<QMimeData> data)
    { m_copy_result = std::move(data); };

    return result;
  }

  /**
   * @brief Creates action handler for testing.
   * @param selected_item Currently selected item (variable, or one of it's attributes).
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(
      mvvm::SessionItem* selected_item, const QMimeData* current_mime)
  {
    return std::make_unique<WorkspaceEditorActionHandler>(
        CreateContext(selected_item, current_mime));
  }

  WorkspaceItem* GetWorkspaceItem() { return m_model.GetWorkspaceItem(); }

  MonitorModel m_model;
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  std::unique_ptr<QMimeData> m_copy_result;  //!< Result of copy operation.
};

TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, InitialState)
{
  auto handler = CreateActionHandler(/*selected variable*/ nullptr, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPaste());
  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, CopyOperation)
{
  auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
  var0->SetName("abc");

  EXPECT_EQ(m_copy_result.get(), nullptr);

  // instruction is selected, no mime
  auto handler = CreateActionHandler(var0, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kCopyVariableMimeType));
}

//! Testing CanPaste.
TEST_F(WorkspaceEditorActionHandlerCopyAndPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(nullptr, nullptr);
    EXPECT_FALSE(handler->CanPaste());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateActionHandler(nullptr, &mime_data);
    EXPECT_FALSE(handler->CanPaste());
  }

  {  // nothing is selected, correct mime data
    const LocalVariableItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);
    auto handler = CreateActionHandler(nullptr, mime_data.get());

    EXPECT_TRUE(handler->CanPaste());
  }

  {  // selected item in the container, correct mime data
    const LocalVariableItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);

    auto var0 = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
    auto handler = CreateActionHandler(var0, mime_data.get());
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
  auto mime_data = CreateCopyMimeData(item_to_paste, kCopyVariableMimeType);

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler(nullptr, mime_data.get());

  QSignalSpy spy_selection_request(handler.get(), &WorkspaceEditorActionHandler::SelectItemRequest);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  EXPECT_TRUE(handler->CanPaste());
  handler->Paste();
  ASSERT_EQ(m_model.GetWorkspaceItem()->GetVariableCount(), 1);

  auto inserted_variable0 = m_model.GetWorkspaceItem()->GetVariables().at(0);
  ASSERT_NE(inserted_variable0, nullptr);
  EXPECT_EQ(inserted_variable0->GetName(), std::string("abc"));

  // validating request to select just inserted item
  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem*>(spy_selection_request), inserted_variable0);
}
