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

#include "sequencergui/composer/instruction_editor_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/epics_instruction_items.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/universal_instruction_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>
#include <testutils/mock_dialog.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

using namespace sequencergui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for InstructionEditorActionHandler class.

class InstructionEditorActionHandlerCopyPasteTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerCopyPasteTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  /**
   * @brief Test helper to create context mimicking current InstructionEditor widget state.
   *
   * @param instruction Currently selected instruction
   * @param current_mime The content of the clipboard.
   *
   * @return Context object.
   */
  InstructionEditorContext CreateContext(InstructionItem* instruction,
                                         const QMimeData* current_mime)
  {
    InstructionEditorContext result;
    result.selected_procedure = [this]() { return m_procedure; };
    result.selected_instruction = [instruction]() { return instruction; };
    result.send_message_callback = m_warning_listener.CreateCallback();
    result.get_mime_data = [current_mime]() { return current_mime; };
    result.set_mime_data = [this](std::unique_ptr<QMimeData> data)
    { m_copy_result = std::move(data); };
    return result;
  }

  std::unique_ptr<InstructionEditorActionHandler> CreateHandler(InstructionItem* instruction,
                                                                const QMimeData* current_mime)
  {
    return std::make_unique<InstructionEditorActionHandler>(
        CreateContext(instruction, current_mime));
  }
  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
  std::unique_ptr<QMimeData> m_copy_result;
};

//! Copy operation when nothing is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  //
  auto handler = CreateHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPasteAfter());  // because mime data is empty
  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(InstructionEditorActionHandlerCopyPasteTest, CopyOperation)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_procedure->GetInstructionContainer());
  sequence->SetName("abc");

  EXPECT_EQ(m_copy_result.get(), nullptr);

  // instruction is selected, no mime
  auto handler = CreateHandler(sequence, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kCopyInstructionMimeType));
}

TEST_F(InstructionEditorActionHandlerCopyPasteTest, CanPasteAfter)
{
  {  // nothing is selected, no mime data
    auto handler = CreateHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);
    EXPECT_FALSE(handler->CanPasteAfter());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateHandler(nullptr, &mime_data);
    EXPECT_FALSE(handler->CanPasteAfter());
  }

  {  // nothing is selected, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);
    auto handler = CreateHandler(nullptr, mime_data.get());
    EXPECT_TRUE(handler->CanPasteAfter());
  }

  {  // selected item in the container, correct mime data
    const WaitItem item_to_paste;
    auto mime_data = CreateCopyMimeData(item_to_paste, kCopyInstructionMimeType);

    auto sequence = m_model.InsertItem<WaitItem>(m_procedure->GetInstructionContainer());
    auto handler = CreateHandler(sequence, mime_data.get());
    EXPECT_TRUE(handler->CanPasteAfter());
  }
}
